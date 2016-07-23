
#include "UserInputSystem.h"
#include <Windows.h>

namespace ginkgo
{
	void UserInputSystem::setInputMapping(IInputMapping* inputMapping)
	{
		this->inputMapping = inputMapping;
	}

	IInputMapping* UserInputSystem::getInputMapping() const
	{
		return inputMapping;
	}

	void UserInputSystem::reloadInputMapping()
	{
		if (inputMapping != nullptr)
		{
			//TODO: IInputMapping
		}
	}

	void UserInputSystem::addControl(int in, int out, OnInputFunc callback)
	{
		controlList.push_back(Control(INPUTTYPE_USER, in, out));
		controlStates.push_back(InputState(out, callback));
	}

	void UserInputSystem::removeControl(int inputCode)
	{
		int outCode;
		bool codeFound = false;
		for (UINT32 a = 0; a < controlList.size(); a++)
		{
			if (controlList[a].inputCode == inputCode)
			{
				outCode = controlList[a].outputCode;
				codeFound = true;
				controlList.erase(controlList.begin() + a);
				break;
			}
		}
		if (!codeFound)
		{
			return;
		}

		for (UINT32 a = 0; a < controlStates.size(); a++)
		{
			if (controlStates[a].outputCode == outCode)
			{
				controlStates.erase(controlStates.begin() + a);
				break;
			}
		}
	}

	void UserInputSystem::setOwner(ICharacter* owner)
	{
		this->owner = owner;
	}

	ICharacter* UserInputSystem::getOwner() const
	{
		return owner;
	}
	
	void UserInputSystem::runInput()
	{
		for (InputState& state : controlStates)
		{
			if (state.isSet ^ state.prevSet)
			{
				if (state.inputFunc != nullptr)
				{
					state.inputFunc(this, state.outputCode, state.isSet);
				}
				state.prevSet = state.isSet;
			}
		}
	}

	void UserInputSystem::onInputCode(Control const& input, bool set)
	{
		for (InputState& state : controlStates)
		{
			if (input.outputCode == state.outputCode)
			{
				state.isSet = set;
			}
		}
	}

	Control const& UserInputSystem::getControl(int inputCode)
	{
		for (Control const& c : controlList)
		{
			if (c.inputCode == inputCode)
			{
				return c;
			}
		}

		return invalidControl;
	}



	IAbstractInputSystem* createUserInputSystem()
	{
		return new UserInputSystem();
	}
}