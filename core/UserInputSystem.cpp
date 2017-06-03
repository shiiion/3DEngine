
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
		for (Command const* c : commandList)
		{
			if (c->outputCode == outputCode)
			{
				return *c;
			}
		}
		return invalidCommand;
	}

	void UserInputSystem::addCommand(Command const& command)
	{
		//disallow this (TODO: log it too)
		if (command.outputCode == -1 || findCommand(command.outputCode).outputCode != -1)
		{
			return;
		}
		
		commandList.emplace_back(command.clone());
	}

	void UserInputSystem::removeCommand(int outputCode)
	{
		for (int a = 0; a < commandList.size(); a++)
		{
			if (commandList[a]->outputCode == outputCode)
			{
				delete commandList[a];
				commandList.erase(commandList.begin() + a);
				return;
			}
		}
	}

	void UserInputSystem::bindInputCode(int in, int out)
	{
		Command const& outputCommand = findCommand(out);
		if (outputCommand.outputCode == -1)
		{
			return;
		}
		bindings.emplace_back(Bind(INPUTTYPE_USER, in, out));
		controlStates.emplace_back(new CommandState(outputCommand));
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
			if (controlStates[a]->command->outputCode == outCode)
			{
				delete controlStates[a];
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
		for (CommandState* state : controlStates)
		{
			switch (state->command->type)
			{
				case CommandParams::NO_PARAMS:
				{
					CommandSetReset* param = (CommandSetReset*)state->command;

					if (state->isSet ^ state->prevSet)
					{
						if (param->onInput != nullptr)
						{
							param->onInput(this, param->outputCode, state->isSet);
						}
						state->prevSet = state->isSet;
					}
				}
				break;
				case CommandParams::FLOAT_2:
				{
					Command2f* param = (Command2f*)state->command;
					if (param->onInput == nullptr) break;
					param->onInput(this, param->outputCode, param->a, param->b);
					//mark that we have processed this command (UNUSED)
					state->isSet = false;
				}
				break;
			}
		}
	}

	Command* UserInputSystem::onInputCode(Bind const& input, bool set)
	{
		for (CommandState* state : controlStates)
		{
			if (input.outputCode == state->command->outputCode)
			{
				state->isSet = set;
				return state->command;
			}
		}
		return nullptr;
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