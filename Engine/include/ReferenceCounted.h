#ifndef _REFERENCECOUNTED_H

#define _REFERENCECOUNTED_H


class ReferenceCounted
{

public:

	inline void grab() const
	{
		referenceCount++;
	}

	inline void drop() const
	{
		referenceCount--;

		if ( !referenceCount )
		{
			delete this;
		}
	};

	ReferenceCounted()
	{
		referenceCount = 0;
	}

	inline unsigned int getReferenceCount()
	{
		return referenceCount;
	}

private:

	unsigned int referenceCount;
};

#endif