#ifndef _STENCIL_ATTRIBUTES_H

#define _STENCIL_ATTRIBUTES_H

#include "common/spiderDataTypes.h"
#include "common/e_comparisionfunction.h"

namespace spider3d
{
	enum STENCIL_OPERATION
	{
	/** When the stencil function matches, increment the stencil value. */
      ESO_STENCIL_INCREMENT = (short) 0x1E02 ,
    /** When the stencil function matches, decrement the stencil value. */
      ESO_STENCIL_DECREMENT = (short) 0x1E03 ,
    /** When the stencil function matches, keep the current stencil value. */
      ESO_STENCIL_KEEP = (short) 0x1E00 ,
    /** When the stencil function matches, bitwise invert the stencil value. */
      ESO_STENCIL_INVERT = (short)0x150A ,
    /** When the stencil function matches, set the stencil value to zero. */
      ESO_STENCIL_ZERO = (short)0 ,
    /**When the stencil function matches, set the stencil value to the reference value. */
	  ESO_STENCIL_REPLACE = (short)0x1E01 
	};

	namespace appearance
	{
		struct StencilAttributes
		{
			/** The bit-plane definition for compare operations. */
			si32 compareMask;

			/** The bit-plane definition for write operations. */
			si32 writeMask;

			/** The reference value to compare for the stencil check */
			si32 referenceValue;

			/** The action to take on a general stencil fail */
			STENCIL_OPERATION failOp;

			/** The action to take on a depth fail */
			STENCIL_OPERATION depthFailOp;

			/** The action to take on a depth pass  */
			STENCIL_OPERATION depthPassOp;

			/** The stencil function mode to use */
			COMPARISION_FUNCTION CompareFunc;

			/** Enable mask*/
			sbool StencilTest;

			StencilAttributes()
			{
				compareMask = ~0;
				writeMask = ~0;
				referenceValue = 0;
				CompareFunc = ECFN_ALWAYS;
				failOp = ESO_STENCIL_KEEP;
				depthFailOp = ESO_STENCIL_KEEP;
				depthPassOp = ESO_STENCIL_KEEP;
				StencilTest = false;
			}

			StencilAttributes(const StencilAttributes& other)
			{
				compareMask = other.compareMask;
				writeMask = other.writeMask;
				referenceValue = other.referenceValue;
				CompareFunc = other.CompareFunc;
				failOp = other.failOp;
				depthFailOp = other.depthFailOp;
				depthPassOp = other.depthPassOp;
				StencilTest = other.StencilTest;
			}

			StencilAttributes& operator=(const StencilAttributes& other)
			{
				compareMask = other.compareMask;
				writeMask = other.writeMask;
				referenceValue = other.referenceValue;
				CompareFunc = other.CompareFunc;
				failOp = other.failOp;
				depthFailOp = other.depthFailOp;
				depthPassOp = other.depthPassOp;
				StencilTest = other.StencilTest;
				return *this;
			}

			sbool operator==(const StencilAttributes& other)
			{
				return ( compareMask == other.compareMask &&
				writeMask == other.writeMask &&
				referenceValue == other.referenceValue &&
				CompareFunc == other.CompareFunc &&
				failOp == other.failOp &&
				depthFailOp == other.depthFailOp &&
				depthPassOp == other.depthPassOp &&
				StencilTest == other.StencilTest);
			}

			sbool operator!=(const StencilAttributes& other)
			{
				return ( compareMask != other.compareMask ||
				writeMask != other.writeMask ||
				referenceValue != other.referenceValue ||
				CompareFunc != other.CompareFunc ||
				failOp != other.failOp ||
				depthFailOp != other.depthFailOp ||
				depthPassOp != other.depthPassOp ||
				StencilTest != other.StencilTest);
			}

		};

	}
}
#endif