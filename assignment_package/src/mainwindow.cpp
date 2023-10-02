#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    connect(ui->mygl, SIGNAL(sig_sendVertexData(Mesh*)),
            this, SLOT(slot_receiveVertexData(Mesh*)));
    connect(ui->mygl, SIGNAL(sig_sendFaceData(Mesh*)),
            this, SLOT(slot_receiveFaceData(Mesh*)));
    connect(ui->mygl, SIGNAL(sig_sendHalfEdgeData(Mesh*)),
            this, SLOT(slot_receiveHalfEdgeData(Mesh*)));
    connect(ui->mygl, SIGNAL(sig_sendSkeletonData(Skeleton*)),
            this, SLOT(slot_receiveSkeletonData(Skeleton*)));

    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_highlightSelectedVertex(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_highlightSelectedFace(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            ui->mygl, SLOT(slot_highlightSelectedHalfEdge(QListWidgetItem*)));
    connect(ui->jointTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            ui->mygl, SLOT(slot_highlightSelectedJoint(QTreeWidgetItem*)));

    connect(ui->LoadObjButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_loadObj()));
    connect(ui->LoadJsonButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_loadJson()));
    connect(ui->MeshSkinningButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_meshSkinning()));

    // rotate
    connect(ui->RotateXButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointRotX()));
    connect(ui->RotateYButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointRotY()));
    connect(ui->RotateZButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointRotZ()));
    connect(ui->RotateNegXButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointNegRotX()));
    connect(ui->RotateNegYButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointNegRotY()));
    connect(ui->RotateNegZButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_jointNegRotZ()));

    // translate
    connect(ui->PosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointPosX(double)));
    connect(ui->PosYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointPosY(double)));
    connect(ui->PosZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_jointPosZ(double)));

    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateVertX(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateVertY(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateVertZ(double)));

    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateFaceR(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateFaceG(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_updateFaceB(double)));

    connect(ui->AddVertexButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_splitEdge()));
    connect(ui->TriangulateButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_triangulate()));
    connect(ui->subdivideButton, SIGNAL(clicked(bool)),
            ui->mygl, SLOT(slot_subdivide()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_receiveFaceData(Mesh *mesh)
{
    for (const uPtr<Face> &f : mesh->faces)
    {
        ui->facesListWidget->addItem(f.get());
    }
}

void MainWindow::slot_receiveHalfEdgeData(Mesh *mesh)
{
    for (const uPtr<HalfEdge> &he : mesh->halfEdges)
    {
        ui->halfEdgesListWidget->addItem(he.get());
    }
}

void MainWindow::slot_receiveVertexData(Mesh *mesh)
{
    for (const uPtr<Vertex> &vt : mesh->vertices)
    {
        ui->vertsListWidget->addItem(vt.get());
    }
}

void MainWindow::slot_receiveSkeletonData(Skeleton *skt)
{
    ui->jointTreeWidget->clear();
    for (const uPtr<Joint> &joint : skt->joints)
    {
        ui->jointTreeWidget->addTopLevelItem(joint.get());
    }
}
