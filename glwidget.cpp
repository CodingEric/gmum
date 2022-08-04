#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget{parent}
{
    // 打开多重采样。
    auto new_format = this->format();
    new_format.setSamples(16);
    this->setFormat(new_format);

    objects.push_back(new ReflectoSphere(this,32,32));
    objects.push_back(new LightBeam(M_PI_2, M_PI_4, 32, this));

    resize(800,600);
    startTimer(1000/60);
}

float offset = 0;

void GLWidget::timerEvent(QTimerEvent *event){
    offset += 1;
    if(offset >= 360)offset = 0;
    m_model.setToIdentity();
    m_model.rotate(offset, 0, 1, 0);
    repaint();
}


void GLWidget::initializeGL(){

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,1,1);

    QFile vertex_shader_file(":/shaders/vertexshader.glsl");
    vertex_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString vertex_shader = vertex_shader_file.readAll();

    QFile fragment_shader_file(":/shaders/fragmentshader.glsl");
    fragment_shader_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fragment_shader = fragment_shader_file.readAll();

    m_program = new QOpenGLShaderProgram();
    m_program -> addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader);
    m_program -> addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader);
    m_program -> link();

    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        (*it)->InitializeObject(m_program);
    }


}

void GLWidget::paintGL() {
    m_program->bind();

    // 这跟顶点着色器的 uniform mat4 view 对应
    // uniform 值和 in/out 相比，uniform是所有着色器统一使用的相同的值。
    m_program->setUniformValue("view",m_view);
    m_program->setUniformValue("projection",m_projection);
    m_program->setUniformValue("model",m_model);

    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        (*it)->PaintObject(m_program);
    }

    m_program->release();

    QPainter painter(this);
    auto rect = this->rect();
    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas",15));
    painter.drawText(rect, Qt::AlignRight, QString("GONIOREFLECTOMETER UPPER MONITOR\nPress [ESC] to quit."));

}

void GLWidget::resizeGL(int w, int h) {
    m_view.setToIdentity();
    m_view.lookAt(QVector3D(2, 2, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    m_projection.setToIdentity();
    m_projection.perspective(60,(float)w/h,0.001,1000);
}

void GLWidget::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_Escape)
        QApplication::quit();
}
