#ifndef LIGHTBEAM_H
#define LIGHTBEAM_H

#include <QObject>
#include "globject.h"

class LightBeam : public GLObject
{
    Q_OBJECT
public:
    LightBeam(float _theta, float _phi, float _length, QColor _color, QWidget *parent = nullptr);
    void InitializeObject(QOpenGLShaderProgram* m_program);
    void PaintObject(QOpenGLShaderProgram* m_program);
    void SetDirection(float theta, float phi);
    float GetTheta();
    float GetPhi();
private:
    float length;
    float theta;
    float phi;
    QColor color;
signals:

};

#endif // LIGHTBEAM_H
