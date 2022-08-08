#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget{parent}
{
    // Enable multisampling.
    auto new_format = this->format();
    new_format.setSamples(16);
    this->setFormat(new_format);

    reflecto_sphere = new ReflectoSphere(this,128,128);
    // object_visibility[*(objects.end()-1)] = true;
    inray = new LightBeam(M_PI_2, M_PI_4, 32, Qt::red, this);
    outray = new LightBeam(M_PI_2, M_PI_4, 32, Qt::green, this);
    // object_visibility[*(objects.end()-1)] = true;
    normal = new LightBeam(0,0, 32, Qt::yellow, this);

    is_ray_visible = true;
    is_sphere_visible = true;
    no_data = true;
    do_displacement = true;
    do_gradient = true;
    color_info = {0,0};

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
    glClearColor(0,0,0,1);

    /*
    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        (*it)->InitializeObject(m_program);
    }
    */

    inray -> initializeObject();
    outray -> initializeObject();
    reflecto_sphere -> initializeObject();
    normal -> initializeObject();

}

void GLWidget::paintGL() {

    /*
    for(std::vector<GLObject*>::iterator it = objects.begin();it!=objects.end();++it){
        if(object_visibility[*it] == false) continue;
        (*it)->PaintObject(m_program);
    }
    */

    if(is_sphere_visible)
        reflecto_sphere->paintObject(m_view, m_projection, m_model, do_displacement, color_info, do_gradient);
    if(is_ray_visible)
    {
        inray->paintObject(m_view, m_projection, m_model, do_displacement);
        outray->paintObject(m_view, m_projection, m_model, do_displacement);
        normal->paintObject(m_view, m_projection, m_model, do_displacement);
    }

    // Paint 2D UI.

    QPainter painter(this);
    auto rect = this->rect();

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas",12));
    painter.drawText(rect, Qt::AlignRight, QString("GONIOREFLECTOMETER UPPER MONITOR"));
    painter.setPen(Qt::red);
    painter.drawText(rect, Qt::AlignRight, QString("\nIncident theta: ")+QString::number(inray->getTheta())+QString(" rad"));
    painter.drawText(rect, Qt::AlignRight, QString("\n\nIncident phi: ")+QString::number(inray->getPhi())+QString(" rad"));
    painter.setPen(Qt::green);
    painter.drawText(rect, Qt::AlignRight, QString("\n\n\nIdeal specular reflected theta: ")+QString::number(outray->getTheta())+QString(" rad"));
    painter.drawText(rect, Qt::AlignRight, QString("\n\n\n\nIdeal specular reflected phi: ")+QString::number(outray->getPhi())+QString(" rad"));

    painter.setPen(Qt::white);
    painter.drawText(rect, Qt::AlignBottom | Qt::AlignRight, QString("QOpenGLContext::openGLModuleType() returns ") + QString::number(QOpenGLContext::openGLModuleType()));

    if(no_data){
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.setFont(QFont("Source Han Sans CN",16,QFont::Black));
        painter.drawRect(0,rect.center().y()-50,rect.width(),100);
        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignCenter, QString("错误：没有反射数据。"));
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

void GLWidget::setGLWidgetRayState(bool is_visible){
    // object_visibility[objects[1]] = is_visible;
    is_ray_visible = is_visible;
}

void GLWidget::setGLWidgetSphereState(bool is_visible){
    // object_visibility[objects[0]] = is_visible;
    is_sphere_visible = is_visible;
}

void GLWidget::setSphereTexture(QImage* image, float incident_theta, float incident_phi, QVector2D color_info){
    reflecto_sphere->setTexture(new QOpenGLTexture(*image));
    inray->setDirection(incident_theta, incident_phi);
    outray->setDirection(incident_theta, incident_phi+M_PI);
    this->color_info = color_info;
    no_data = false;
}

void GLWidget::setDoGradient(bool do_gradient){
    this->do_gradient = do_gradient;
}

void GLWidget::setDoDisplacement(bool _do_displacement){
    do_displacement = _do_displacement;
}
