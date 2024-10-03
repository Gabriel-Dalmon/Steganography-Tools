#pragma once

class SteganographyHelper
{
	public:
	static void HideTextInImage(const char* imagePath, const char* textToHide, const char* outputImagePath);
	static void ExtractTextFromImage(const char* imagePath, char* outputText, int outputTextSize);
};