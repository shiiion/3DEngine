#include "Transform.h"

namespace ginkgo
{
	ITransform* transformFactory()
	{
		return new Transform();
	}
}