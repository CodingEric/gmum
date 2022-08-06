#include "lightbeam.h"

LightBeam::LightBeam(float _theta, float _phi, float _length, QColor _color, QWidget *parent) : theta(_theta), phi(_phi), length(_length), color(_color), GLObject{parent}
{

}

void LightBeam::InitializeObject(){
    initializeOpenGLFunctions();

    QFile vertex_shader_file(":/shaders/ray.vsh");
    vertex_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString vertex_shader = vertex_shader_file.readAll();

    QFile fragment_shader_file(":/shaders/ray.fsh");
    fragment_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fragment_shader = fragment_shader_file.readAll();

    program = new QOpenGLShaderProgram();
    program -> addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader);
    program -> addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader);
    program -> link();



    QImage solid_color_image(1,1,QImage::Format_RGB16);
    solid_color_image.fill(color);
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

    program->bind();

    // 最后一个参数代表偏移量（周期），表示元组大小的数据不需要乘上float的大小。
    program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
    program->enableAttributeArray("vPos");

    program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    program->enableAttributeArray("vTexture");

    program->release();
    vao.release();
    vbo.release();
}

void LightBeam::PaintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement){
    program->bind();

    // 这跟顶点着色器的 uniform mat4 view 对应
    // uniform 值和 in/out 相比，uniform是所有着色器统一使用的相同的值。
    program->setUniformValue("view",m_view);
    program->setUniformValue("projection",m_projection);
    program->setUniformValue("model",m_model);
    program->setUniformValue("do_displacement",do_displacement);

    vao.bind();

    texture->bind();
    glDrawArrays(GL_LINES, 0, 2);
    texture->release();

    vao.release();

    program->release();
}


void LightBeam::SetDirection(float theta, float phi){
    this->theta = theta;
    this->phi = phi;
    vbo.bind();
    float* vertices = static_cast<float*>(vbo.map(QOpenGLBuffer::WriteOnly));
    vertices[5] =  length * qCos(phi) * qSin(theta);
    vertices[6] =  length * qCos(theta);
    vertices[7] =  length * qSin(phi) * qSin(theta);
    vbo.unmap();
    vbo.release();
}

float LightBeam::GetTheta(){
    return theta;
}

float LightBeam::GetPhi(){
    return phi;
}
