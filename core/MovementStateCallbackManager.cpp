#include "MovementStateCallbackManager.h"
#include "Character.h"

namespace ginkgo {
	MovementStateCallbackManager::MovementStateCallbackManager()
	{
		RegisteredMovementState fallingState("FallingMovementState", [](const ICharacter&, float) { return false; }, resolveFreemove, [](ICharacter&, float) { return; }, [](ICharacter&, float) { return; });
		this->RegisterMovementState(fallingState);
		RegisteredMovementState walkingState("WalkingMovementState", checkWalking, resolveWalking, walkingEnabled, walkingDisabled);
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

	int MovementStateCallbackManager::GetMovementStateID(const std::string& state_name) const
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

	void MovementStateCallbackManager::CheckMovementStates(const std::vector<IEntity*>& entities, float elapsedTime)
	{
		for (IEntity* entity : entities) {
			ICharacter* character = dynamic_cast<ICharacter*>(entity);
			if (character == nullptr) continue;
			bool stateFound = false;
			//TODO: ignore world static objects
			//character->setMovementState(0); JASON?????
			for (std::vector<int>::size_type i = 0; i < character->getMovementStates().size(); ++i) {
				const RegisteredMovementState& state = this->states.at(i);
				if (state.CheckMovementState(*character, elapsedTime)) 
				{
					if (state.name.compare(states.at(character->getMovementState()).name) != 0)
					{
						states.at(character->getMovementState()).OnStateDisabled(*character, elapsedTime);
						state.OnStateEnabled(*character, elapsedTime);
					}
					character->setMovementState(i);
					stateFound = true;
					break;
				}
			}

			if (!stateFound)
			{
				if (states.at(0).name.compare(states.at(character->getMovementState()).name) != 0)
				{
					states.at(character->getMovementState()).OnStateDisabled(*character, elapsedTime);
					states.at(0).OnStateEnabled(*character, elapsedTime);
				}
				character->setMovementState(0);
			}
		}
	}

	void MovementStateCallbackManager::DoCallbacks(const std::vector<IEntity*>& entities, float elapsedTime)
	{
		for (IEntity* entity : entities) {
			ICharacter* character = dynamic_cast<ICharacter*>(entity);
			if (character == nullptr) continue;
			this->states.at(character->getMovementState()).OnMovementState(*character, elapsedTime); //masking?
		}
	}

	std::vector<RegisteredMovementState> MovementStateCallbackManager::GetRegisteredMovementStates() const
	{
		return this->states;
	}
}