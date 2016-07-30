#ifndef _APPEARANCE_H

#define _APPEARANCE_H

#include "Material.h"
#include "PolygonAttributes.h"
#include "TransparencyAttributes.h"
#include "PointAttributes.h"
#include "LineAttributes.h"
#include "ColoringAttributes.h"
#include "DepthAttributes.h"
#include "StencilAttributes.h"
#include "TexCoordGeneration.h"
#include "TextureAttributes.h"
#include "TextureUnitState.h"
#include "RenderingAttributes.h"
#include "Texture.h"
#include "TexturePointSprite.h"
#include <vector>

namespace spider3d
{
	namespace render
	{
		class OglRenderer;
	}
}

using namespace spider3d::render;

using namespace std;



namespace spider3d
{

	namespace appearance
	{

		class Appearance
		{
		public:

			inline void setMaterial(Material& material)
			{
				this->material = material;
			}

			inline void setPolygonAttributes(PolygonAttributes& patt)
			{
				this->patt = patt;
			}

			inline void setTransparencyAttributes(TransparencyAttributes& transatt)
			{
				this->transatt = transatt;
			}

			inline void setColoringAttributes(ColoringAttributes& colatt)
			{
				this->colatt = colatt;
			}

			inline void setPointAttributes(PointAttributes& pointatt)
			{
				this->pointatt = pointatt;
			}

			inline void setLineAttributes(LineAttributes& latt)
			{
				this->latt = latt;
			}

			inline void setDepthAttributes(DepthAttributes& depthAtt )
			{
				this->depthAtt = depthAtt;
			}

			/*void setTextureUnitState(TextureUnitState* texUnitState)
			{
				this->texUnitState = texUnitState;
			}*/

			void setTextureUnitState(unsigned index , TextureUnitState* texUnitState)
			{
				if ( index < MAX_TEXTURE_UNITS - 1)
				{
				  vecTextureUnitState[index] = texUnitState;
				  numTexUnits++;
				}
			}

			inline Material& getMaterial()
			{
				return material;
			}

			inline PolygonAttributes& getPolygonAttributes()
			{
				return patt;
			}

			inline TransparencyAttributes& getTransparencyAttributes()
			{
				return transatt;
			}

			inline ColoringAttributes& getColoringAttributes()
			{
				return colatt;
			}

			inline PointAttributes& getPointAttributes()
			{
				return pointatt;
			}

			inline LineAttributes& getLineAttributes()
			{
				return latt;
			}

			inline DepthAttributes& getDepthAttributes()
			{
				return depthAtt;
			}

			inline StencilAttributes& getStencilAttributes()
			{
				return stencilAtt;
			}

			inline RenderingAttributes& getRenderingAttributes()
			{
				return renderAtt;
			}
			
			inline unsigned getNumTextureUnits()
			{
				return numTexUnits;
			}

			inline TextureUnitState* getTextureUnitState(unsigned index)
			{
				if ( index > MAX_TEXTURE_UNITS - 1)
					return 0;
				return vecTextureUnitState[index];
			}

			inline sbool operator!= (Appearance& other)
			{
				return !( material==other.material && patt==other.patt && pointatt==other.pointatt &&
					latt==other.latt && colatt==other.colatt && transatt==other.transatt 
					&& stencilAtt == other.stencilAtt );
			}

			inline sbool operator== (Appearance& other)
			{
				return ( material==other.material && patt==other.patt && pointatt==other.pointatt &&
					latt==other.latt && colatt==other.colatt && transatt==other.transatt &&  stencilAtt == other.stencilAtt );
			}

			inline sbool isTransparent()
			{
				return transatt.transparency > 0.0;
			}

			inline sbool isShaderAppearance()
			{
				return m_bShaderAppearance;
			}

			Appearance()
			{
				m_Dirty = false;
				texUnitState = 0;
				numTexUnits = 0;
				vecTextureUnitState.resize(MAX_TEXTURE_UNITS);
				m_bShaderAppearance = false;
			}

			Appearance(const Appearance& other)
			{
			  material = other.material;
			  m_Dirty = other.m_Dirty;
			  patt = other.patt; 
			  pointatt = other.pointatt;
			  renderAtt = other.renderAtt;
			  latt = other.latt;
			  colatt = other.colatt;
			  transatt = other.transatt;
  			  depthAtt = other.depthAtt;
  			  stencilAtt = other.stencilAtt;
			  texUnitState = other.texUnitState;
			  m_bShaderAppearance = false;
			}

			Appearance& operator= (Appearance& other)
			{
			  m_Dirty = other.m_Dirty;
              material = other.material;
			  patt = other.patt; 
			  pointatt = other.pointatt;
			  renderAtt = other.renderAtt;
			  latt = other.latt;
			  colatt = other.colatt;
			  transatt = other.transatt;
			  depthAtt = other.depthAtt;
			  stencilAtt = other.stencilAtt;
			  m_bShaderAppearance= other.m_bShaderAppearance;
              return *this;
			}

			inline bool isDirty()
			{
				return m_Dirty;
			}
			
			inline void setDirty(bool bDirty)
			{
				m_Dirty = bDirty;
			}

			static unsigned MAX_TEXTURE_UNITS;
			friend class spider3d::render::OglRenderer;

		protected:
			virtual void OnShaderSet()
			{
				//glUseProgramObjectARB(0);
			}
			
			virtual void OnShaderUnSet()
			{
			}

			unsigned numTexUnits;
			bool m_Dirty;
			bool m_bShaderAppearance;
			Material material;
			PolygonAttributes patt;
			TransparencyAttributes transatt;
			RenderingAttributes renderAtt;
			PointAttributes  pointatt;
			LineAttributes   latt;
			ColoringAttributes colatt;
			DepthAttributes depthAtt;
			StencilAttributes stencilAtt;
			TextureUnitState* texUnitState;
			vector<TextureUnitState*> vecTextureUnitState;
		};

	}

}
#endif