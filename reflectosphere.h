#ifndef REFLECTOSPHERE_H
#define REFLECTOSPHERE_H

#include <QObject>
#include "globject.h"

class ReflectoSphere : public GLObject
{
    Q_OBJECT
public:
    explicit ReflectoSphere(QWidget *parent = nullptr, int _subdivision_theta = 16, int _subdivision_phi = 16);
    void InitializeObject(QOpenGLShaderProgram* m_program);
    void PaintObject(QOpenGLShaderProgram* m_program);
private:
    int subdivision_theta;
    int subdivision_phi;
    int face_count = 0;
signals:

};

#endif // REFLECTOSPHERE_H
