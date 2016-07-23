
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

	void UserInputSystem::addControl(Control const& ctl)
	{
		controlList.push_back(ctl);
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
		for (InputState const& state : controlStates)
		{
			if (state.isSet || state.prevSet)
			{
				if (state.inputFunc != nullptr)
				{
					state.inputFunc(this, state.outputCode);
				}
			}
		}
	}

	void UserInputSystem::checkInput()
	{
		vector<int> setCodes;
		vector<int> resetCodes;
		for (UINT32 a = 0; a < controlList.size(); a++)
		{
			if (GetAsyncKeyState(controlList[a].inputCode))
			{
				setCodes.emplace_back(controlList[a].outputCode);
			}
			else
			{
				resetCodes.emplace_back(controlList[a].outputCode);
			}
		}
		for (UINT32 a = 0; a < setCodes.size(); a++)
		{
			for (UINT32 b = 0; b < controlStates.size(); b++)
			{
				if (setCodes[a] == controlStates[b].outputCode)
				{
					controlStates[b].prevSet = controlStates[b].isSet;
					controlStates[b].isSet = true;
					break;
				}
			}
		}
		for (UINT32 a = 0; a < resetCodes.size(); a++)
		{
			for (UINT32 b = 0; b < controlStates.size(); b++)
			{
				if (resetCodes[a] == controlStates[b].outputCode)
				{
					controlStates[b].prevSet = controlStates[b].isSet;
					controlStates[b].isSet = false;
					break;
				}
			}
		}
	}


}