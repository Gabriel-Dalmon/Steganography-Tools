#pragma once

struct EncodingSchema {
	byte red : 2;
	byte green : 1;
	byte blue : 1;
};

class SteganographyHelper
{
public:
	static int EncryptText(const char* text, byte* pixelBuffer, size_t pixelBufferSize);
	// Should be renamed with a better name.
	static inline bool IsTextFittingInBuffer(const char* text, size_t pixelBufferSize) { return std::strlen(text) * 6 < pixelBufferSize; };
	static const char* ReadEncryptedText(byte* pixelBuffer, size_t pixelBufferSize);
	static bool CheckEncryptionSignature(byte* pixelBuffer);

	// Not fully implemented, these are supposed to receive an EncodingSchema to encrypt the text 
	// in the pixel buffer.
	static void EncryptCharInPixels(char c, byte* pixelBuffer, EncodingSchema* encodingSchema);
	static void EncryptTextInPixelBuffer(const char* text, byte* pixelBuffer, size_t bufferSize);
	static const char* ReadTextFromPixelBuffer(byte* pixelBuffer);
};