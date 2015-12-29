#pragma once

#include <vector>
#include <queue>

#include "VaettirBot.h"
#include "Actions.h"
#include "ActionQueue.h"

class FixedPathBot : public VaettirBot {
public:
	FixedPathBot(const World& world);
	
	void Update() override;

private:
	//Action* current_action_;
	//std::queue<Action*> action_queue_;
	ActionQueue action_queue_;
};
