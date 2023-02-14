#pragma once
#include "CommonHeaders.h"

namespace Utility
{
	struct PolarCoord {
		float theta = 0;
		float radius = 0;
		glm::vec2 position;
	};
	string readFileContents(string filePath);
	std::vector<unsigned char> readBinaryFileContents(const std::string filename);

	void savePngFile(std::string filename, int w, int h, int comp, unsigned char* data);
	void pngToWebP(std::string filename);

	/*
	* @param str string with the content
	* @param delim string with delimiters
	*/
	vector<string> split(string str, string delim);

	
	/*
	* @param cAngle current angle in degrees
	* @param normal normal angle in degrees
	*/
	float getReflectionAngle(float cAngle,float normal);
}