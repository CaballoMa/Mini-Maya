#include "facedisplay.h"

FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr)
{}

FaceDisplay::~FaceDisplay()
{}

GLenum FaceDisplay::drawMode()
{
    return GL_LINES;
}

void FaceDisplay::updateFace(Face * f)
{
    representedFace = f;
}

void FaceDisplay::create()
{
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> nor;
    std::vector<glm::vec4> col;
    std::vector<GLuint> indices;
    int edgeNum = 0;

    if (representedFace)
    {
        HalfEdge *currHE = representedFace->halfEdge;

        do
        {
            pos.push_back(glm::vec4(currHE->vertex->pos, 1));
            nor.push_back(glm::vec4(0, 0, 1, 1));
            col.push_back(glm::vec4(glm::vec3(1)-representedFace->color, 1));
            currHE = currHE->nextHE;
            edgeNum++;
        }
        while(currHE != representedFace->halfEdge);

        for (int i = 0; i < edgeNum; i++)
        {
            indices.push_back(i);
            indices.push_back((i + 1) % edgeNum);
        }
    }

    count = indices.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    bindNor();
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}
