#ifndef _LOADER_BASE_H

#define _LOADER_BASE_H

#include "SceneManager.h"

using namespace spider3d;

class Scene;

class LoaderBase
{

public:
	virtual Scene* load ( char* fileName , SceneManager* ) = 0;
};

#endif