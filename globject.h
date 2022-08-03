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
    virtual void InitializeObject(QOpenGLShaderProgram* m_program) = 0;
    virtual void PaintObject(QOpenGLShaderProgram* m_program) = 0;
protected:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};

#endif // GLOBJECT_H