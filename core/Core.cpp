#include "Core.h"
#include "IWorld.h"

namespace ginkgo
{
	long Core::entityIDBase = 1;
	Core Core::core = Core();

	long Core::generateID()
	{
		return entityIDBase++;
	}

	void Core::coreThread()
	{
		vector<
	}
}