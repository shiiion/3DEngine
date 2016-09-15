#include "MovementStateCallbackManager.h"


namespace ginkgo {
	MovementStateCallbackManager::MovementStateCallbackManager()
	{}


	MovementStateCallbackManager::~MovementStateCallbackManager()
	{}

	void MovementStateCallbackManager::RegisterMovementState(const RegisteredMovementState & state)
	{}

	int MovementStateCallbackManager::GetMovementStateID(const std::wstring& state_name) const
	{
		return 0;
	}

	std::wstring MovementStateCallbackManager::GetMovementStateName(int ID) const
	{
		return this->states[ID].name;
	}

	void MovementStateCallbackManager::CheckMovementStates(const std::vector<IEntity*>& entities)
	{
		for (const IEntity* entity : entities) {
			for (int i = 0; i < this->states.size(); ++i) {
				const RegisteredMovementState& state = this->states[i];
				if ((state.CheckMovementState)(*entity)) {
					//entity->setMovementState(i); //commented out because i can't figure out a good way to do it yet
					break;
				}
			}
		}
	}

	void MovementStateCallbackManager::DoCallbacks(const std::vector<IEntity*>& entities)
	{

	}
}