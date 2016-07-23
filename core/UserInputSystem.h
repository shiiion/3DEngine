#pragma once

#include "IAbstractInputSystem.h"

namespace ginkgo
{
	class UserInputSystem : public IAbstractInputSystem
	{
	private:
		const Control invalidControl = Control(INPUTTYPE_INVALID, -1, -1);

		ICharacter* owner;
		vector<Control> controlList;
		vector<InputState> controlStates;

		IInputMapping* inputMapping;
	public:
		virtual void setInputMapping(IInputMapping* inputMapping) override;
		virtual IInputMapping* getInputMapping() const override;

		virtual void reloadInputMapping() override;

		virtual void addControl(int in, int out, OnInputFunc callback) override;
		virtual void removeControl(int inputCode) override;

		virtual void setOwner(ICharacter* owner) override;
		virtual ICharacter* getOwner() const override;

		virtual void onInputCode(Control const& input, bool set) override;
		virtual void runInput() override;

		virtual Control const& getControl(int inputState) override;
	};


}