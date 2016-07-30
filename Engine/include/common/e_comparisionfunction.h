#ifndef _E_COMPARISION_FUNCTION_H

#define _E_COMPARISION_FUNCTION_H

namespace spider3d
{

enum COMPARISION_FUNCTION 
   {
	ECFN_NEVER = 0x0200 ,
    /** Comparison function that passes if current depth < range. */
    ECFN_LESS = 0x0201 ,
    /** Comparison function that passes if current depth <= range. */
    ECFN_LESS_OR_EQUAL = 0x0203 ,
    /** Comparison function that passes if current depth > range. */
    ECFN_GREATER = 0x0204,
    /** Comparison function that passes if current depth >= range. */
    ECFN_GREATER_OR_EQUAL = 0x0206,
    /** Comparison function that passes if current depth == range. */
    ECFN_EQUAL = 0x0202 ,
    /** Comparison function that passes if current depth is not equal range. */
    ECFN_NOTEQUAL = 0x0205 ,
    /** Comparison function that always passes. */
	ECFN_ALWAYS = 0x0207 
    };

}

#endif