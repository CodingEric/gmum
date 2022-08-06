#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <QObject>

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GLObject : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    GLObject(QWidget *parent = nullptr);
    virtual void InitializeObject() = 0;
    virtual void PaintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement) = 0;
    void setTexture(QOpenGLTexture* _texture);

protected:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLTexture* texture;
    QOpenGLShaderProgram* program;

};

#endif // GLOBJECT_H
