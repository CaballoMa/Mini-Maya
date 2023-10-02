#ifndef VERTEXDISPLAY_H
#define VERTEXDISPLAY_H

#include <vertex.h>
#include <drawable.h>

class VertexDisplay : public Drawable {
protected:
    Vertex *representedVertex;

public:
    void create() override;
    void updateVertex(Vertex*);
    GLenum drawMode() override;

    VertexDisplay(OpenGLContext* context);
    ~VertexDisplay();
};


#endif
