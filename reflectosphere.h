#ifndef REFLECTOSPHERE_H
#define REFLECTOSPHERE_H

#include <QObject>
#include <QFile>

#include "globject.h"

class ReflectoSphere : public GLObject
{
    Q_OBJECT
public:
    explicit ReflectoSphere(QWidget *parent = nullptr, int _subdivision_theta = 16, int _subdivision_phi = 16);
    void initializeObject();
    void paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement, QVector2D color_info, bool do_gradient);
private:
    int subdivision_theta;
    int subdivision_phi;
    int face_count;
signals:

};

#endif // REFLECTOSPHERE_H
