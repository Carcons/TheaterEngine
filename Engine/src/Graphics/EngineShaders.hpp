#pragma once

namespace Theater
{
	namespace ShadersSource
	{
		inline constexpr char* const BASIC_MESH_VSHADER = R"(
			#version 410 core

			uniform mat4 _uEngineProjMat;
			uniform mat4 _uEngineViewMat;
			uniform mat4 _uEngineModelMat;

			uniform float _uShininess;

			layout(location = 0) in vec3 _EngineVertexPosition;
			layout(location = 1) in vec2 _EngineVertexTexCoords;

			out vec2 TexCoords; 

			void main()
			{
				TexCoords = _EngineVertexTexCoords;

				gl_Position = _uEngineProjMat * _uEngineViewMat * _uEngineModelMat * vec4(_EngineVertexPosition, 1.0);
			}
		)";

		inline constexpr char* const BASIC_MESH_FSHADER = R"(
			#version 410 core

			#define MAX_TEXTURES 4

			uniform float _uShininess;

			// Texture samplers
			uniform sampler2D _uDiffuseTextures[MAX_TEXTURES];
			uniform sampler2D _uSpecularTextures[MAX_TEXTURES];

			// Active texture counts
			uniform int _uNumDiffuseTextures;
			uniform int _uNumSpecularTextures;

			in vec2 TexCoords;
			out vec4 _EngineOutColor;

			void main()
			{
				vec3 color = vec3(1.0, 1.0, 1.0);

				if (_uNumDiffuseTextures > 0) 
				{
					color = texture(_uDiffuseTextures[0], TexCoords).rgb;
				}

				if (_uNumSpecularTextures > 0)
				{
					color += texture(_uSpecularTextures[0], TexCoords).rgb * 0.5;
				}
				
				_EngineOutColor = vec4(color, 1.0);
			}
		)";

		inline constexpr char* const TEXTUREDQUAD_VSHADER = R"(
	
			#version 410 core
			layout (location = 0) in vec2 Pos;
			layout (location = 1) in vec2 TexCoords;

			out vec2 outTexCoords;

			void main()
			{
				gl_Position = vec4(Pos.x, Pos.y, 0.0, 1.0); 
				outTexCoords = TexCoords;
			}  
		
		)";

		inline constexpr char* const TEXTUREDQUAD_FSHADER = R"(
			
			#version 410 core

			out vec4 FragColor;  
			in vec2 outTexCoords;

			uniform sampler2D Texture;

			void main()
			{ 
				FragColor = texture(Texture, outTexCoords);
			}

		)";
	}
}