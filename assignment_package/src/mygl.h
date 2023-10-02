#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include <skeleton.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QFileDialog>
#include <QMessageBox>
#include "QTreeWidget"
#include "QListWidget"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <mesh.h>
#include "vertexdisplay.h"
#include "facedisplay.h"
#include "halfedgedisplay.h"

class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton;
    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;


public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    Mesh m_mesh;
    Skeleton m_skeleton;
    VertexDisplay m_vertDisplay;
    FaceDisplay m_faceDisplay;
    HalfEdgeDisplay m_halfEdgeDisplay;
    QListWidgetItem* m_select;
    Joint* loadJoints(QJsonObject root);
    void jointRot(glm::vec3 axis, float radians);
    void jointPos(glm::vec3 axis, double value);

public slots:
    void slot_loadObj();
    void slot_loadJson();
    void slot_meshSkinning();
    void slot_highlightSelectedVertex(QListWidgetItem* v);
    void slot_highlightSelectedFace(QListWidgetItem* f);
    void slot_highlightSelectedHalfEdge(QListWidgetItem* he);
    void slot_highlightSelectedJoint(QTreeWidgetItem *j);
    void slot_updateVertX(double);
    void slot_updateVertY(double);
    void slot_updateVertZ(double);
    void slot_updateFaceR(double);
    void slot_updateFaceG(double);
    void slot_updateFaceB(double);
    void slot_splitEdge();
    void slot_triangulate();
    void slot_subdivide();
    void slot_jointRotX();
    void slot_jointRotY();
    void slot_jointRotZ();
    void slot_jointNegRotX();
    void slot_jointNegRotY();
    void slot_jointNegRotZ();
    void slot_jointPosX(double);
    void slot_jointPosY(double);
    void slot_jointPosZ(double);

signals:
    void sig_sendVertexData(Mesh* mesh);
    void sig_sendFaceData(Mesh* mesh);
    void sig_sendHalfEdgeData(Mesh* mesh);
    void sig_sendSkeletonData(Skeleton*);

protected:
    void keyPressEvent(QKeyEvent *e);
};


#endif // MYGL_H
