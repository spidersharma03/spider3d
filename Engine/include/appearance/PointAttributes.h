#ifndef _POINT_ATTRIBUTES_H

#define _POINT_ATTRIBUTES_H

#include "common/spiderDataTypes.h"

namespace spider3d
{

	namespace appearance
	{

		struct PointAttributes
		{
            
			PointAttributes()
			{
               size = 1.0;
			   antiAliased = false;
			   m_ThresholdSize = 1.0f;
			   m_MinimumSize = 0.0f;
			   m_MaximumSize = 1.0f;
			   m_bPointSizeAttributesEnabled = false;
			}

            PointAttributes(PointAttributes& other)
			{
			   size = other.size;
               antiAliased = other.antiAliased;
			   m_ThresholdSize = other.m_ThresholdSize;
			   m_MinimumSize = other.m_MinimumSize;
			   m_MaximumSize = other.m_MaximumSize;
			   m_bPointSizeAttributesEnabled = other.m_bPointSizeAttributesEnabled;
			}

			PointAttributes& operator= (const PointAttributes& other)
			{
				size = other.size;
				antiAliased = other.antiAliased;
	            m_ThresholdSize = other.m_ThresholdSize;
			    m_MinimumSize = other.m_ThresholdSize;
				m_MaximumSize = other.m_ThresholdSize;
				m_bPointSizeAttributesEnabled = other.m_bPointSizeAttributesEnabled;
				return *this;
			}

	
			sbool operator!= ( PointAttributes& other )
			{
   				return !( size == other.size && antiAliased == other.antiAliased );
			}

			sbool operator== ( PointAttributes& other )
			{
   				return ( size == other.size && antiAliased == other.antiAliased );
			}

			sf32 m_ThresholdSize;

			sf32 m_MinimumSize;

			sf32 m_MaximumSize;

			Point3 m_DistanceAttenuation;

			sf32 size;

			sbool antiAliased;

			sbool m_bPointSizeAttributesEnabled;
		};

	}

}
#endif