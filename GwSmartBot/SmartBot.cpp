#include "SmartBot.h"

class PlannedMoveAction : public Action {
	static const int MOVE_INTERVAL = 1000;
	const float precision = 200.0f;

public:
	PlannedMoveAction(const PathPlanner& planner) : planner_(planner) {}

	void Perform(const World& world) override {
		last_moved_ = std::clock();
		Point2f dest = planner_.GetNextDestination(world);
		GWCAClient::Api().Move(dest.x(), dest.y());
	}

	void Resume(const World& world) override {
		Perform(world);
	}

	void Update(const World& world) override {
		if (world.player().MoveX == 0 && world.player().MoveY == 0) {
			printf("not moving! blocked = %d\n", blocked_);

			++blocked_;

			if (blocked_ < 15) {
				MoveIfPossible(planner_.GetNextDestination(world), 250);
			} else {
				if (GWCAClient::Api().IsSkillRecharged(SkillSlot::HoS)) {
					GWCAClient::Api().UseSkill(SkillSlot::HoS, Target::Self);
					blocked_ = 0;
				}
			}
		} else {
			blocked_ = 0;
			MoveIfPossible(planner_.GetNextDestination(world));
		}
	}

	bool Done(const World& world) override {
		Point2f dest = planner_.GetFinalDestination();
		return Utils::GetSquaredDistance(world.player().X, world.player().Y,
			dest.x(), dest.y()) < (precision * precision);
	}

private:
	void MoveIfPossible(Point2f dest, int time = MOVE_INTERVAL) {
		if (std::clock() - last_moved_ > time) {
			last_moved_ = std::clock();
			GWCAClient::Api().Move(dest.x(), dest.y());
		}
	}

	int blocked_ = 0;
	const PathPlanner& planner_;
	std::clock_t last_moved_;
};

SmartBot::SmartBot(const World& world)
	: VaettirBot(world) {

	const int NORTH_SOUTH_LINE = -19930;

	action_queue_.Append(new LogAction("Doing left side"));
	action_queue_.Append(new CustomSimpleAction([NORTH_SOUTH_LINE, this](const World&) {
		path_planner_.SetFinalDestination(Point2f(8982, -20576));
		path_planner_.SetValidatorFunc([NORTH_SOUTH_LINE](AgentPosition apos) {
			return apos.y < NORTH_SOUTH_LINE;
		});
	}));
	action_queue_.Append(new MoveAction(13501, -20925));
	action_queue_.Append(new MoveAction(13172, -22137));
	action_queue_.Append(new CustomSimpleAction([](const World& world) {
		float closest_dist = GwConstants::SqrRange::Compass;
		DWORD closest_id = Target::Self;
		for (AgentPosition agent : world.agents()) {
			if (closest_dist > Utils::GetSquaredDistance(world.player(), agent)) {
				closest_dist = Utils::GetSquaredDistance(world.player(), agent);
				closest_id = agent.id;
			}
		}
		GWCAClient::Api().ChangeTarget(closest_id);
	}));
	action_queue_.Append(new PlannedMoveAction(path_planner_));

	action_queue_.Append(new LogAction("Waiting for left ball"));
	action_queue_.Append(new WaitAction(14 * 1000));
	action_queue_.Append(new UseSkillAction(SkillSlot::HoS, Target::Current));
	action_queue_.Append(new WaitAction(6 * 1000));

	action_queue_.Append(new LogAction("Doing right side"));
	action_queue_.Append(new CustomSimpleAction([NORTH_SOUTH_LINE, this](const World&) {
		path_planner_.SetFinalDestination(Point2f(12476, -16157));
		path_planner_.SetValidatorFunc([NORTH_SOUTH_LINE](AgentPosition apos) {
			return apos.y > NORTH_SOUTH_LINE;
		});
	}));
	action_queue_.Append(new PlannedMoveAction(path_planner_));

	action_queue_.Append(new LogAction("Waiting for right ball"));
	action_queue_.Append(new WaitAction(15 * 1000));
	action_queue_.Append(new UseSkillAction(SkillSlot::HoS, Target::Current));
	action_queue_.Append(new WaitAction(5 * 1000));

	action_queue_.Append(new LogAction("Blocking enemies"));
	action_queue_.Append(new MoveAction(12920, -17032, 30));
	action_queue_.Append(new MoveAction(12847, -17136, 30));
	action_queue_.Append(new MoveAction(12720, -17222, 30));
	action_queue_.Append(new WaitAction(300));
	action_queue_.Append(new MoveAction(12617, -17273, 30));
	action_queue_.Append(new WaitAction(300));
	action_queue_.Append(new MoveAction(12518, -17305, 20));
	action_queue_.Append(new WaitAction(300));
	action_queue_.Append(new MoveAction(12445, -17327, 10));

	action_queue_.Append(new LogAction("Please take over, and kill them all"));
}

void SmartBot::Update(bool do_actions) {
	if (world_.player().Id == 0) return;
	if (GWCAClient::Api().GetInstancType() != GwConstants::InstanceType::Explorable) return;
	if (GWCAClient::Api().GetMapID() != GwConstants::MapID::Jaga_Moraine) return;

	path_planner_.Update(world_);

	static bool resume = false;

	if (do_actions) {
		if (!cast_queue_.Empty()) {
			cast_queue_.Update(world_);
			resume = true;
		} else {
			UpdateCounts();

			StayAlive();

			action_queue_.Update(world_);
		}
	}
}
