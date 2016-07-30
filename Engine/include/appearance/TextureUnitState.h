#ifndef _TEX_UNIT_STATE_H
#define _TEX_UNIT_STATE_H

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#endif
//#include <GL/glext.h>
#include "Texture.h"
#include "TexCoordGeneration.h"
#include "TextureAttributes.h"

namespace spider3d
{
	namespace appearance
	{
		class TextureUnitState
		{
		private:
			Texture* texture;
			TexCoordGeneration* texCoordGen;
			TextureAttributes* textureAtt;
		public:
			TextureUnitState(Texture*);
			TextureUnitState(Texture*, TexCoordGeneration*); 
			TextureUnitState(Texture*, TextureAttributes*); 
			TextureUnitState(Texture*, TexCoordGeneration*, TextureAttributes*); 
			TextureAttributes* getTextureAttributes();
			Texture* getTexture();
			TexCoordGeneration* getTexCoordGeneration();
			virtual ~TextureUnitState();
		};
	}
}
#endif