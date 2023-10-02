#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this), m_progSkeleton(this),
      m_glCamera(), m_mesh(this), m_skeleton(this),
      m_vertDisplay(this), m_faceDisplay(this), m_halfEdgeDisplay(this),
      m_select(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_vertDisplay.destroy();
    m_faceDisplay.destroy();
    m_halfEdgeDisplay.destroy();
    m_select = nullptr;
    m_mesh.destroy();
    m_skeleton.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);
    m_mesh.create();
    m_skeleton.create();
    //Create the instances of Cylinder and Sphere.
    m_geomSquare.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    m_progSkeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    emit sig_sendVertexData(&m_mesh);
    emit sig_sendFaceData(&m_mesh);
    emit sig_sendHalfEdgeData(&m_mesh);
    emit sig_sendSkeletonData(&m_skeleton);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSkeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    //Send the geometry's transformation matrix to the shader
    //m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    //m_progLambert.draw(m_geomSquare);
    //m_progLambert.setModelMatrix(glm::mat4(1));
    //m_progLambert.draw(m_mesh);
    m_progSkeleton.setModelMatrix(glm::mat4(1.f));
    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    //model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    //m_progLambert.setModelMatrix(model);
    //m_progLambert.draw(m_geomSquare);

    Face* face = dynamic_cast<Face*>(m_select);
    HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(m_select);
    Vertex* vertex = dynamic_cast<Vertex*>(m_select);


    if (!m_mesh.vertices.empty() && !m_mesh.vertices[0]->jWeights.empty())
    {
        m_progSkeleton.draw(m_mesh);
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_skeleton);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        m_progFlat.draw(m_mesh);
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_skeleton);
        glEnable(GL_DEPTH_TEST);
    }

    glDisable(GL_DEPTH_TEST);

    if (face)
    {
        m_faceDisplay.create();
        m_progFlat.draw(m_faceDisplay);
    }
    else if(halfEdge)
    {
        m_halfEdgeDisplay.create();
        m_progFlat.draw(m_halfEdgeDisplay);
    }
    else if(vertex)
    {
        m_vertDisplay.create();
        m_progFlat.draw(m_vertDisplay);
    }

    glEnable(GL_DEPTH_TEST);
}

void MyGL::slot_loadObj()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("OpenObjFile"), QDir::currentPath().append(QString("../../obj_files")),
        tr("ObjFile(*.obj)"));

    QFile file(fileName);

    //std::unordered_map<int,int, HalfEdge*> map_sym;
    std::map<std::pair<int, int>, HalfEdge*> map_sym;

    if(!file.exists()) return;

    m_mesh.destroy();

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream streamIn(&file);

    while (!streamIn.atEnd())
    {
       QString line = streamIn.readLine();
       QStringList objInfoList = line.split(" ");

       // face
       if (objInfoList.at(0) == "f")
       {
           // initialize faces and edges
           uPtr<Face> face = mkU<Face>();
           face->setRdmColor();
           int lastIdx = objInfoList.length() - 1;

           for (int i = 1; i <= lastIdx; i++)
           {
               QString objInfo = objInfoList[i];
               uPtr<HalfEdge> he = mkU<HalfEdge>();

               //f pos/uv/norm
               int vertexIdx = objInfo.split("/")[0].toInt() - 1;
               he->face = face.get();
               he->vertex = m_mesh.vertices[vertexIdx].get();
               m_mesh.vertices[vertexIdx]->halfEdge = he.get();
               if(i == lastIdx)
               {
                    face->halfEdge = he.get();
                    m_mesh.faces.push_back(std::move(face));
               }
               m_mesh.halfEdges.push_back(std::move(he));
           }

           int edgeNum = m_mesh.halfEdges.size();

           for (int i = 1; i <= lastIdx; i++)
           {
               int objInfoSize = lastIdx + 1 - i;
               int edgeIdx = edgeNum - objInfoSize;

               if (i != lastIdx)
               {
                   HalfEdge *theNextHE = m_mesh.halfEdges[edgeIdx + 1].get();
                   m_mesh.halfEdges[edgeIdx]->nextHE = theNextHE;
               }

               HalfEdge *lastHE = m_mesh.halfEdges[edgeNum - lastIdx].get();
               m_mesh.halfEdges[edgeNum - 1]->nextHE = lastHE;

               //sym
               HalfEdge *he = m_mesh.halfEdges[edgeIdx].get();

               QStringList currObjInfo = objInfoList[i].split("/");
               QStringList prevObjInfo = i == 1 ? objInfoList[lastIdx].split("/") : objInfoList[i - 1].split("/");

               std::pair pair_he = {currObjInfo[0].toInt(), prevObjInfo[0].toInt()};
               std::pair pair_he_sym = {prevObjInfo[0].toInt(), currObjInfo[0].toInt()};

               if (map_sym.find(pair_he_sym) == map_sym.end())
               {
                   map_sym.insert({pair_he, he});
               }
               else
               {
                   HalfEdge *he_sym = map_sym.find(pair_he_sym)->second;
                   he->sym = he_sym;
                   he_sym->sym = he;
               }
           }
       }
       // vertex
       else if (objInfoList.at(0) == "v")
       {
           glm::vec3 pos = glm::vec3(objInfoList[1].toFloat(),
                                     objInfoList[2].toFloat(),
                                     objInfoList[3].toFloat());
           uPtr<Vertex> vertex = mkU<Vertex>();
           vertex->pos = pos;
           m_mesh.vertices.push_back(std::move(vertex));

       }
    }

    file.close();

    m_mesh.create();

    emit sig_sendFaceData(&m_mesh);
    emit sig_sendVertexData(&m_mesh);
    emit sig_sendHalfEdgeData(&m_mesh);

    update();
};

void MyGL::slot_loadJson(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("OpenJsonFile"), QDir::currentPath().append(QString("../../json_files")), tr("json_files (*.json)"));
    QFile file(fileName);

    for(unsigned int i = 0; i < m_skeleton.joints.size(); i++)
    {
        m_skeleton.joints[i]->jParent = nullptr;
        m_skeleton.joints[i].release();
    }
    m_skeleton.joints.clear();
    m_skeleton.destroy();

    if (file.exists())
    {
        jointIdCount = 0;
        m_skeleton.selectJoint();
        //m_skeleton.joints.clear();
        for (uPtr<Vertex> &v : m_mesh.vertices)
        {
            v->jWeights.clear();
        }

        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(0, "error", file.errorString());
        }

        QString val = file.readAll();
        file.close();
        QJsonObject root = QJsonDocument::fromJson(val.toUtf8()).object()["root"].toObject();

        loadJoints(root);
    }

    file.close();

    m_skeleton.create();

    emit sig_sendSkeletonData(&m_skeleton);

    update();
}

Joint* MyGL::loadJoints(QJsonObject root)
{
    QJsonArray rot = root["rot"].toArray();

    glm::vec3 pos = glm::vec3(root["pos"].toArray()[0].toDouble(),
                              root["pos"].toArray()[1].toDouble(),
                              root["pos"].toArray()[2].toDouble());

    double halfAngle = rot[0].toDouble() / 2;
    glm::quat qRot(cos(glm::radians(halfAngle)),
                   sin(glm::radians(halfAngle)) * rot[1].toDouble(),
                   sin(glm::radians(halfAngle)) * rot[2].toDouble(),
                   sin(glm::radians(halfAngle)) * rot[3].toDouble());

    uPtr<Joint> j = mkU<Joint>(root["name"].toString());
    j->pos = pos;
    j->rot = qRot;

    Joint* joint = j.get();
    m_skeleton.joints.push_back(std::move(j));

    QJsonArray children = root["children"].toArray();
    if(!children.isEmpty())
    {
        for(QJsonValueRef child: children)
        {
            joint->addChild(loadJoints(child.toObject()));
        }
    }
    return joint;
}

void MyGL::slot_meshSkinning()
{
    m_skeleton.bindJointsMatrix();
    m_progSkeleton.setTransMatrix(m_skeleton.setJointsTransMatrix());

    for (uPtr<Vertex> &v : m_mesh.vertices)
    {
        Joint* joint1 = nullptr;
        Joint* joint2 = nullptr;
        float distance1 = 0;
        float distance2 = 0;

        for (uPtr<Joint> &joint : m_skeleton.joints)
        {
            glm::vec4 jointPos = joint->getOverallTransformation() * glm::vec4(0, 0, 0, 1);
            float distanceJ2V = glm::distance(jointPos, glm::vec4(v->pos, 1));

            if (!joint1)
            {
                joint1 = joint.get();
                distance1 = distanceJ2V;
            }
            else
            {
                if (distance1 > distanceJ2V)
                {
                    joint2 = joint1;
                    distance2 = distance1;
                    joint1 = joint.get();
                    distance1 = distanceJ2V;
                }
                else
                {
                    joint2 = joint.get();
                    distance2 = distanceJ2V;
                }
            }
        }

        float j1Weight = distance1 / (distance1 + distance2);
        float j2Weight = distance2 / (distance1 + distance2);

        v->jWeights.push_back({joint1->jointId, j1Weight});
        v->jWeights.push_back({joint2->jointId, j2Weight});
    }

    std::vector<glm::mat4> bindMats;
    std::vector<glm::mat4> transMats;

    for (uPtr<Joint> &joint : m_skeleton.joints)
    {
        bindMats.push_back(joint->bindMat);
        transMats.push_back(joint->getOverallTransformation());
    }

    m_progSkeleton.setBindMatrix(bindMats);
    m_progSkeleton.setTransMatrix(transMats);

    m_mesh.create();
    m_progSkeleton.draw(m_mesh);

    update();
}

void MyGL::jointRot(glm::vec3 axis, float radians)
{
    int selectJointId = m_skeleton.getSelectedJoint();
    if (selectJointId != -1)
    {
        Joint* joint = dynamic_cast<Joint*>(m_skeleton.joints[selectJointId].get());

        if (joint)
        {
            joint->rot *= glm::rotate(glm::quat(), glm::radians(radians), axis);
            m_progSkeleton.setTransMatrix(m_skeleton.setJointsTransMatrix());

            m_skeleton.destroy();
            m_skeleton.create();

            update();
        }
    }
}

void MyGL::jointPos(glm::vec3 axis, double value)
{
    int selectedJoint = m_skeleton.getSelectedJoint();
    if (selectedJoint != -1)
    {
        Joint* joint = dynamic_cast<Joint*>(m_skeleton.joints[selectedJoint].get());

        if (joint)
        {
            if(axis.x > 0)
            {
                joint->pos.x = value;
            }
            else if(axis.y > 0)
            {
                joint->pos.y = value;
            }
            else
            {
                joint->pos.z = value;
            }

            m_progSkeleton.setTransMatrix(m_skeleton.setJointsTransMatrix());

            m_skeleton.destroy();
            m_skeleton.create();

            update();
        }
    }
}

void MyGL::slot_jointRotX()
{
    jointRot(glm::vec3(1, 0, 0), 5.f);
}

void MyGL::slot_jointRotY()
{
    jointRot(glm::vec3(0, 1, 0), 5.f);
}

void MyGL::slot_jointRotZ()
{
    jointRot(glm::vec3(0, 0, 1), 5.f);
}

void MyGL::slot_jointNegRotX()
{
    jointRot(glm::vec3(1, 0, 0), -5.f);
}

void MyGL::slot_jointNegRotY()
{
    jointRot(glm::vec3(0, 1, 0), -5.f);
}

void MyGL::slot_jointNegRotZ()
{
    jointRot(glm::vec3(0, 0, 1), -5.f);
}

void MyGL::slot_jointPosX(double pos)
{
    jointPos(glm::vec3(1, 0, 0), pos);
}

void MyGL::slot_jointPosY(double pos)
{
    jointPos(glm::vec3(0, 1, 0), pos);
}

void MyGL::slot_jointPosZ(double pos)
{
    jointPos(glm::vec3(0, 0, 1), pos);
}

void MyGL::slot_highlightSelectedVertex(QListWidgetItem *vt)
{
    if(vt)
    {
        m_vertDisplay.destroy();

        m_select = vt;
        Vertex* vertex = dynamic_cast<Vertex*>(vt);

        if(vertex)
        {
            m_vertDisplay.updateVertex(vertex);
        }
        update();
    }
}

void MyGL::slot_highlightSelectedFace(QListWidgetItem *f)
{
    if(f)
    {
        m_faceDisplay.destroy();

        m_select = f;
        Face* face = dynamic_cast<Face*>(f);

        if(face)
        {
            m_faceDisplay.updateFace(face);
        }
        update();
    }
}

void MyGL::slot_highlightSelectedHalfEdge(QListWidgetItem *he)
{
    if(he)
    {
        m_halfEdgeDisplay.destroy();

        m_select = he;
        HalfEdge* halfEdge = dynamic_cast<HalfEdge*>(he);

        if(halfEdge)
        {
            m_halfEdgeDisplay.updateHalfEdge(halfEdge);
        }
        update();
    }
}

void MyGL::slot_highlightSelectedJoint(QTreeWidgetItem *jt)
{
    if (jt)
    {
        Joint* joint = dynamic_cast<Joint*>(jt);

        if (joint)
        {
            m_skeleton.destroy();
            m_skeleton.selectJoint(joint);
            m_skeleton.create();
        }

        update();
    }
}

void MyGL::slot_updateVertX(double x)
{
    if(m_select)
    {
        Vertex* vt = dynamic_cast<Vertex*>(m_select);
        if (vt)
        {
            vt->pos.x = x;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_updateVertY(double y)
{
    if(m_select)
    {
        Vertex* vt = dynamic_cast<Vertex*>(m_select);
        if (vt)
        {
            vt->pos.y = y;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_updateVertZ(double z)
{
    if(m_select)
    {
        Vertex* vt = dynamic_cast<Vertex*>(m_select);
        if (vt)
        {
            vt->pos.z = z;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_updateFaceR(double r)
{
    if(m_select)
    {
        Face* f = dynamic_cast<Face*>(m_select);
        if (f)
        {
            f->color.r = r;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_updateFaceG(double g)
{
    if(m_select)
    {
        Face* f = dynamic_cast<Face*>(m_select);
        if (f)
        {
            f->color.g = g;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_updateFaceB(double b)
{
    if(m_select)
    {
        Face* f = dynamic_cast<Face*>(m_select);
        if (f)
        {
            f->color.b = b;
            m_mesh.destroy();
            m_mesh.create();
            update();
        }
    }
}

void MyGL::slot_splitEdge()
{
    if(m_select)
    {
        HalfEdge* he = dynamic_cast<HalfEdge*>(m_select);
        m_mesh.splitEdge(he);
        emit sig_sendVertexData(&m_mesh);
        emit sig_sendHalfEdgeData(&m_mesh);
    }

    m_mesh.destroy();
    m_mesh.create();
    update();
}

void MyGL::slot_triangulate()
{
    if(m_select)
    {
        Face* f = dynamic_cast<Face*>(m_select);
        m_mesh.triangulate(f);
        emit sig_sendHalfEdgeData(&m_mesh);
        emit sig_sendFaceData(&m_mesh);
    }

    m_mesh.destroy();
    m_mesh.create();
    update();
}

void MyGL::slot_subdivide()
{
    m_mesh.subdivide();
    emit sig_sendVertexData(&m_mesh);
    emit sig_sendHalfEdgeData(&m_mesh);
    emit sig_sendFaceData(&m_mesh);
    m_mesh.destroy();
    m_mesh.create();
    update();
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        HalfEdge* he = dynamic_cast<HalfEdge*>(m_select);
        if (he) {
            slot_highlightSelectedHalfEdge(he-> nextHE);
            std::cout << he->nextHE->halfEdgeId << std::endl;
        }
    } else if (e->key() == Qt::Key_M) {
        HalfEdge* he = dynamic_cast<HalfEdge*>(m_select);
        if (he) {
            slot_highlightSelectedHalfEdge(he->sym);
        }
    } else if (e->key() == Qt::Key_F) {
        HalfEdge* he = dynamic_cast<HalfEdge*>(m_select);
        if (he) {
            slot_highlightSelectedFace(he-> face);
        }
    } else if (e->key() == Qt::Key_V) {
        HalfEdge* he = dynamic_cast<HalfEdge*>(m_select);
        if (he) {
            slot_highlightSelectedVertex(he->vertex);
        }
    } else if (e->key() == Qt::Key_H && amount > 2.f) {
        Face* f = dynamic_cast<Face*>(m_select);
        if (f) {
            slot_highlightSelectedHalfEdge(f-> halfEdge);
        }
    } else if (e->key() == Qt::Key_H) {
        Vertex* v = dynamic_cast<Vertex*>(m_select);
        if (v) {
            slot_highlightSelectedHalfEdge(v-> halfEdge);
        }
    }

    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}
