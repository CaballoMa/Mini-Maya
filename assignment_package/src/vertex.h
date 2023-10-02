#ifndef VERTEX_H
#define VERTEX_H

#include <halfEdge.h>
#include <face.h>
#include <QListWidget>

class HalfEdge;
static int vt_count;

class Vertex : public QListWidgetItem
{
public:
    glm::vec3 pos;
    HalfEdge* halfEdge;

    int vertexId;
    QString name;
    std::vector<std::pair<int, float>> jWeights;

    Vertex();
    ~Vertex();
    void clearCount();
};

#endif // VERTEX_H
