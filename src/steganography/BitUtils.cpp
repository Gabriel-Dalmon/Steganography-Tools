#include "pch.h"
#include "BitUtils.h"

void BitUtils::SetBits(byte value, byte numOfBits, byte* place) {
	byte mask = numOfBits * 2 - 1;
	byte invertedMask = ~mask;
	*place &= invertedMask;
	value &= mask;
	*place |= value;
}

byte BitUtils::ReadBits(byte numofBits, byte* place) {
	byte mask = numofBits * 2 - 1;
	return *place & mask;
}


void BitUtils::SetBytes(unsigned int value, byte size, byte* place) {
	int offset = 0;
	for (int i = 0, j = 0; i < size * 6; i++) {
		byte tempStorage = 0b00;
		if (j == 2) {
			tempStorage = ((byte)(value >> (i + offset)));
			BitUtils::SetBits(tempStorage, 2, place);
			offset++;
			j = 0;
		}
		else {
			tempStorage = ((byte)(value >> (i + offset)));
			BitUtils::SetBits(tempStorage, 1, place);
			j++;
		}
		place++;
	}
}

unsigned int BitUtils::ReadBytes(byte size, byte* place) {
	unsigned int returnValue = 0b00;
	int offset = 0;
	for (int i = 0, j = 0; i < size * 6; i++) {
		byte getBits = 0b00;
		unsigned int tempStorage = 0b00;

		if (j == 2) {
			getBits = BitUtils::ReadBits(2, place);
			tempStorage = (unsigned int)getBits << (i + offset);
			offset++;
			j = 0;

		}
		else {
			getBits = BitUtils::ReadBits(1, place);
			tempStorage = (unsigned int)getBits << (i + offset);
			j++;
		}
		returnValue |= tempStorage;
		place++;
	}
	return returnValue;
}

void BitUtils::SetSignEncrypted(byte* place) {
	unsigned int sign = ENCRYPTSIGN;
	BitUtils::SetBytes(sign, 2, place);
}
