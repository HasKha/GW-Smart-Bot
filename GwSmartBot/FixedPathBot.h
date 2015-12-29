#pragma once

#include <vector>
#include <queue>

#include "VaettirBot.h"
#include "Actions.h"
#include "ActionQueue.h"

class FixedPathBot : public VaettirBot {
public:
	FixedPathBot(const World& world);
	
	void Update(bool do_actions) override;

private:
};
