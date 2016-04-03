#pragma once

#include "meshs/RenderMesh.h"

namespace ginkgo
{

#define RENDERER_MAX_SPRITES	65535
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32

#define SHADER_VERTEX_INDEX	0
#define SHADER_UV_INDEX		1
#define SHADER_TID_INDEX	2
#define SHADER_COLOR_INDEX	3


	class IRenderEngine
	{
	private:
		virtual void init() = 0;
	public:
		virtual void begin() = 0;
		virtual void submit(const RenderMesh* mesh) = 0;
		virtual void end() = 0;
		virtual void flush() = 0;
	};
	
	DECLSPEC_RENDER IRenderEngine* createRenderEngine();

}

