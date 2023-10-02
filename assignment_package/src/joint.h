#ifndef JOINT_H
#define JOINT_H

#include <QTreeWidget>
#include <la.h>
static int jointIdCount;
class Joint : public QTreeWidgetItem
{
public:
    int jointId;
    QString name;
    Joint* jParent;
    std::vector<Joint*> jChildren;
    glm::vec3 pos;
    glm::quat rot;
    glm::mat4 bindMat;

    void addChild(Joint* childJoint);
    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();

    Joint();
    Joint(QString name);
};

#endif // JOINT_H
