#pragma once

#include <iostream>
#include <fstream>

#include <TinyObjLoader\tiny_obj_loader.h>

#include "ObjLoader.h"

namespace ginkgo {

	ObjLoader::ObjLoader(const string& path)
	{
		tinyobj::attrib_t attrib;
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
			std::cerr << "Filepath for obj wrong." << std::endl;
			throw std::runtime_error(err);
		}

		if (attrib.normals.size() == 0)
		{
			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					uvs.push_back(
						vec2(
							attrib.texcoords[2 * index.texcoord_index + 0],
							attrib.texcoords[2 * index.texcoord_index + 1]
						)
					);
					
					positions.push_back(
						vec3(
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
						)
					);

					indices.push_back(indices.size());
				}
			}
		}
		else
		{
			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					uvs.push_back(
						vec2(
							attrib.texcoords[2 * index.texcoord_index + 0],
							attrib.texcoords[2 * index.texcoord_index + 1]
						)
					);

					positions.push_back(
						vec3(
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
						)
					);
					
					normals.push_back(
						vec3(
							attrib.normals[3 * index.normal_index + 0],
							attrib.normals[3 * index.normal_index + 1],
							attrib.normals[3 * index.normal_index + 2]
						)
					);

					indices.push_back(indices.size());
				}
			}
		}
	}
}