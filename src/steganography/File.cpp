#include "pch.h"
#include "File.h"
#include <stdio.h>


File::File() {
	m_size = 0;
	m_path = "";
	m_pFile = nullptr;
}

bool File::Open(const char* path, const char* openMode) {
	m_path = path;
	fopen_s(&m_pFile, path, openMode);
	if (m_pFile == nullptr) {
		return false;
	}
	fseek(m_pFile, 0, SEEK_END);
	m_size = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	return true;
}

long int File::GetSize() {
	return m_size;
}

void File::Read(byte* buffer) {
	if (m_pFile != nullptr) {
		fseek(m_pFile, 0, SEEK_SET);
		fread(buffer, sizeof(char), m_size, m_pFile);
	}
}

void File::Write(byte* buffer, int size) {
	m_size = size;
	if (m_pFile != nullptr) {
		fwrite(buffer, sizeof(char), m_size, m_pFile);
	}
}

void File::Close() {
	if (m_pFile != nullptr) {
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}