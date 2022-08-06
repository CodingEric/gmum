#ifndef LIGHTBEAM_H
#define LIGHTBEAM_H

#include <QObject>
#include <QFile>
#include "globject.h"

class LightBeam : public GLObject
{
    Q_OBJECT
public:
    LightBeam(float _theta, float _phi, float _length, QColor _color, QWidget *parent = nullptr);
    void InitializeObject();
    void PaintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement);
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
