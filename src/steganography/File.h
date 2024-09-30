
class File {
	const char* m_path;
	long int m_size;
	FILE* m_pFile;
public:
	File();

	bool Open(const char* path);

	long int GetSize();

	void Read(byte* buffer);

	void Write(byte* buffer);

	void Close();

	~File() {};
};