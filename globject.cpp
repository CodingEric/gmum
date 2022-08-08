#include "globject.h"

GLObject::GLObject(QWidget *parent) : QOpenGLWidget{parent}
{

}

void GLObject::setTexture(QOpenGLTexture* _texture){
    delete texture;
    texture = _texture;
}

void GLObject::paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement){
    throw std::logic_error("Not implemented.");
}

void GLObject::paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement, QVector2D color_info, bool do_gradient){
    throw std::logic_error("Not implemented.");
}
