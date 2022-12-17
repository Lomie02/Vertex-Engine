#pragma once
#include <map>

#include "Graphics.h"

#include "Texture2D.h"
#include "shader.h"

struct Character {
    unsigned int TextureID; 
    glm::ivec2   Size;      
    glm::ivec2   Bearing;   
    unsigned int Advance;   
};

class VertexText2D
{
public:
    std::map<char, Character> Characters;
    Shader TextShader;
    VertexText2D(unsigned int width, unsigned int height);
    void Load(std::string font, unsigned int fontSize);
    void Text2D(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
    unsigned int VAO, VBO;
};

