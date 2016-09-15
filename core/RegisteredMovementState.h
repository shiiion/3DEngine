#pragma once

#include <string>
#include <functional>

#include "IEntity.h"

namespace ginkgo {
	struct RegisteredMovementState {
		std::wstring name;
		std::function<bool(const IEntity&)> CheckMovementState;
		std::function<void(IEntity&)> OnMovementState;
	};
}