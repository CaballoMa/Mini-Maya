#include "face.h"

Face::Face():
    QListWidgetItem(), halfEdge(nullptr), color(glm::vec3()), faceId(0), name("f")
{
    faceId = f_count;
    name.append(QString::number(faceId));
    setText(name);
    f_count++;
}

Face::~Face()
{
    f_count = 0;
}

void Face::setRdmColor()
{
    this->color = glm::vec3(rand() / (float)RAND_MAX,
                            rand() / (float)RAND_MAX,
                            rand() / (float)RAND_MAX);
}

void Face::clearCount()
{
    f_count = 0;
}
