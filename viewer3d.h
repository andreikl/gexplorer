#ifndef VIEWER3D_H
#define VIEWER3D_H

#include <Qt3D/QGLView>

QT_BEGIN_NAMESPACE
class QGLAbstractScene;
class QGLSceneNode;
QT_END_NAMESPACE

class Viewer3D: public QGLView
{
    Q_OBJECT

public:
    Viewer3D(const QString& path, QWindow* parent = NULL);
    ~Viewer3D();

protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);

private:
    QString path;

private:
    QGLAbstractScene* scene;
    QGLSceneNode* rootNode;
};

#endif // VIEWER3D_H
