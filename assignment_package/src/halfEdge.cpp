#include"halfEdge.h"

HalfEdge::HalfEdge():
    QListWidgetItem(),
    nextHE(nullptr), sym(nullptr), face(nullptr),
    vertex(nullptr), halfEdgeId(0), name("he")
{
    halfEdgeId = he_count;
    if(halfEdgeId == 283)
    {
        halfEdgeId++;
        halfEdgeId--;
    }
    name.append(QString::number(halfEdgeId));
    setText(name);
    he_count++;
}


HalfEdge::~HalfEdge()
{
}

void HalfEdge::setFace(Face* f)
{
    this->face = f;
    f->halfEdge = this;
}

void HalfEdge::clearCount()
{
    he_count = 0;
}
