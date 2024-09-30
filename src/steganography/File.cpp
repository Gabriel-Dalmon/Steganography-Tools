#include "pch.h"
#include "File.h"
#include <stdio.h>


File::File() {
	m_size = 0;
	m_path = "";
	m_pFile = nullptr;
}

bool File::Open(const char* path) {
	m_path = path;
	m_pFile = fopen(path, "rb+");
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
	fseek(m_pFile, 0, SEEK_SET);
	fread(buffer, sizeof(char), m_size, m_pFile);
}

void File::Write(byte* buffer) {
	fwrite(buffer, sizeof(char), m_size, m_pFile);
}

void File::Close() {
	fclose(m_pFile);
	m_pFile = nullptr;
}