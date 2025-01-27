#include <glad/glad.h>
#include <Theater/Engine/Graphics/Shader.hpp>
#include <Theater/Engine/Graphics/Vertex.hpp>


namespace Theater
{
    TexturedQuad::~TexturedQuad()
    {
        if (m_VAO)
        {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }

        if (m_VBO)
        {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
    }
    void TexturedQuad::Draw(Shader& shader, uint32_t textureID)
	{
        shader.Bind();
        glBindVertexArray(m_VAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        shader.UnBind();


        glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TexturedQuad::Create()
    {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_VertexData), &m_VertexData, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0u);
    }
}