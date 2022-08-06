#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget{parent}
{
    // 打开多重采样。
    auto new_format = this->format();
    new_format.setSamples(16);
    this->setFormat(new_format);

    reflecto_sphere = new ReflectoSphere(this,64,64);
    // object_visibility[*(objects.end()-1)] = true;
    inray = new LightBeam(M_PI_2, M_PI_4, 32, Qt::red, this);
    outray = new LightBeam(M_PI_2, M_PI_4, 32, Qt::green, this);
    // object_visibility[*(objects.end()-1)] = true;
    // resize(800,600);
    normal = new LightBeam(0,0, 32, Qt::yellow, this);

    is_ray_visible = true;
    is_sphere_visible = true;
    no_data = true;
    do_displacement = true;

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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,1,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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

    /*
    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        (*it)->InitializeObject(m_program);
    }
    */

    inray -> InitializeObject(m_program);
    outray -> InitializeObject(m_program);
    reflecto_sphere -> InitializeObject(m_program);
    normal -> InitializeObject(m_program);

}

void GLWidget::paintGL() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_program->bind();

    // 这跟顶点着色器的 uniform mat4 view 对应
    // uniform 值和 in/out 相比，uniform是所有着色器统一使用的相同的值。
    m_program->setUniformValue("view",m_view);
    m_program->setUniformValue("projection",m_projection);
    m_program->setUniformValue("model",m_model);
    m_program->setUniformValue("do_displacement",do_displacement);

    /*
    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        if(object_visibility[*it] == false) continue;
        (*it)->PaintObject(m_program);
    }
    */
    if(is_sphere_visible)
        reflecto_sphere->PaintObject(m_program);
    if(is_ray_visible)
    {
        inray->PaintObject(m_program);
        outray->PaintObject(m_program);
        normal->PaintObject(m_program);
    }

    m_program->release();

    QPainter painter(this);
    auto rect = this->rect();
    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas",12));
    painter.drawText(rect, Qt::AlignRight, QString("GONIOREFLECTOMETER UPPER MONITOR"));
    painter.setPen(Qt::red);
    painter.drawText(rect, Qt::AlignRight, QString("\nIncident theta: ")+QString::number(inray->GetTheta())+QString(" rad"));
    painter.drawText(rect, Qt::AlignRight, QString("\n\nIncident phi: ")+QString::number(inray->GetPhi())+QString(" rad"));
    painter.setPen(Qt::green);
    painter.drawText(rect, Qt::AlignRight, QString("\n\n\nIdeal specular reflected theta: ")+QString::number(outray->GetTheta())+QString(" rad"));
    painter.drawText(rect, Qt::AlignRight, QString("\n\n\n\nIdeal specular reflected phi: ")+QString::number(outray->GetPhi())+QString(" rad"));

    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignBottom | Qt::AlignRight, QString("QOpenGLContext::openGLModuleType() returns ") + QString::number(QOpenGLContext::openGLModuleType()));

    if(no_data){
        painter.setPen(Qt::white);
        painter.setFont(QFont("Consolas",16));
        painter.drawText(rect, Qt::AlignCenter, QString("ERROR: NO GONIOREFLECTOMETER DATA."));
    }
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

void GLWidget::slotSetGLWidgetRayState(bool is_visible){
    // object_visibility[objects[1]] = is_visible;
    is_ray_visible = is_visible;
}

void GLWidget::slotSetGLWidgetSphereState(bool is_visible){
    // object_visibility[objects[0]] = is_visible;
    is_sphere_visible = is_visible;
}

void GLWidget::slotSetSphereTexture(QImage* image, float incident_theta, float incident_phi){
    reflecto_sphere->setTexture(new QOpenGLTexture(*image));
    // delete inray;
    // delete outray;

    // inray = new LightBeam(incident_theta,incident_phi,32,Qt::red,this);
    // inray->InitializeObject(m_program);
    inray->SetDirection(incident_theta, incident_phi);
    outray->SetDirection(incident_theta, incident_phi+M_PI);
    no_data = false;
}

void GLWidget::setDoDisplacement(bool _do_displacement){
    do_displacement = _do_displacement;
}
