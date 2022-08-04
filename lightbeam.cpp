#include "lightbeam.h"

LightBeam::LightBeam(float _theta, float _phi, float _length, QWidget *parent) : theta(_theta), phi(_phi), length(_length), GLObject{parent}
{

}

void LightBeam::InitializeObject(QOpenGLShaderProgram* m_program){
    initializeOpenGLFunctions();

    QImage solid_color_image(1,1,QImage::Format_RGB16);
    solid_color_image.fill(Qt::red);
    texture = new QOpenGLTexture(solid_color_image);

    vao.create();
    vbo.create();

    float vertices[] = {
        0.0f,0.0f,0.0f,0.0f,0.0f,
        length * qCos(phi)*qSin(theta), length * qCos(theta), length * qSin(phi)*qSin(theta), 0.0f, 0.0f,
    };


    vao.bind();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    m_program->bind();

    // 最后一个参数代表偏移量（周期），表示元组大小的数据不需要乘上float的大小。
    m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
    m_program->enableAttributeArray("vPos");

    m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    m_program->enableAttributeArray("vTexture");

    m_program->release();
    vao.release();
    vbo.release();
}

void LightBeam::PaintObject(QOpenGLShaderProgram* m_program){
    vao.bind();

    texture->bind();
    glDrawArrays(GL_LINES, 0, 2);
    texture->release();

    vao.release();
}
