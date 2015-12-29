#pragma once

#include "VaettirBot.h"

class SmartBot : public VaettirBot {
public:
	SmartBot(const World& world);

	void Update() override;

private:
};
