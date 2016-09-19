#include "MovementStateCallbackManager.h"
#include "ICharacter.h"

namespace ginkgo {
	MovementStateCallbackManager::MovementStateCallbackManager()
	{
		RegisteredMovementState fallingState = NullMovementState;
		fallingState.name = L"FallingMovementState";
		this->RegisterMovementState(fallingState);
		RegisteredMovementState walkingState = NullMovementState;
		walkingState.name = L"WalkingMovementState";
		this->RegisterMovementState(walkingState);
	}


	MovementStateCallbackManager::~MovementStateCallbackManager()
	{
	}

	int MovementStateCallbackManager::RegisterMovementState(const RegisteredMovementState & state)
	{
		this->states.push_back(state);
		return this->states.size() - 1;
	}

	int MovementStateCallbackManager::GetMovementStateID(const std::wstring& state_name) const
	{
		for (size_t i = 0; i < this->states.size(); ++i) {
			if (state_name == this->states[i].name) {
				return i;
			}
		}
		return -1;
	}

	const RegisteredMovementState& MovementStateCallbackManager::GetMovementState(int ID) const
	{
		return this->states.at(ID);
	}

	RegisteredMovementState& MovementStateCallbackManager::GetMovementState(int ID)
	{
		return this->states.at(ID);
	}

	void MovementStateCallbackManager::CheckMovementStates(const std::vector<IEntity*>& entities)
	{
		for (IEntity* entity : entities) {
			for (std::vector<int>::size_type i = 0; i < entity->getAllowedMovementStates().size(); ++i) {
				const RegisteredMovementState& state = this->states.at(i);
				if (state.CheckMovementState(*entity)) {
					entity->setMovementState(i);
					break;
				}
			}
		}
	}

	void MovementStateCallbackManager::DoCallbacks(const std::vector<IEntity*>& entities)
	{
		for (IEntity* entity : entities) {
			this->states.at(entity->getMovementState()).OnMovementState(*entity); //masking?
		}
	}

	std::vector<RegisteredMovementState> MovementStateCallbackManager::GetRegisteredMovementStates() const
	{
		return this->states;
	}
}