#include "halfedgedisplay.h"

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext* context)
    : Drawable(context), representedHalfEdge(nullptr)
{}

HalfEdgeDisplay::~HalfEdgeDisplay()
{}

GLenum HalfEdgeDisplay::drawMode()
{
    return GL_LINES;
}

void HalfEdgeDisplay::updateHalfEdge(HalfEdge * he)
{
    representedHalfEdge = he;
}

void HalfEdgeDisplay::create()
{
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> nor;
    std::vector<glm::vec4> col;
    std::vector<GLuint> indices;


    if (representedHalfEdge)
    {
        glm::vec4 vertexPos = glm::vec4(representedHalfEdge->vertex->pos, 1);
        glm::vec4 symVertexPos = glm::vec4(representedHalfEdge->sym->vertex->pos, 1);

        pos.push_back(vertexPos);
        pos.push_back(symVertexPos);

        nor.push_back(glm::vec4(0, 0, 1, 1));
        nor.push_back(glm::vec4(0, 0, 1, 1));

        col.push_back(glm::vec4(1, 0, 0, 1));
        col.push_back(glm::vec4(1, 1, 0, 1));

        indices.push_back(0);
        indices.push_back(1);
    }

    count = indices.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generateNor();
    bindNor();
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
}
