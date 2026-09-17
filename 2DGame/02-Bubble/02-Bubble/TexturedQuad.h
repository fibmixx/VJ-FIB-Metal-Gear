#ifndef _TEXTUREDQUAD_INCLUDE
#define _TEXTUREDQUAD_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include <GL/glew.h> 

class TexturedQuad
{
public:
    // Textured quads can only be created inside an OpenGL context
    static TexturedQuad* createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram& program);

    TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram& program);
    ~TexturedQuad();

    void render(const Texture& tex) const;
    void free();

    void setPosition(const glm::vec2& pos);

private:
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    glm::vec2 position;
};

#endif // _TEXTUREDQUAD_INCLUDE