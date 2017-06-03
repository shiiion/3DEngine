#include "ResourceManagement.h"
#include "Mesh.h"
#include "ObjLoader.h"
#include "Texture.h"
#include "FileUtils.h"

namespace ginkgo
{
	static map<string, Mesh*> meshHash;
	static map<string, Texture*> textureHash;


	Mesh* createMesh(const vector<vec3>& positions, const vector<unsigned int>& indices, const vector<vec2>& uvs, string const& UID, const vector<vec3>& normals)
	{
		if (meshHash.find(UID) != meshHash.end())
		{
			//TODO: error handle
			return nullptr;
		}
		Mesh* ret = new Mesh();
		ret->addData(positions, indices, uvs, normals);
		meshHash[UID] = ret;
		return ret;
	}

	Mesh* loadMesh(const string& path, const string& UID)
	{
		try
		{
			//repeat this to avoid loading mesh and failure
			if (meshHash.find(UID) != meshHash.end())
			{
				//TODO: error handle
				return nullptr;
			}
			ObjLoader meshData(path);
			return createMesh(meshData.getPositionList(), meshData.getIndexList(), meshData.getUVList(), UID, meshData.getNormalList());
		}
		catch (std::runtime_error e)
		{
			//TODO: logme
			return nullptr;
		}
	}

	Texture* createTexture(const string& path, bool pixelate, const string& UID)
	{
		if (textureHash.find(UID) != textureHash.end())
		{
			//TODO: error handle
			return nullptr;
		}

		Texture* t = new Texture();
		BYTE* pixels = FileUtils::loadImage(path.c_str(), &t->width, &t->height);
		GLenum err;
		err = glGetError();
		if (err != GL_NO_ERROR)
		{
			//TODO: error handle (return static checkerboard texture source engine style)
			delete[] pixels;
			return nullptr;

		}
		glGenTextures(1, &t->tid);
		glBindTexture(GL_TEXTURE_2D, t->tid);
		GLenum option = (pixelate) ? GL_NEAREST : GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		err = glGetError();
		glBindTexture(GL_TEXTURE_2D, 0);
		if (err != GL_NO_ERROR)
		{
			//TODO: error handle (return static checkerboard texture source engine style)
			delete[] pixels;
			return nullptr;
		}
		t->filepath = path;

		delete[] pixels;

		textureHash[UID] = t;

		return t;
	}


	Mesh* retrieveMesh(const string& UID)
	{
		if (meshHash.find(UID) == meshHash.end())
		{
			return nullptr;
		}
		return meshHash[UID];
	}

	Texture* retrieveTexture(const string& UID)
	{
		if (textureHash.find(UID) == textureHash.end())
		{
			return nullptr;
		}
		return textureHash[UID];
	}

	void deallocMesh(const string& UID)
	{
		auto pos = meshHash.find(UID);
		if (pos == meshHash.end())
		{
			return;
		}
		Mesh* m = meshHash[UID];
		meshHash.erase(pos);
		delete m;
	}

	void deallocTexture(const string& UID)
	{
		auto pos = textureHash.find(UID);
		if (pos == textureHash.end())
		{
			return;
		}
		Texture* t = textureHash[UID];
		textureHash.erase(pos);
		delete t;
	}
}