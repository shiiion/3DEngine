#pragma once

#include "RenderResource.h"

#define CM_RIGHT 0 
#define CM_LEFT 1
#define CM_TOP 2
#define CM_BOTTOM 3
#define CM_FRONT 4
#define CM_BACK 5

namespace ginkgo
{
	class ICubeMap
	{
	public:
		virtual void draw(const mat4& transformProjectionView) const = 0;
		virtual void bindCubeMapTexture() const = 0;
		virtual void unbindCubeMapTexture() const = 0;
	};

	DECLSPEC_RENDER ICubeMap* cubeMapFactory(map<unsigned int, string>& faces, float scale);
}