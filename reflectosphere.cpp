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

void ReflectoSphere::InitializeObject(QOpenGLShaderProgram* m_program){
    initializeOpenGLFunctions();
    QImage temp_texture(1,1,QImage::Format_RGB32);
    temp_texture.fill(Qt::magenta);
    texture = new QOpenGLTexture(temp_texture);

    vao.create();
    vbo.create();

    face_count = 0;

    float d_theta =  M_PI_2 / subdivision_theta;
    float d_phi = 2.0f * M_PI / subdivision_phi;

    std::vector<Point> vertices;

    for(float theta = 0.0f + d_theta; theta <= M_PI_2; theta += d_theta){
        for(float phi = 0.0f; phi <= 2.0f * M_PI; phi += d_phi){
            vertices.push_back(Point(qCos(phi)*qSin(theta - d_theta),qCos(theta - d_theta),qSin(phi)*qSin(theta - d_theta),static_cast<float>(phi/(2.0f * M_PI)), static_cast<float>((theta - d_theta)/M_PI_2))); // theta - d_theta, phi
            vertices.push_back(Point(qCos(phi + d_phi)*qSin(theta - d_theta),qCos(theta - d_theta),qSin(phi + d_phi)*qSin(theta - d_theta), static_cast<float>((phi + d_phi)/(2.0f * M_PI)), static_cast<float>((theta - d_theta)/(M_PI_2)))); // theta - d_theta, phi + d_phi
            vertices.push_back(Point(qCos(phi + d_phi)*qSin(theta),qCos(theta),qSin(phi + d_phi)*qSin(theta), static_cast<float>((phi + d_phi)/(2.0f * M_PI)), static_cast<float>(theta/(M_PI_2)))); // theta, phi + d_phi
            vertices.push_back(Point(qCos(phi)*qSin(theta),qCos(theta),qSin(phi)*qSin(theta), static_cast<float>(phi/(2.0f * M_PI)), static_cast<float>(theta/M_PI_2))); // theta, phi
            ++face_count;
        }
    } 

    vao.bind();
    vbo.bind();

    vbo.allocate(&vertices[0], vertices.size() * sizeof(Point));

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

void ReflectoSphere::PaintObject(QOpenGLShaderProgram* m_program){
    vao.bind();

    texture->bind();
    for(int i=0;i<face_count;++i){
        glDrawArrays(GL_POLYGON, 4*i, 4);
    }
    texture->release();
    //glDrawArrays(GL_POLYGON, 4, 4);

    vao.release();
}
