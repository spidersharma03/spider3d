#include "loader/3dsloader/ChunkBuffer.h"

bool ChunkBuffer::hasRemaining()
{
	return size > currPosition ? true : false;
}

int  ChunkBuffer::position()
{
	return currPosition;
}

void ChunkBuffer::position(int position)
{
	currPosition = position;
}

int ChunkBuffer::limit()
{
  return size;
}

void ChunkBuffer::limit(int newLimit)
{
  size = newLimit;
  if ( currPosition > newLimit )
	  currPosition = newLimit;
}

char* ChunkBuffer::getData()
{
	return buffer;
}

void ChunkBuffer::get(char* buf , int size)
{
	for ( int i = 0 ; i < size ; i ++ )
		buf[i] = getChar();
}


char ChunkBuffer::getChar()
{
	char val = buffer[currPosition];
    currPosition++; 
	return val;
	
}

short ChunkBuffer::getShort()
{
	unsigned char d[2];
	d[0] = buffer[currPosition];
	d[1] = buffer[currPosition + 1];

	int val = ( (unsigned)d[1] << 8 ) |  d[0] ;
	currPosition += 2;

	return val;
}


int ChunkBuffer::getInt()
{
	unsigned char d[4];
	d[0] = buffer[currPosition];
	d[1] = buffer[currPosition + 1];
	d[2] = buffer[currPosition + 2];
	d[3] = buffer[currPosition + 3];

	int  val = 0;
	val = ( ( (unsigned)d[3] << 24 ) |  ( (unsigned)d[2] << 16 ) | ( (unsigned)d[1] << 8 ) |  d[0]) ;

	currPosition += 4;
    
	return val;
}


float ChunkBuffer::getFloat()
{
	unsigned char d[4];
	d[0] = buffer[currPosition ];
	d[1] = buffer[currPosition + 1];
	d[2] = buffer[currPosition + 2];
	d[3] = buffer[currPosition + 3];

	Float f;
	f.dword_value = ( ( (unsigned)d[3] << 24 ) |  ( (unsigned)d[2] << 16 ) | ( (unsigned)d[1] << 8 ) |  d[0] ) ;
	float val = f.float_value;
	currPosition += 4;
	return val;
}

long ChunkBuffer::getLong()
{
	unsigned char d[8];
	d[0] = buffer[currPosition ];
	d[1] = buffer[currPosition + 1];
	d[2] = buffer[currPosition + 2];
	d[3] = buffer[currPosition + 3];
	d[4] = buffer[currPosition + 4];
	d[5] = buffer[currPosition + 5];
	d[6] = buffer[currPosition + 6];
	d[7] = buffer[currPosition + 7];

	long val = ( (unsigned)d[7] << 56 ) | ( (unsigned)d[6] << 48 ) |  ( (unsigned)d[5] << 40 ) | ( (unsigned)d[4] << 32 ) 
		| ( (unsigned)d[3] << 24 ) |  ( (unsigned)d[2] << 16 ) | ( (unsigned)d[1] << 8 ) |  d[0] ;
	currPosition += 8;
	return val;
}


double ChunkBuffer::getDouble()
{
	unsigned char d[8];
	d[0] = buffer[currPosition ];
	d[1] = buffer[currPosition + 1];
	d[2] = buffer[currPosition + 2];
	d[3] = buffer[currPosition + 3];
	d[4] = buffer[currPosition + 4];
	d[5] = buffer[currPosition + 5];
	d[6] = buffer[currPosition + 6];
	d[7] = buffer[currPosition + 7];

	double val = ( (unsigned)d[7] << 56 ) | ( (unsigned)d[6] << 48 ) |  ( (unsigned)d[5] << 40 ) 
		| ( (unsigned)d[4] << 32 ) | ( (unsigned)d[3] << 24 ) |  ( (unsigned)d[2] << 16 ) | ( (unsigned)d[1] << 8 ) |  d[0] ;
	currPosition += 8;
	return val;
}


ChunkBuffer::ChunkBuffer(int size)
{
	this->size = size;
	buffer = new char[size];
	currPosition = 0;
}

