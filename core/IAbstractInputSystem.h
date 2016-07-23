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

		virtual void addControl(Control const& ctl) = 0;
		virtual void removeControl(int inputCode) = 0;

		virtual void setOwner(ICharacter* owner) = 0;
		virtual ICharacter* getOwner() const = 0;

		virtual void checkInput() = 0;
		virtual void runInput() = 0;
	};


}