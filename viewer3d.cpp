#include <Qt3D/QGLAbstractScene>

#include "viewer3d.h"

Viewer3D::Viewer3D(const QString& path, QWindow* parent): QGLView(parent), scene(NULL), rootNode(NULL)
{
    this->path = path;

    // Viewing Volume
    camera()->setFieldOfView(25);
    camera()->setNearPlane(1);
    camera()->setFarPlane(1000);

    // Position of the camera
    camera()->setEye(QVector3D(0, 3, 4));
    //camera()->setEye(QVector3D(500, 500, 500));

    // Direction that the camera is pointing
    camera()->setCenter(QVector3D(0, 3, 0));
}

Viewer3D::~Viewer3D()
{
    delete scene;
}

void Viewer3D::initializeGL(QGLPainter* painter)
{
    // Background color
    painter->setClearColor(QColor(70, 70, 70));

    // Load the 3d model from the file
    scene = QGLAbstractScene::loadScene(path);

    rootNode = scene->mainNode();
}

void Viewer3D::paintGL(QGLPainter* painter)
{
    rootNode->draw(painter);
}
