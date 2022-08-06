#include "mainwindow.h"

MainWindow::MainWindow()
{
    this->resize(800,800);

    gl_widget = new GLWidget(this);
    chkbox_ray_visibility = new QCheckBox("Show rays", this);
    chkbox_ray_visibility->setCheckState(Qt::Checked);
    chkbox_ray_visibility->setTristate(false);
    chkbox_sphere_visibility = new QCheckBox("Show reflecto sphere", this);
    chkbox_sphere_visibility->setCheckState(Qt::Checked);
    chkbox_sphere_visibility->setTristate(false);
    chkbox_do_displacement = new QCheckBox("Enable displacement", this);
    chkbox_do_displacement->setCheckState(Qt::Checked);
    chkbox_do_displacement->setTristate(false);

    label_phong_in_phi = new QLabel("入射光Φ",this);
    spinbox_phong_in_phi = new QSpinBox(this);
    spinbox_phong_in_phi->setMaximum(360);
    label_phong_in_theta = new QLabel("入射光θ",this);
    spinbox_phong_in_theta = new QSpinBox(this);
    spinbox_phong_in_theta->setMaximum(90);
    label_phong_order = new QLabel("散射函数阶数",this);
    spinbox_phong_order = new QSpinBox(this);
    button_calc_phong_map = new QPushButton("计算反射强度图",this);
    button_phong_import = new QPushButton("导入Phong计算数据",this);
    button_phong_import->setEnabled(false);

    phong_texture = new QImage(360,91,QImage::Format_RGB32);
    phong_texture->fill(Qt::red);

    label_phong_calculated_image = new QLabel(this);
    label_phong_calculated_image->setPixmap(QPixmap::fromImage(*phong_texture));

    label_meter_re_phi = new QLabel("入射光Φ",this);
    spinbox_meter_re_phi = new QSpinBox(this);
    spinbox_meter_re_phi->setMaximum(360);
    label_meter_re_theta = new QLabel("入射光θ",this);
    spinbox_meter_re_theta = new QSpinBox(this);
    spinbox_meter_re_theta->setMaximum(90);
    button_calc_meter_map = new QPushButton("测量并计算反射强度图",this);
    button_meter_import = new QPushButton("导入测量数据",this);

    meter_texture = new QImage(360,91,QImage::Format_RGB32);
    meter_texture->fill(Qt::red);

    label_meter_calculated_image = new QLabel(this);
    label_meter_calculated_image->setPixmap(QPixmap::fromImage(*meter_texture));

    groupbox_phong = new QGroupBox("Phong Lighting Model", this);
    groupbox_reflectometer = new QGroupBox("Reflectometer Data", this);

    main_layout = new QHBoxLayout(this);
    controller_layout = new QVBoxLayout();
    groupbox_phong_layout = new QVBoxLayout();
    groupbox_reflectometer_layout = new QVBoxLayout();

    groupbox_phong->setLayout(groupbox_phong_layout);
    groupbox_reflectometer->setLayout(groupbox_reflectometer_layout);

    controller_layout->addWidget(chkbox_ray_visibility);
    controller_layout->addWidget(chkbox_sphere_visibility);
    controller_layout->addWidget(chkbox_do_displacement);
    controller_layout->addWidget(groupbox_phong);
    controller_layout->addWidget(groupbox_reflectometer);

    main_layout->addWidget(gl_widget);
    main_layout->addLayout(controller_layout);
    main_layout->setSpacing(10);
    this->setLayout(main_layout);

    groupbox_phong_layout->addWidget(label_phong_in_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(spinbox_phong_in_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(label_phong_in_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(spinbox_phong_in_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(label_phong_order,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(spinbox_phong_order,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(button_calc_phong_map,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(label_phong_calculated_image,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_layout->addWidget(button_phong_import,0,Qt::AlignLeft | Qt::AlignTop);

    groupbox_reflectometer_layout->addWidget(label_meter_re_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(spinbox_meter_re_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(label_meter_re_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(spinbox_meter_re_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(button_calc_meter_map,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(label_meter_calculated_image,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(button_meter_import,0,Qt::AlignLeft | Qt::AlignTop);

    connect(chkbox_ray_visibility,SIGNAL(stateChanged(int)), this, SLOT(onChkboxRayVisibilityStateChanged(int)));
    connect(chkbox_sphere_visibility,SIGNAL(stateChanged(int)), this, SLOT(onChkboxSphereVisibilityStateChanged(int)));
    connect(chkbox_do_displacement,SIGNAL(stateChanged(int)), this, SLOT(onChkboxDoDisplacementStateChanged(int)));
    connect(this,SIGNAL(signalSetGLWidgetRayState(bool)), gl_widget, SLOT(slotSetGLWidgetRayState(bool)));
    connect(this,SIGNAL(signalSetGLWidgetSphereState(bool)), gl_widget, SLOT(slotSetGLWidgetSphereState(bool)));
    connect(button_calc_phong_map, SIGNAL(clicked(bool)), this, SLOT(onButtonCalcPhongMapClicked()));
    connect(button_phong_import, SIGNAL(clicked(bool)), this, SLOT(onButtonPhongImportClicked()));
    connect(this,SIGNAL(signalPhongImport(QImage*,float,float)), gl_widget, SLOT(slotSetSphereTexture(QImage*,float,float)));
}

void MainWindow::onChkboxRayVisibilityStateChanged(int state){
    emit signalSetGLWidgetRayState(static_cast<bool>(state));
}

void MainWindow::onChkboxSphereVisibilityStateChanged(int state){
    emit signalSetGLWidgetSphereState(static_cast<bool>(state));
}

void MainWindow::onButtonCalcPhongMapClicked(){
    float in_theta = qDegreesToRadians(spinbox_phong_in_theta->value());
    float in_phi = qDegreesToRadians(spinbox_phong_in_phi->value());
    QVector3D in_ray(sin(in_theta)*cos(in_phi),cos(in_theta),sin(in_theta)*sin(in_phi));
    for(int out_theta = 90; out_theta>=0; --out_theta){
        for(int out_phi = 0; out_phi < 360; ++ out_phi){
            float out_theta_rad = qDegreesToRadians(out_theta);
            float out_phi_rad = qDegreesToRadians(out_phi);
            QVector3D out_ray(sin(out_theta_rad)*cos(out_phi_rad),cos(out_theta_rad),sin(out_theta_rad)*sin(out_phi_rad));
            QVector3D half = in_ray + out_ray;
            half.normalize();
            float brightness = pow(QVector3D::dotProduct(half, QVector3D(0,1,0)),spinbox_phong_order->value());
            phong_texture->setPixelColor(out_phi,out_theta,QColor(static_cast<int>(255*brightness),static_cast<int>(255*brightness),static_cast<int>(255*brightness)));
        }
    }
    label_phong_calculated_image->setPixmap(QPixmap::fromImage(*phong_texture));
    button_phong_import->setEnabled(true);
}

void MainWindow::onButtonPhongImportClicked(){
    emit signalPhongImport(phong_texture, qDegreesToRadians(spinbox_phong_in_theta->value()), qDegreesToRadians(spinbox_phong_in_phi->value()));
}

void MainWindow::onChkboxDoDisplacementStateChanged(int state){
    gl_widget->setDoDisplacement(static_cast<bool>(state));
}
