#include "vertex.h"

Vertex::Vertex()
    : QListWidgetItem(), pos(glm::vec3()), halfEdge(nullptr),
      vertexId(0), name("vt"), jWeights()
{
    vertexId = vt_count;
    name.append(QString::number(vertexId));
    setText(name);
    vt_count++;
}

Vertex::~Vertex()
{
}

void Vertex::clearCount()
{
    vt_count = 0;
}
