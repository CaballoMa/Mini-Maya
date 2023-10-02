#ifndef SKELETON_H
#define SKELETON_H

#include <joint.h>
#include <drawable.h>
#include <smartpointerhelp.h>
#include <glm/gtx/rotate_vector.hpp>

class Skeleton: public Drawable
{
public:
    std::vector <uPtr<Joint>> joints;

    Skeleton(OpenGLContext* context);
    ~Skeleton();
    void create() override;
    GLenum drawMode() override;

    void selectJoint(Joint*);
    void selectJoint();
    int getSelectedJoint();
    void bindJointsMatrix();
    std::vector<glm::mat4> setJointsTransMatrix();

private:
    int selectedJointID;
};

#endif // SKELETON_H
