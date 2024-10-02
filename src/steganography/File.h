#include "pch.h"
#include <stdio.h>
class File {
	const char* m_path;
	long int m_size;
	FILE* m_pFile;
public:
	File();

	bool Open(const char* path, const char* openMode);

	long int GetSize();

	void Read(byte* buffer);

	void Write(byte* buffer, int size);

	void Close();

	~File() {};
};