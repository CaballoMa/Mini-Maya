#include "skeleton.h"

Skeleton::Skeleton(OpenGLContext *context)
    : Drawable(context), selectedJointID(-1)
{}

Skeleton::~Skeleton() {}

void Skeleton::create()
{
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> idx;

    int currentIdx = 0;

    for (uPtr<Joint> &joint: joints)
    {
        glm::vec4 offsetYZ = glm::vec4(0, 0.5, 0, 1);
        glm::vec3 axisX = glm::vec3(1, 0, 0);
        int originIdx = currentIdx;

        for (int i = 0; i < 12; i++)
        {
            pos.push_back(joint->getOverallTransformation() * offsetYZ);

            if (joint->jointId == selectedJointID)
            {
                col.push_back(glm::vec4(1, 1, 1, 1));
            }
            else
            {
                col.push_back(glm::vec4(1, 0, 0, 1));
            }

            idx.push_back(currentIdx);

            if (i == 11)
            {
                idx.push_back(originIdx);
                currentIdx++;
            }
            else
            {
                idx.push_back(++currentIdx);
            }

            offsetYZ = glm::rotate(offsetYZ, glm::radians(30.f), axisX);
        }

        glm::vec4 offsetZX = glm::vec4(0, 0, 0.5, 1);
        glm::vec3 axisY = glm::vec3(0, 1, 0);
        originIdx = currentIdx;

        for (int i = 0; i < 12; i++)
        {
            pos.push_back(joint->getOverallTransformation() * offsetZX);

            if (joint->jointId == selectedJointID)
            {
                 col.push_back(glm::vec4(1, 1, 1, 1));
            }
            else
            {
                col.push_back(glm::vec4(0, 1, 0, 1));
            }

            idx.push_back(currentIdx);

            if (i == 11)
            {
                idx.push_back(originIdx);
                currentIdx++;
            }
            else
            {
                idx.push_back(++currentIdx);
            }

            offsetZX = glm::rotate(offsetZX, glm::radians(30.f), axisY);
        }

        glm::vec4 offsetXY = glm::vec4(0.5, 0, 0, 1);
        glm::vec3 axisZ = glm::vec3(0, 0, 1);
        originIdx = currentIdx;

        for (int i = 0; i < 12; i++)
        {
            pos.push_back(joint->getOverallTransformation() * offsetXY);

            if (joint->jointId == selectedJointID)
            {
                 col.push_back(glm::vec4(1, 1, 1, 1));
            }
            else
            {
                col.push_back(glm::vec4(0, 0, 1, 1));
            }

            idx.push_back(currentIdx);

            if (i == 11)
            {
                idx.push_back(originIdx);
                currentIdx++;
            }
            else
            {
                idx.push_back(++currentIdx);
            }

            offsetXY = glm::rotate(offsetXY, glm::radians(30.f), axisZ);
        }

        if (joint->jParent)
        {
            pos.push_back(joint->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
            pos.push_back(joint->jParent->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
            col.push_back(glm::vec4(1, 1, 0, 1));
            col.push_back(glm::vec4(1, 0, 1, 1));
            idx.push_back(currentIdx);
            idx.push_back(++currentIdx);
            ++currentIdx;
        }
    }

    count = idx.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);


}


void Skeleton::selectJoint(Joint* joint)
{
    selectedJointID = joint->jointId;
}

void Skeleton::selectJoint()
{
    selectedJointID = -1;
}

int Skeleton::getSelectedJoint()
{
    return selectedJointID;
}

void Skeleton::bindJointsMatrix()
{
    for (uPtr<Joint> &joint : joints)
    {
        joint->bindMat = glm::inverse(joint->getOverallTransformation());
    }
}

std::vector<glm::mat4> Skeleton::setJointsTransMatrix()
{
    std::vector<glm::mat4> jTranslates = {};
    for (uPtr<Joint> &joint : joints)
    {
        jTranslates.push_back(joint->getOverallTransformation());
    }
    return jTranslates;
}

GLenum Skeleton::drawMode()
{
    return GL_LINES;
}
