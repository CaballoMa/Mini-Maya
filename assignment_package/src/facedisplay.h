#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H

#include <face.h>
#include <drawable.h>

class FaceDisplay: public Drawable
{
protected:
    Face *representedFace;

public:
    void create() override;
    void updateFace(Face*);
    GLenum drawMode() override;

    FaceDisplay(OpenGLContext* context);
    ~FaceDisplay();
};

#endif // FACEDISPLAY_H
