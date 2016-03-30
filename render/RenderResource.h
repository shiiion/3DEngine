#pragma once

#ifdef COMP_DLL_RENDER
#define DECLSPEC_RENDER __declspec(dllexport)
#else
#define DECLSPEC_RENDER __declspec(dllimport)
#endif
