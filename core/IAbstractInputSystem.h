#pragma once

#include "CoreReource.h"

namespace ginkgo
{
	class IInputMapping;
	class ICharacter;

	class IAbstractInputSystem
	{
	public:
		virtual void setInputMapping(IInputMapping* inputMapping) = 0;
		virtual IInputMapping* getInputMapping() const = 0;

		virtual void reloadInputMapping() = 0;

		virtual void addControl(int in, int out, OnInputFunc callback) = 0;
		virtual void removeControl(int inputCode) = 0;

		virtual void setOwner(ICharacter* owner) = 0;
		virtual ICharacter* getOwner() = 0;

		virtual void onInputCode(Control const& input, bool set) = 0;
		virtual void runInput() = 0;

		virtual Control const& getControl(int inputState) = 0;
	};

	DECLSPEC_CORE IAbstractInputSystem* createUserInputSystem();
}