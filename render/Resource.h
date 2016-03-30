#pragma once

#ifdef COMP_DLL_RENDER
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
