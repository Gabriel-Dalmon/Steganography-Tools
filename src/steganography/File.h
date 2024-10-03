#pragma once

class File {
	const char* m_path;
	long int m_size;
	FILE* m_pFile;
public:
	File();

	bool Open(const char* path, const char* openMode);
	bool Open(const wchar_t* path, const wchar_t* openMode);

	long int GetSize();

	void Read(byte* buffer);

	void Write(byte* buffer, int size);

	void Close();

	~File() {};
};