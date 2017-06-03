#pragma once

#include "RenderResource.h"

namespace ginkgo
{
	class Mesh;
	struct Texture;

	DECLSPEC_RENDER Mesh* createMesh(const vector<vec3>& positions, const vector<unsigned int>& indices, const vector<vec2>& uvs, string const& UID, const vector<vec3>& normals = vector<vec3>());
	DECLSPEC_RENDER Mesh* loadMesh(const string& path, string const& UID);
	DECLSPEC_RENDER Texture* createTexture(const string& path, bool pixelate, const string& UID);

	DECLSPEC_RENDER Mesh* retrieveMesh(const string& UID);
	DECLSPEC_RENDER Texture* retrieveTexture(const string& UID);

	DECLSPEC_RENDER void deallocMesh(const string& UID);
	DECLSPEC_RENDER void deallocTexture(const string& UID);
}