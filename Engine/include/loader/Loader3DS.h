#ifndef _LOADER3DS_H

#define _LOADER3DS_H

#include "LoaderBase.h"

class ChunkChopper;

class Loader3DS : public LoaderBase
{
public:

	virtual Scene* load ( char* fileName , SceneManager* ) ;


	void optimize();

private:

	ChunkChopper* chopper;
};

#endif