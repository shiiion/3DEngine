
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

	Command const& UserInputSystem::findCommand(int outputCode) const
	{
		for (Command const& c : commandList)
		{
			if (c.outputCode == outputCode)
			{
				return c;
			}
		}
		return invalidCommand;
	}

	void UserInputSystem::addCommand(Command const& command)
	{
		commandList.emplace_back(command);
	}

	void UserInputSystem::bindInputCode(int in, int out)
	{
		Command const& outputCommand = findCommand(out);
		if (outputCommand.onCommand == nullptr && outputCommand.outputCode == -1)
		{
			return;
		}
		bindings.push_back(Bind(INPUTTYPE_USER, in, out));
		controlStates.push_back(CommandState(outputCommand));
	}

	void UserInputSystem::unbindInputCode(int inputCode)
	{
		int outCode;
		bool codeFound = false;
		for (UINT32 a = 0; a < bindings.size(); a++)
		{
			if (bindings[a].inputCode == inputCode)
			{
				outCode = bindings[a].outputCode;
				codeFound = true;
				bindings.erase(bindings.begin() + a);
				break;
			}
		}
		if (!codeFound)
		{
			return;
		}

		for (UINT32 a = 0; a < controlStates.size(); a++)
		{
			if (controlStates[a].command.outputCode == outCode)
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

	ICharacter* UserInputSystem::getOwner()
	{
		return owner;
	}

	void UserInputSystem::runInput()
	{
		for (CommandState& state : controlStates)
		{
			if (state.isSet ^ state.prevSet)
			{
				if (state.command.onCommand != nullptr)
				{
					state.command.onCommand(this, state.command.outputCode, state.isSet);
				}
				state.prevSet = state.isSet;
			}
		}
	}

	void UserInputSystem::onInputCode(Bind const& input, bool set)
	{
		for (CommandState& state : controlStates)
		{
			if (input.outputCode == state.command.outputCode)
			{
				state.isSet = set;
			}
		}
	}

	Bind const& UserInputSystem::getControl(int inputCode)
	{
		for (Bind const& c : bindings)
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