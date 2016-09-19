#pragma once

#include <string>
#include <functional>

#include "IEntity.h"

namespace ginkgo {
	struct RegisteredMovementState {
		RegisteredMovementState(const std::wstring& name, const std::function<bool(const IEntity&)>& CheckMovementState, const std::function<void(IEntity&)>& OnMovementState) :
			name(name),
			CheckMovementState(CheckMovementState),
			OnMovementState(OnMovementState)
		{
		}

		std::wstring name;
		std::function<bool(const IEntity&)> CheckMovementState;
		std::function<void(IEntity&)> OnMovementState;
	};

	static const RegisteredMovementState NullMovementState(L"NullMovementState", [](const IEntity&) {return false; }, [](const IEntity&) {return; });
}