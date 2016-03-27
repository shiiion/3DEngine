#pragma once

#ifdef COMP_DLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport)
#endif
namespace ginkgo
{
};