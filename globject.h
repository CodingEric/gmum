#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QObject>

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <exception>

class GLObject : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit GLObject(QWidget *parent = nullptr);

    virtual void initializeObject() = 0;
    virtual void paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement);
    virtual void paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement, QVector2D color_info, bool do_gradient);

    void setTexture(QOpenGLTexture* _texture);

protected:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLTexture* texture;
    QOpenGLShaderProgram* program;
};

#endif // GLOBJECT_H
