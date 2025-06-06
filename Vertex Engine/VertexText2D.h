#pragma once
#include <map>

#include "Graphics.h"

#include "Texture2D.h"
#include "shader.h"
#include "Camera.h"
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
    void Text2D(std::string text, float x, float y, glm::vec2 scale, glm::vec4 color = glm::vec4(1.0f), glm::mat4 _camera = glm::mat4(1.0f));
private:
    unsigned int VAO, VBO;
};

