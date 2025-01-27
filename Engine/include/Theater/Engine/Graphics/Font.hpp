#pragma once

#include "IRenderable.hpp"
#include "Texture.hpp"

namespace Theater
{
	class Text : public IRenderable
	{
	public:

		Text();
		~Text();

		Text(const Text&) = delete;
		Text& operator = (const Text&) = delete;
	/*
		Text(Text&& other) noexcept;
		Text& operator = (const Text& other) noexcept;*/

	public:

		//void Draw() override;

	private:
		

	private:
		Texture2D m_TextureAtlas;
		void* m_pFontFace;
	};
}