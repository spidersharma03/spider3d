#ifndef _GLSL_SHADER_PROGRAM_H

#define _GLSL_SHADER_PROGRAM_H

#ifdef WIN32
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef _MACOSX_
#include <OpenGL/OpenGL.h>
#endif


#include "appearance/ShaderProgram.h"

namespace spider3d
{

	namespace appearance
	{
		class Shader;

		class GLSLShaderProgram:public ShaderProgram
		{
		public:
			GLSLShaderProgram()
			{
				shaderPrgram = glCreateProgramObjectARB();
			}

			void setVertexShader(Shader* vertexShader)
			{
				m_pVertexShader = vertexShader;
				if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
				{
					// 1. Create ShaderObject
					vs = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

					const char* shaderSource = m_pVertexShader->getShaderSource();
					// 2. Attach Shader Code
					glShaderSourceARB(vs, 1, &shaderSource ,NULL);

					// 3. Compile Shader
					glCompileShaderARB(vs);

					// 4. Attach Shader Object to Shader Program
					glAttachObjectARB(shaderPrgram,vs);

					// 5. Link Shader Program
					glLinkProgramARB(shaderPrgram);
				}
			}

			void setFragmentShader(Shader* fragmentShader)
			{
				m_pFragmentShader = fragmentShader;
				if ( m_pFragmentShader && m_pFragmentShader->getShaderSource() )
				{
					// 1. Create ShaderObject
					fs = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

					const char* shaderSource = m_pFragmentShader->getShaderSource();
					// 2. Attach Shader Code
					glShaderSourceARB(fs, 1, &shaderSource ,NULL);

					// 3. Compile Shader
					glCompileShaderARB(fs);

					// 4. Attach Shader Object to Shader Program
					glAttachObjectARB(shaderPrgram,fs);

					// 5. Link Shader Program
					glLinkProgramARB(shaderPrgram);
				}
			}

			void addShaderUniform(ShaderUniform* shaderUniform) 
			{
				GLint location = -1;
				static int i = -1;
				i++;
				if ( shaderUniform )
				{
					glUseProgramObjectARB(shaderPrgram);
					location = glGetUniformLocationARB(shaderPrgram, shaderUniform->m_Name);
					glUseProgramObjectARB(0);
					if ( location > -1 )
					{
						m_vecShaderUniforms.push_back(shaderUniform);
						shaderUniform->m_Location = location;
					}
				}
			}

			void addShaderAttributeName(const char *name )
			{
				GLint location = -1;
				if ( name )
				{
					glUseProgramObjectARB(shaderPrgram);
					location = glGetAttribLocationARB(shaderPrgram, name);
					if( location > -1 )
					{
						m_vecShaderAttributeLocation.push_back(location);
					}
					glUseProgramObjectARB(0);
				}
			}

			void OnSet()
			{
				// Set Program
				glUseProgramObjectARB(shaderPrgram);
				// Set Uniform variables for this Shader
				for ( unsigned i=0; i<m_vecShaderUniforms.size(); i++)
				{
					ShaderUniform *shaderUniform = m_vecShaderUniforms[i];
					GLint location = shaderUniform->m_Location;
					sf32* value    = shaderUniform->m_Value;
					switch( shaderUniform->m_VarType )
					{
					case ESVT_SINGLE:
						{
						int val = *shaderUniform->m_Value;
						glUniform1iARB(location,val);
						}
						break;
					case ESVT_VEC2:
						glUniform2fvARB(location, 1, value);
						break;
					case ESVT_VEC3:
						glUniform3fvARB(location, 1, value);
						break;
					case ESVT_VEC4:
						glUniform4fvARB(location, 1, value);
						break;
					case ESVT_MAT2:
						glUniformMatrix2fvARB(location, 1, false, value); 
						break;
					case ESVT_MAT3:
						glUniformMatrix3fvARB(location, 1, false, value);
						break;
					case ESVT_MAT4:
						glUniformMatrix4fvARB(location, 1, false, value);
						break;
					default:
						printf("Variable Type not Supported\n");
						break;
					}
				}
			}

		private:
			// Handle to Vertex and Fragment Shaders
			GLhandleARB vs, fs;
			// Handle to Shader Program
			GLhandleARB shaderPrgram;
		};
	}
}

#endif