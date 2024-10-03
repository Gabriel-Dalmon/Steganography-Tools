#include "pch.h"
#include "Bitmap.h"
#include "File.h"

Bitmap::Bitmap() {
	m_buffer = nullptr;
	memset(&m_bfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	m_colorBits = nullptr;
}

bool Bitmap::Init(const char* path) {
	File file;
	file.Open(path, "rb");
	long int size = file.GetSize();
	m_buffer = new byte[size];
	
	file.Read(m_buffer);
	memcpy(&m_bfh, m_buffer, sizeof(BITMAPFILEHEADER));
	memcpy(&m_bih, m_buffer+ sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));	
	m_colorBits = m_buffer + m_bfh.bfOffBits;
	file.Close();
	return true;
}
void Bitmap::uninit() {
	m_buffer = nullptr;
	memset(&m_bfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	m_colorBits = nullptr;
}


HBITMAP Bitmap::GenerateHBitMap(HDC hdc) {
	HBITMAP bmpTest = CreateDIBitmap(hdc, &m_bih, CBM_INIT, m_colorBits, (BITMAPINFO*)&m_bih, DIB_RGB_COLORS);
	return bmpTest;
}

BITMAPINFOHEADER Bitmap::GetMapInfo() {
	return m_bih;
}
BITMAPFILEHEADER Bitmap::GetFileInfo() {
	return m_bfh;
}

byte* Bitmap::GetBuffer() {
	return m_buffer;
}

void Bitmap::SetBits(byte value, byte numOfBits, byte* place) {
	byte mask = numOfBits * 2 - 1;
	byte invertedMask = ~mask;
	*place &= invertedMask;
	value &= mask;
	*place |= value;
}

byte Bitmap::ReadBits(byte numofBits, byte* place) {
	byte mask = numofBits * 2 - 1;
	return *place & mask;
}

void Bitmap::SetBytes(unsigned int value, byte size, byte* place) {
	int offset = 0;
	for (int i = 0, j = 0; i < size * 6 ; i++) {
		byte tempStorage = 0b00;
		if (j==2) {
			tempStorage = ((byte)(value >> (i + offset)));
			SetBits(tempStorage, 2, place);
			offset++;
			j = 0;
		}
		else {
			tempStorage = ((byte)(value >> (i + offset)));
			SetBits(tempStorage, 1, place);
			j++;
		}
		place++;
	}
}

unsigned int Bitmap::ReadBytes(byte size, byte* place) {
	unsigned int returnValue = 0b00;
	int offset = 0;
	for (int i = 0, j = 0; i < size * 6; i++) {
		byte getBits = 0b00;
		unsigned int tempStorage = 0b00;
		
		if (j == 2) {
			getBits = ReadBits(2, place);
			tempStorage = (unsigned int)getBits << (i + offset);
			offset++;
			j = 0;
			
		}
		else {
			getBits = ReadBits(1, place);
			tempStorage = (unsigned int)getBits << (i + offset);
			j++;
		}
		returnValue |= tempStorage;
		place++;
	}
	return returnValue;
}

void Bitmap::SetSignEncrypted() {
	byte* place = m_colorBits;
	unsigned int sign = ENCRYPTSIGN;
	SetBytes(sign, 2, place);
}
void Bitmap::setTextHeader(unsigned int lengthOfText) {
	byte* place = m_colorBits+12;
	SetBytes(lengthOfText, 2, place);
	
}

bool Bitmap::EncryptText(const char* text) {
	int size = std::strlen(text);
	int bits = m_bfh.bfSize - m_bfh.bfOffBits - 24;
	if (size < bits) {
		setTextHeader(size);
		byte* place = m_colorBits + 24;
		for (int i = 0; i < size; i++) {
			SetBytes((unsigned int)text[i], 2, place);
			place += 6;
		}
		SetSignEncrypted();
		return true;
	}
	else {
		return false;
	}
}

bool Bitmap::CheckSignEncrypted() {
	byte* place = m_colorBits;
	unsigned int sign = ENCRYPTSIGN;
	unsigned int sign2 = (unsigned int)ReadBytes(2, place);
	if (sign2 == sign) {
		return true;
	}
	return false;
}

unsigned int Bitmap::ReadTextHeader() {
	byte* place = m_colorBits + 12;
	return ReadBytes(2, place);
}

const char* Bitmap::ReadEncryptedText(int textLength) {
	char* returnArray = new char[textLength+1];
	byte* place = m_colorBits + 24;
	for (int i = 0; i < textLength; i++) {
		returnArray[i] = (char)ReadBytes(1, place);
		place += 6;
	}
	returnArray[textLength] = '\0';
	return returnArray;
}

