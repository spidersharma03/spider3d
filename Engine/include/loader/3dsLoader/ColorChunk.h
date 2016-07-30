#ifndef _COLOR_CHUNK_H

#define _COLOR_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"

/*******************************************************************************
* Loads colors from binary data representing them.
******************************************************************************/
class ColorChunk : public Chunk {

	////////////////////////////////////////////////////////////////////////////
	// Member Variables ////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	/***************************************************************************
	* Color type
	**************************************************************************/


private:
	

	const static int FLOAT_COLOR        = 0x10;
	const static int BYTE_COLOR         = 0x11;
	const static int BYTE_COLOR_GAMMA   = 0x12;
	const static int FLOAT_COLOR_GAMMA  = 0x13;



	/***************************************************************************
	* Based on the color type retrieved
	* from {@link #getColorType} loads
	* an rgb or float color and pushes
	* it onto the chunk chopper.
	*
	* @param chopper the chopper that will store the color data.  
	**************************************************************************/
public:

	void loadData(ChunkChopper* chopper) 
	{
		Vector3* color = new Vector3();

		int colorType = getColorType(chopper);
		if (colorType == BYTE_COLOR) 
		{
			float r = chopper->getUnsignedByte() / 255.0f;
			float g = chopper->getUnsignedByte() / 255.0f;
			float b = chopper->getUnsignedByte() / 255.0f;
			color->set(r , g , b);
			chopper->pushData(chopper->getID(), color);
		} 
		else if (colorType == FLOAT_COLOR) 
		{
			float r = chopper->getFloat() ;
			float g = chopper->getFloat() ;
			float b = chopper->getFloat() ;
			color->set(r , g , b);
			chopper->pushData(chopper->getID(), color);
		}
		
	}



protected:

	int   currentColorType;

	/***************************************************************************
	* Gets the color type for this chunk.
	* @param chopper with the information the
	* chunk may use to determine color type
	* @return the color type for the chunk retrieved
	* from the chopper using this chunks id.
	**************************************************************************/
	virtual int getColorType(ChunkChopper* chopper) {
		return chopper->getID();
	}

};

#endif