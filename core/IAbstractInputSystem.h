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

		virtual void addCommand(Command const& command) = 0;
		virtual void removeCommand(int outputCode) = 0;
		virtual void bindInputCode(int in, int out) = 0;
		virtual void unbindInputCode(int inputCode) = 0;

		virtual void setOwner(ICharacter* owner) = 0;
		virtual ICharacter* getOwner() = 0;

		virtual Command* onInputCode(Bind const& input, bool set) = 0;
		virtual void runInput() = 0;

		virtual Bind const& getControl(int inputState) = 0;
	};

	DECLSPEC_CORE IAbstractInputSystem* createUserInputSystem();
}