#ifndef _CG_SHADER_PROGRAM_H

#define _CG_SHADER_PROGRAM_H

#include <cg\cg.h>													
#include <cg\cggl.h>												

#include "ShaderProgram.h"

namespace spider3d
{

	namespace appearance
	{
		class Shader;

		class CGShaderProgram:public ShaderProgram
		{
		public:
			CGShaderProgram()
			{
				// Setup Cg
				cgContext = cgCreateContext();										// Create A New Context For Our Cg Program(s)

				cgVertexProgram = 0;	
				cgFragmentProgram = 0;

				// Validate CG Context Generation Was Successful
				if (cgContext == NULL)
				{
					printf("Failed To Create Cg Context\n");
				}

			}

			void setVertexShader(Shader* vertexShader)
			{
				m_pVertexShader = vertexShader;
				if ( m_pVertexShader && m_pVertexShader->getShaderSource() )
				{
					cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);				// Get The Latest GL Vertex Profile

					// Validate if Profile Determination Was Successful
					if (cgVertexProfile == CG_PROFILE_UNKNOWN)
					{
						printf("Invalid Vertex profile type\n");
					}

					cgGLSetOptimalOptions(cgVertexProfile);								// Set The Current Profile

					// Load And Compile The Vertex Shader From File
					cgVertexProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, "CG/Wave.cg", cgVertexProfile, "main", 0);

					// Validate Success
					if (cgVertexProgram == NULL)
					{
						// We Need To Determine What Went Wrong
						CGerror Error = cgGetError();

						// Show A Message Box Explaining What Went Wrong
						printf(cgGetErrorString(Error));
						return;													// We Cannot Continue
					}

					// Load The Program
					cgGLLoadProgram(cgVertexProgram);
				}
			}

			void setFragmentShader(Shader* fragmentShader)
			{
				m_pFragmentShader = fragmentShader;
				if ( m_pFragmentShader && m_pFragmentShader->getShaderSource() )
				{
					cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);				// Get The Latest GL Vertex Profile

					// Validate if Profile Determination Was Successful
					if (cgFragmentProfile == CG_PROFILE_UNKNOWN)
					{
						printf("Invalid Fragment profile type\n");
					}

					cgGLSetOptimalOptions(cgFragmentProfile);								// Set The Current Profile

					// Load And Compile The Vertex Shader From File
					cgFragmentProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, "CG/Wave.cg", cgFragmentProfile, "main", 0);

					// Validate Success
					if (cgFragmentProgram == NULL)
					{
						// We Need To Determine What Went Wrong
						CGerror Error = cgGetError();

						// Show A Message Box Explaining What Went Wrong
						printf(cgGetErrorString(Error));
						return;													// We Cannot Continue
					}

					// Load The Program
					cgGLLoadProgram(cgFragmentProgram);
				}
			}

			void addShaderUniform(ShaderUniform* shaderUniform) 
			{
				GLint location = -1;
				if ( shaderUniform )
				{
					if ( cgVertexProgram )
						cgGLBindProgram(cgVertexProgram);
					if ( cgFragmentProgram )
						cgGLBindProgram(cgFragmentProgram);
					
					//shaderUniform->m_Location = (CGparameter)cgGetNamedParameter(cgVertexProgram, shaderUniform->m_Name); 
					
					//glUseProgramObjectARB(shaderPrgram);
					//location = glGetUniformLocationARB(shaderPrgram, name);
					//glUseProgramObjectARB(0);
					//m_mapShaderUniformLocation[name] = location;
				}

				/*switch( varType )
				{
				case EVT_SINGLE:
				glUniform1fvARB(location, 1, value);
				break;
				case EVT_VEC2:
				glUniform2fvARB(location, 1, value);
				break;
				case EVT_VEC3:
				glUniform3fvARB(location, 1, value);
				break;
				case EVT_VEC4:
				glUniform4fvARB(location, 1, value);
				break;
				case EVT_MAT2:
				glUniformMatrix2fvARB(location, 1, false, value); 
				break;
				case EVT_MAT3:
				glUniformMatrix3fvARB(location, 1, false, value);
				break;
				case EVT_MAT4:
				glUniformMatrix4fvARB(location, 1, false, value);
				break;
				default:
				printf("Variable Type not Supported\n");
				break;
				}*/
			}

			void setShaderAttributeName(const char *name )
			{
				/*GLint location = -1;
				if ( name )
				{
				location = glGetAttribLocationARB(shaderPrgram, name);
				}*/
			}

			void OnSet()
			{
				cgGLEnableProfile(cgVertexProfile);								
				cgGLEnableProfile(cgFragmentProfile);
				// Bind Our Vertex Program To The Current State
				if ( cgVertexProgram )
					cgGLBindProgram(cgVertexProgram);
				if ( cgFragmentProgram )
					cgGLBindProgram(cgFragmentProgram);
			}

		private:
			// Handle to Vertex and Fragment Shaders
			//GLhandleARB vs, fs;
			// Handle to Shader Program
			CGcontext	cgContext;	// A Context To Hold Our Cg Program(s)
			CGprogram	cgVertexProgram;	// Cg Vertex Program
			CGprogram	cgFragmentProgram;	// Cg Fragment Program
			CGprofile	cgVertexProfile;	// The Profile To Use For Our Vertex Shader
			CGprofile	cgFragmentProfile;    // The Profile To Use For Our Fragment Shader
		};
	}
}

#endif