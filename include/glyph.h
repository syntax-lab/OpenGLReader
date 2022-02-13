#pragma once

struct glyph{
    GLuint       texture_id; // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Horizontal offset to advance to next glyph
};
