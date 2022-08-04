#include "globject.h"

GLObject::GLObject(QWidget *parent) : QOpenGLWidget{parent}
{

}

void GLObject::set_texture(const QOpenGLTexture _texture){
    texture(_texture);
}
