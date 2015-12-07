#pragma once

#include <string>

#include "Action.h"

class LogAction : public Action {
public:
	LogAction(std::string message) : message_(message) {}

	void Perform(const World& ) override {
		printf("%s\n", message_.c_str());
	}

private:
	const std::string message_;
};
