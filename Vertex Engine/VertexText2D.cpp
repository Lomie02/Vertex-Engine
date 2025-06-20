#include "VertexText2D.h"
#include "ResourceManager.h"

#include <iostream>
#include "gtc/matrix_transform.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

VertexText2D::VertexText2D(unsigned int width, unsigned int height)
{
	this->TextShader = ResourceManager::LoadShader("Builds/Shaders/text2d.vs", "Builds/Shaders/text2d.frag", nullptr, "VertexText");
    this->TextShader.SetInteger("text", 0);
	
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VertexText2D::Load(std::string font, unsigned int fontSize)
{
    this->Characters.clear();
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        std::cout << "VERTEX ERROR: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cout << "VERTEX ERROR: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0.5, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) 
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "VERTEX ERROR: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void VertexText2D::Text2D(std::string text, float x, float y, glm::vec2 scale, glm::vec4 color, glm::mat4 _camera)
{
    // activate corresponding render state	
    this->TextShader.Use();
    this->TextShader.SetMatrix4("projection", _camera, true);
    this->TextShader.SetVector4f("textColor", color);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale.x;
        float ypos = y - (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale.y;

        float w = ch.Size.x * scale.x;
        float h = ch.Size.y * scale.y;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos - h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos - h,   0.0f, 1.0f },
            { xpos + w, ypos - h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
