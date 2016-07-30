#ifndef _SHADER_PROGRAM_H

#define _SHADER_PROGRAM_H

#include "SourceCodeShader.h"
#include <map>

using namespace std;

namespace spider3d
{

	namespace appearance
	{
		class Shader;
		
		enum SHADER_VARIABLE_TYPE { ESVT_SINGLE, ESVT_VEC2, ESVT_VEC3, ESVT_VEC4, ESVT_MAT2, ESVT_MAT3, ESVT_MAT4 };

		struct ShaderUniform
		{
		public:
			SHADER_VARIABLE_TYPE m_VarType;
			char *m_Name;
			float *m_Value;

			friend class GLSLShaderProgram;
			friend class CGShaderProgram;
		private:
			int m_Location;
		};

		class ShaderProgram
		{
		public:

			ShaderProgram()
			{
			}

			virtual void setVertexShader(Shader* vertexShader) = 0;

			virtual void setFragmentShader(Shader* fragmentShader) = 0;

			virtual void addShaderUniform(ShaderUniform* shaderUniform) = 0;
		
			virtual void addShaderAttributeName(const char *name ) = 0;
			
			inline vector<int>& getAllAttributeLocations()
			{
				return m_vecShaderAttributeLocation;
			}

			friend class ShaderAppearance;

		protected:
			virtual void OnSet() = 0;

			Shader *m_pVertexShader;
			Shader *m_pFragmentShader;
			map<const char*, int> m_mapShaderUniformLocation;
			vector<ShaderUniform*> m_vecShaderUniforms;
			vector<int> m_vecShaderAttributeLocation;
		};
	}
}

#endif