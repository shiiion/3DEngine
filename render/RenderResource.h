#pragma once


#ifdef COMP_DLL_RENDER
#define DECLSPEC_RENDER __declspec(dllexport)
#else
#define DECLSPEC_RENDER __declspec(dllimport)
#endif


#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <string>

using std::vector;
using std::string;
using std::map;
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::mat4;
using glm::quat;