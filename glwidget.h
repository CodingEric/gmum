#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtMath>
#include <QPainter>
#include <QFile>
#include <QKeyEvent>
#include <QApplication>
#include <QDateTime>

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "globject.h"
#include "reflectosphere.h"
#include "lightbeam.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);

    void setDoDisplacement(bool _do_displacement);
    void setGLWidgetRayState(bool is_visible);
    void setGLWidgetSphereState(bool is_visible);
    void setSphereTexture(QImage* image, float incident_theta, float incident_phi, QVector2D color_info);
    void setDoGradient(bool do_gradient);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
private:
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
    bool no_data;
    bool do_displacement;
    bool do_gradient;
    QVector2D color_info;
    // std::map<GLObject*, bool> object_visibility;
};

#endif // GLWIDGET_H
