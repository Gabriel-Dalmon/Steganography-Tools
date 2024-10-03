#pragma once
#define ENCRYPTSIGN 38977
#define MAX16BITS 65535


class BitUtils {
public:
	static void SetBits(byte value, byte numOfBits, byte* place);

	static void SetBytes(unsigned int value, byte size, byte* place);

	static void SetSignEncrypted(byte* place);

	static byte ReadBits(byte numOfBits, byte* place);

	static unsigned int ReadBytes(byte size, byte* place);


};