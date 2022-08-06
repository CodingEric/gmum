#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QtMath>
#include <QPainter>
#include <QFile>
#include <QKeyEvent>
#include <QApplication>

#include "globject.h"
#include "reflectosphere.h"
#include "lightbeam.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void timerEvent(QTimerEvent *event) override;

    void keyPressEvent(QKeyEvent *e) override;
private:
    QOpenGLShaderProgram* m_program;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;
    QMatrix4x4 m_model;
    // std::vector<GLObject*> objects;
    LightBeam* inray;
    LightBeam* outray;
    ReflectoSphere* reflecto_sphere;
    LightBeam* normal;

    bool is_ray_visible;
    bool is_sphere_visible;
    // std::map<GLObject*, bool> object_visibility;
private slots:
    void slotSetGLWidgetRayState(bool is_visible);
    void slotSetGLWidgetSphereState(bool is_visible);
    void slotSetSphereTexture(QImage* image, float incident_theta, float incident_phi);
};

#endif // GLWIDGET_H
