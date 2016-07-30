#ifndef _SHADER_APPEARANCE_H

#define _SHADER_APPEARANCE_H

#include "Appearance.h"
#include "ShaderProgram.h"


namespace spider3d
{
	namespace appearance
	{
		
		class ShaderAppearance:public Appearance
		{
		public:

			ShaderAppearance()
			{
				m_pShaderProgram = 0;
				m_bShaderAppearance = true;
			}

			void setShaderProgram(ShaderProgram* shaderProgram)
			{
				m_pShaderProgram = shaderProgram;
			}

			ShaderProgram* getShaderProgram()
			{
				return m_pShaderProgram;
			}
			
			friend class ShaderProgram;

		private:

			void OnShaderSet()
			{
				if(m_pShaderProgram)
					m_pShaderProgram->OnSet();
			}
			
			void OnShaderUnSet()
			{
				//m_pShaderProgram->OnSet();
			}
			
			ShaderProgram *m_pShaderProgram;
		};

	}
}

#endif
