#pragma once

namespace Theater
{
	class Shader;

	class IRenderable 
	{
	public:

		virtual ~IRenderable() {};

	public:

		virtual void Draw(Shader&) = 0;
	};
}