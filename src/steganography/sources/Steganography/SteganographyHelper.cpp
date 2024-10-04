#include "pch.h"


int SteganographyHelper::EncryptText(const char* text, byte* pixelBuffer, size_t pixelBufferSize)
{
	size_t textLength = std::strlen(text);
	if (textLength > MAX16BITS || textLength * 6 >= pixelBufferSize) {
		return 1;
	}
	byte* cursor = pixelBuffer + 12;
	BitwiseOperationsHelper::SetBytes(textLength, 2, cursor);
	cursor = pixelBuffer + 24;
	for (int i = 0; i < textLength; i++) {
		BitwiseOperationsHelper::SetBytes(text[i], 2, cursor);
		cursor += 6;
	}
	cursor = pixelBuffer;
	BitwiseOperationsHelper::SetSignEncrypted(cursor);
	return 0;
}

const char* SteganographyHelper::ReadEncryptedText(byte* pixelBuffer, size_t pixelBufferSize)
{
	if (!CheckEncryptionSignature(pixelBuffer)) {
		return nullptr;
	}
	byte* cursor = pixelBuffer + 12;
	unsigned int textLength = BitwiseOperationsHelper::ReadBytes(2, pixelBuffer + 12);
	char* returnArray = new char[textLength + 1];
	cursor = pixelBuffer + 24;
	for (int i = 0; i < textLength; i++) {
		returnArray[i] = (char)BitwiseOperationsHelper::ReadBytes(1, cursor);
		cursor += 6;
	}
	returnArray[textLength] = '\0';
	return returnArray;
}

// Signature is stored on the first 2 bytes of the pixel buffer.
bool SteganographyHelper::CheckEncryptionSignature(byte* pixelBuffer)
{
	unsigned int signature = BitwiseOperationsHelper::ReadBytes(2, pixelBuffer);
	if (signature == ENCRYPTSIGN) {
		return true;
	}
	return false;
}

void SteganographyHelper::EncryptCharInPixels(char c, byte* pixelBuffer, EncodingSchema* encodingSchema)
{
	MessageBox(0, L"EncryptCharInPixels function is not implemented.", 0, 0);
	return;
}

void SteganographyHelper::EncryptTextInPixelBuffer(const char* text, byte* pixelBuffer, size_t bufferSize)
{
	MessageBox(0, L"EncryptTextInPixelBuffer function is not implemented.", 0, 0);
	return;
	int textLength = strlen(text);
	int textIndex = 0;
	int pixelIndex = 0;
	int bitIndex = 0;
	if (textLength > MAX16BITS || bufferSize < textLength * 6)
	{
		return;
	}
	while (textIndex < textLength)
	{
		char c = text[textIndex];
		for (int i = 0; i < 8; i++)
		{
			byte bit = (c >> i) & 1;
			pixelBuffer[pixelIndex] = (pixelBuffer[pixelIndex] & 0xFE) | bit;
			pixelIndex++;
		}
		textIndex++;
	}

}

const char* SteganographyHelper::ReadTextFromPixelBuffer(byte* pixelBuffer)
{
	MessageBox(0, L"ReadTextFromPixelBuffer function is not implemented.", 0, 0);
	return nullptr;
}