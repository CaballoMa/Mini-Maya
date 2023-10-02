#ifndef FACE_H
#define FACE_H

#include"la.h"
#include <vertex.h>
#include <halfedge.h>
#include <QListWidget>

class HalfEdge;
static int f_count;
class Face : public QListWidgetItem
{
public:
    HalfEdge* halfEdge;
    glm::vec3 color;
    int faceId;
    QString name;

    Face();
    ~Face();
    void clearCount();
    void setRdmColor();
};

#endif // FACE_H
