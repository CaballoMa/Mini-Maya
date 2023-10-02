#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <vertex.h>
#include <face.h>
#include <la.h>
#include <QListWidget>

class Vertex;
class Face;
static int he_count;

class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge* nextHE;
    HalfEdge* sym;
    Face* face;
    Vertex* vertex;
    int halfEdgeId;
    QString name;

    HalfEdge();
    ~HalfEdge();

    void setFace(Face* f);
    void clearCount();
};

#endif // HALFEDGE_H
