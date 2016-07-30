#ifndef _CHUNK_BUFFER_H

#define _CHUNK_BUFFER_H

typedef union { 
  unsigned dword_value; 
  float float_value;
} Float;

class ChunkBuffer
{
public:

	bool hasRemaining();

	int  position();

	void position(int);

	int limit();

	void limit(int);

	char* getData();

	void get(char* , int);

	char getChar();

	short getShort();

	int getInt();

	long getLong();

	float getFloat();

	double getDouble();

	ChunkBuffer(int size);

private:

	int currPosition;

	char* buffer;

	int size;
};

#endif