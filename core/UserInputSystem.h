#pragma once

#include "IAbstractInputSystem.h"

namespace ginkgo
{
	class UserInputSystem : public IAbstractInputSystem
	{
	private:
		const Bind invalidControl = Bind(INPUTTYPE_INVALID, -1, -1);
		const CommandSetReset invalidCommand = CommandSetReset(-1, nullptr);

		ICharacter* owner;
		vector<Bind> bindings;
		vector<Command*> commandList;
		vector<CommandState*> controlStates;

		IInputMapping* inputMapping;

		Command const& findCommand(int outputCode) const;
	public:
		virtual void setInputMapping(IInputMapping* inputMapping) override;
		virtual IInputMapping* getInputMapping() const override;

		virtual void reloadInputMapping() override;

		virtual void addCommand(Command const& command) override;
		virtual void removeCommand(int outputCode) override;

		virtual void bindInputCode(int in, int out) override;
		virtual void unbindInputCode(int inputCode) override;

		virtual void setOwner(ICharacter* owner) override;
		virtual ICharacter* getOwner() override;

		virtual Command* onInputCode(Bind const& input, bool set) override;
		virtual void runInput() override;

		virtual Bind const& getControl(int inputState) override;

	};


}