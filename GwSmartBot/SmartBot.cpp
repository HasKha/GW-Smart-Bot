#include "SmartBot.h"

SmartBot::SmartBot(const World& world)
	: VaettirBot(world) {

}

void SmartBot::Update() {
	if (world_.player().Id == 0) return;
	if (GWCAClient::Api().GetInstancType() != GwConstants::InstanceType::Explorable) return;
	if (GWCAClient::Api().GetMapID() != GwConstants::MapID::Jaga_Moraine) return;


}
