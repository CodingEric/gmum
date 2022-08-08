#include "lightbeam.h"

LightBeam::LightBeam(float _theta, float _phi, float _length, QColor _color, QWidget *parent) : theta(_theta), phi(_phi), length(_length), color(_color), GLObject{parent}
{

}

void LightBeam::initializeObject(){
    initializeOpenGLFunctions();

    // Initialize shaders.

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

    // Initialize texture.

    QImage solid_color_image(1,1,QImage::Format_RGB16);
    solid_color_image.fill(color);
    texture = new QOpenGLTexture(solid_color_image);

    vao.create();
    vbo.create();

    // Generate vertices.

    float vertices[] = {
        0.0f,0.0f,0.0f,0.0f,0.0f,
        length * qCos(phi)*qSin(theta), length * qCos(theta), length * qSin(phi)*qSin(theta), 0.0f, 0.0f,
    };

    // Setting up VAO & VBO.

    vao.bind();
    vbo.bind();

    vbo.allocate(vertices, sizeof(vertices));

    program->bind();

    // Structure of VAOs: [x y z] [u v]

    program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
    program->enableAttributeArray("vPos");

    program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    program->enableAttributeArray("vTexture");

    program->release();
    vao.release();
    vbo.release();
}

void LightBeam::paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement){
    program->bind();

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


void LightBeam::setDirection(float theta, float phi){
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

float LightBeam::getTheta(){
    return theta;
}

float LightBeam::getPhi(){
    return phi;
}
