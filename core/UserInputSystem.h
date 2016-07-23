#pragma once

#include "IAbstractInputSystem.h"

namespace ginkgo
{
	class UserInputSystem : public IAbstractInputSystem
	{
	private:
		ICharacter* owner;
		vector<Control> controlList;
		vector<InputState> controlStates;

		IInputMapping* inputMapping;
	public:
		virtual void setInputMapping(IInputMapping* inputMapping) override;
		virtual IInputMapping* getInputMapping() const override;

		virtual void reloadInputMapping() override;

		virtual void addControl(Control const& ctl) override;
		virtual void removeControl(int inputCode) override;

		virtual void setOwner(ICharacter* owner) override;
		virtual ICharacter* getOwner() const override;

		virtual void checkInput() override;
		virtual void runInput() override;
	};


}