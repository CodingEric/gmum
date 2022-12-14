#include "reflectosphere.h"

ReflectoSphere::ReflectoSphere(QWidget *parent, int _subdivision_theta, int _subdivision_phi)
    : GLObject{parent}, subdivision_theta(_subdivision_theta), subdivision_phi(_subdivision_phi)
{

}

class Point{
public:
    Point(float _x,float _y,float _z,float _u,float _v):x(_x),y(_y),z(_z),u(_u),v(_v){}
private:
    float x;
    float y;
    float z;
    float u;
    float v;
};

void ReflectoSphere::initializeObject(){
    initializeOpenGLFunctions();

    // Initialize shaders.

    QFile vertex_shader_file(":/shaders/sphere.vsh");
    vertex_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString vertex_shader = vertex_shader_file.readAll();
    QFile fragment_shader_file(":/shaders/sphere.fsh");
    fragment_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fragment_shader = fragment_shader_file.readAll();

    program = new QOpenGLShaderProgram();
    program -> addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader);
    program -> addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader);
    program -> link();

    // Initialize texture.

    QImage temp_texture(1,1,QImage::Format_RGB32);
    temp_texture.fill(Qt::white);
    texture = new QOpenGLTexture(temp_texture);

    vao.create();
    vbo.create();

    // Generate vertices.

    face_count = 0;

    float d_theta =  M_PI_2 / subdivision_theta;
    float d_phi = 2.0f * M_PI / subdivision_phi;

    std::vector<Point> vertices;

    for(float theta = 0.0f + d_theta; theta < M_PI_2 - d_theta; theta += d_theta){
        for(float phi = 0.0f; phi < 2.0f * M_PI; phi += d_phi){
            vertices.push_back(Point(qCos(phi)*qSin(theta - d_theta),qCos(theta - d_theta),qSin(phi)*qSin(theta - d_theta),static_cast<float>(phi/(2.0f * M_PI)), static_cast<float>((theta - d_theta)/M_PI_2))); // theta - d_theta, phi
            vertices.push_back(Point(qCos(phi + d_phi)*qSin(theta - d_theta),qCos(theta - d_theta),qSin(phi + d_phi)*qSin(theta - d_theta), static_cast<float>((phi + d_phi)/(2.0f * M_PI)), static_cast<float>((theta - d_theta)/(M_PI_2)))); // theta - d_theta, phi + d_phi
            vertices.push_back(Point(qCos(phi + d_phi)*qSin(theta),qCos(theta),qSin(phi + d_phi)*qSin(theta), static_cast<float>((phi + d_phi)/(2.0f * M_PI)), static_cast<float>(theta/(M_PI_2)))); // theta, phi + d_phi
            vertices.push_back(Point(qCos(phi)*qSin(theta),qCos(theta),qSin(phi)*qSin(theta), static_cast<float>(phi/(2.0f * M_PI)), static_cast<float>(theta/M_PI_2))); // theta, phi
            ++face_count;
        }
    }

    // Setting up VAO & VBO.

    vao.bind();
    vbo.bind();

    vbo.allocate(&vertices[0], vertices.size() * sizeof(Point));

    // Structure of VAOs: [x y z] [u v]

    program->bind();
    program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
    program->enableAttributeArray("vPos");
    program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    program->enableAttributeArray("vTexture");
    program->release();

    vao.release();
    vbo.release();
}

void ReflectoSphere::paintObject(QMatrix4x4 m_view, QMatrix4x4 m_projection, QMatrix4x4 m_model, bool do_displacement, QVector2D color_info, bool do_gradient){

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    program->bind();

    program->setUniformValue("view",m_view);
    program->setUniformValue("projection",m_projection);
    program->setUniformValue("model",m_model);
    program->setUniformValue("do_displacement",do_displacement);
    program->setUniformValue("colorInfo",color_info);
    program->setUniformValue("doGradient",do_gradient);

    vao.bind();
    texture->bind();
    for(int i=0;i<face_count;++i){
        glDrawArrays(GL_POLYGON, 4*i, 4);
    }
    texture->release();
    vao.release();

    program->release();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
