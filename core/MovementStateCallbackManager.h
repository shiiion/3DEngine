#pragma once

#include <vector>
#include "CoreReource.h"

/*
Unsolved problems:
multiple movement states
masks for what callbacks should be applied
exempting certain entities from callbacks
*/

namespace ginkgo {
	class IEntity;

	class MovementStateCallbackManager {
	public:
		MovementStateCallbackManager();
		~MovementStateCallbackManager();
		int RegisterMovementState(const RegisteredMovementState& state);
		int GetMovementStateID(const std::string& state_name) const;
		RegisteredMovementState& GetMovementState(int ID);
		const RegisteredMovementState& GetMovementState(int ID) const;
		void CheckMovementStates(const std::vector<IEntity*>& entities, float elapsedTime);
		void DoCallbacks(const std::vector<IEntity*>& entities, float elapsedTime);
		std::vector<RegisteredMovementState> GetRegisteredMovementStates() const;


	private:
		std::vector<RegisteredMovementState> states;
	};
}

