#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "Fileutils.h"

#include "Mesh.h"
#include "CubeMap.h"

namespace ginkgo {

	string FileUtils::read_file(const char* filepath)
	{
		FILE* file = fopen(filepath, "rt");	//we read as t -> textfile bc if b -> bytes then it would all be in one line like in bytes
		fseek(file, 0, SEEK_END);
		unsigned long length = ftell(file); // binary streams -> number of bytes, text streams -> number of characters

		char* data = new char[length + 1]; // + Termination character
		memset(data, 0, length + 1);	//use memset to clear away any binary text
		fseek(file, 0, SEEK_SET);	//Back to beginning
		fread(data, 1, length, file);
		fclose(file);

		string result(data);
		delete[] data;

		return result;
	}

	BYTE* FileUtils::loadImage(const char* filename, GLsizei* width, GLsizei* height, double rotationAngleInDegrees)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib = nullptr;

		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		if(fmod(rotationAngleInDegrees,360) != 0)
			dib = FreeImage_Rotate(dib, rotationAngleInDegrees);

		if (FreeImage_GetBPP(dib) != 24)
			dib = FreeImage_ConvertTo24Bits(dib);

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		int size = *width * *height * (bits / 8);
		BYTE* result = new BYTE[size];
		memcpy(result, pixels, size);
		FreeImage_Unload(dib);

		return result;
	}

	void FileUtils::screenshot(unsigned int width, unsigned int height)
	{
		static int id = 0;
		
		unsigned char* pixels = new unsigned char[3 * width * height];

		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

		FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
		FreeImage_Save(FIF_BMP, image, ("Render" + std::to_string(id) + ".bmp").c_str(), 0);
		//FreeImage_Save(FIF_BMP, image, "Render.bmp", 0);
		
		FreeImage_Unload(image);
		delete[] pixels;

		id++;
	}

}