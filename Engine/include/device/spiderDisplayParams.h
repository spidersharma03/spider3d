#ifndef _SPIDERDISPLAYPARAMS_H

#define _SPIDERDISPLAYPARAMS_H

#include "spiderDataTypes.h"
#include "Renderer.h"

namespace spider3d
{

struct DisplayParams
{
	DisplayParams():rendererType(spider3d::OPENGL),WindowWidth(800),WindowHeight(600),Bits(16),ZBufferBits(16),
		Fullscreen(false),Stencilbuffer(false),WindowID(0) 
	{
	};
    
	DisplayParams(const DisplayParams& inParams)
	{
        rendererType = inParams.rendererType;
		WindowWidth = inParams.WindowWidth;
		WindowHeight = inParams.WindowHeight;
		Bits = inParams.Bits;
		ZBufferBits = inParams.ZBufferBits;
		Fullscreen = inParams.Fullscreen;
		Stencilbuffer = inParams.Stencilbuffer;
		WindowID = inParams.WindowID;
	};


	spider3d::RENDERER_TYPE rendererType;

		sf32 WindowWidth;
		sf32 WindowHeight;
		su8 Bits;
		su8 ZBufferBits;
		sbool Fullscreen;
		sbool Stencilbuffer;
		void* WindowID;

};

}

#endif