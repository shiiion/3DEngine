#pragma once

#include <vector>

#include "RegisteredMovementState.h"

/*
Unsolved problems:
multiple movement states
masks for what callbacks should be applied
exempting certain entities from callbacks
*/

namespace ginkgo {
	class MovementStateCallbackManager {
	public:
		MovementStateCallbackManager();
		~MovementStateCallbackManager();
		void RegisterMovementState(const RegisteredMovementState& state);
		int GetMovementStateID(const std::wstring& state_name) const;
		std::wstring GetMovementStateName(int ID) const;
		void CheckMovementStates(const std::vector<IEntity*>& entities);
		void DoCallbacks(const std::vector<IEntity*>& entities);


	private:
		std::vector<RegisteredMovementState> states;
	};
}

