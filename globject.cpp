#include "globject.h"

GLObject::GLObject(QWidget *parent) : QOpenGLWidget{parent}
{

}

void GLObject::setTexture(QOpenGLTexture* _texture){
    delete texture;
    texture = _texture;
}
