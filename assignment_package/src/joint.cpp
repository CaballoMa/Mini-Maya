#include "joint.h"

Joint::Joint():
    QTreeWidgetItem(),
    jointId(jointIdCount++),
    name(),
    jParent(nullptr),
    jChildren(),
    pos(glm::vec3()),
    rot(glm::quat()),
    bindMat(glm::mat4())
{
    this->QTreeWidgetItem::setText(0, this->name);
}

Joint::Joint(QString n):
    QTreeWidgetItem(),
    jointId(jointIdCount++),
    name(n),
    jParent(nullptr),
    jChildren(),
    pos(glm::vec3()),
    rot(glm::quat()),
    bindMat(glm::mat4())
{
    this->QTreeWidgetItem::setText(0, this->name);
}

void Joint::addChild(Joint* child)
{
    QTreeWidgetItem::addChild(child);
    jChildren.push_back(child);
    child->jParent = this;
}

glm::mat4 Joint::getLocalTransformation()
{
    glm::mat4 trans = glm::mat4(glm::vec4(1, 0, 0, 0),
                            glm::vec4(0, 1, 0, 0),
                            glm::vec4(0, 0, 1, 0),
                            glm::vec4(pos, 1));
    glm::mat4 rotate = glm::mat4_cast(rot);

    return trans * rotate;
}

glm::mat4 Joint::getOverallTransformation()
{
    return this->jParent != nullptr ?
    jParent->getOverallTransformation() * this->getLocalTransformation()
              : this->getLocalTransformation();
}
