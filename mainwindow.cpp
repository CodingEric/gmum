#include "mainwindow.h"

MainWindow::MainWindow()
{
    this->resize(800,800);



    main_layout = new QHBoxLayout(this);
    main_layout->setSpacing(10);
    this->setLayout(main_layout);

    gl_widget = new GLWidget(this);
    gl_widget -> setMinimumWidth(600);
    main_layout->addWidget(gl_widget);

    controller_layout = new QVBoxLayout();
    main_layout->addLayout(controller_layout);


// BEGIN Viewport Control

    chkbox_ray_visibility = new QCheckBox("显示光线和法线", this);
    chkbox_ray_visibility->setCheckState(Qt::Checked);
    chkbox_ray_visibility->setTristate(false);
    chkbox_sphere_visibility = new QCheckBox("显示反射球", this);
    chkbox_sphere_visibility->setCheckState(Qt::Checked);
    chkbox_sphere_visibility->setTristate(false);
    chkbox_do_displacement = new QCheckBox("启用置换", this);
    chkbox_do_displacement->setCheckState(Qt::Checked);
    chkbox_do_displacement->setTristate(false);
    chkbox_do_gradient = new QCheckBox("启用渐变色可视化辅助", this);
    chkbox_do_gradient->setCheckState(Qt::Checked);
    chkbox_do_gradient->setTristate(false);
    controller_layout->addWidget(chkbox_ray_visibility);
    controller_layout->addWidget(chkbox_sphere_visibility);
    controller_layout->addWidget(chkbox_do_displacement);
    controller_layout->addWidget(chkbox_do_gradient);

// END Viewport Control

// BEGIN Phong Lighting Model Simulation

    groupbox_phong = new QGroupBox("Phong光照模型仿真", this);
    groupbox_phong_main_layout = new QVBoxLayout();
    groupbox_phong_secondary_layout = new QHBoxLayout();
    groupbox_phong_sublayout1 = new QVBoxLayout();
    groupbox_phong_sublayout2 = new QVBoxLayout();

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

    label_ka = new QLabel("环境光强度系数", this);
    spinbox_ka = new QDoubleSpinBox(this);
    spinbox_ka->setValue(0.1);
    spinbox_ka->setMaximum(1);
    spinbox_ka->setMinimum(0);
    spinbox_ka->setSingleStep(0.1);
    spinbox_ka->setDecimals(4);
    label_kd = new QLabel("散射光强度系数", this);
    spinbox_kd = new QDoubleSpinBox(this);
    spinbox_kd->setValue(0.1);
    spinbox_kd->setMaximum(1);
    spinbox_kd->setMinimum(0);
    spinbox_kd->setSingleStep(0.1);
    spinbox_kd->setDecimals(4);
    label_ks = new QLabel("各向异性反射强度系数", this);
    spinbox_ks = new QDoubleSpinBox(this);
    spinbox_ks->setValue(0.4);
    spinbox_ks->setMaximum(1);
    spinbox_ks->setMinimum(0);
    spinbox_ks->setSingleStep(0.1);
    spinbox_ks->setDecimals(4);

    groupbox_phong->setLayout(groupbox_phong_main_layout);
    groupbox_phong_main_layout ->addLayout(groupbox_phong_secondary_layout);
    groupbox_phong_main_layout->addWidget(button_calc_phong_map,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_main_layout->addWidget(label_phong_calculated_image,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_main_layout->addWidget(button_phong_import,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_secondary_layout->addLayout(groupbox_phong_sublayout1);
    groupbox_phong_secondary_layout->addLayout(groupbox_phong_sublayout2);

    groupbox_phong_sublayout1->addWidget(label_phong_in_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout1->addWidget(spinbox_phong_in_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout1->addWidget(label_phong_in_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout1->addWidget(spinbox_phong_in_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout1->addWidget(label_phong_order,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout1->addWidget(spinbox_phong_order,0,Qt::AlignLeft | Qt::AlignTop);

    groupbox_phong_sublayout2->addWidget(label_ka,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout2->addWidget(spinbox_ka,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout2->addWidget(label_kd,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout2->addWidget(spinbox_kd,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout2->addWidget(label_ks,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_phong_sublayout2->addWidget(spinbox_ks,0,Qt::AlignLeft | Qt::AlignTop);

// END Phong Lighting Model Simulation

// BEGIN Gonioreflectometer Control

    groupbox_reflectometer = new QGroupBox("反射计数据", this);
    groupbox_reflectometer_layout = new QVBoxLayout();
    groupbox_reflectometer->setLayout(groupbox_reflectometer_layout);

    label_com_ports = new QLabel("Gonioreflectometer 所在 COM 口", this);
    combobox_com_ports = new QComboBox(this);
    button_update_com_ports = new QPushButton("更新 COM 列表", this);

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

    groupbox_reflectometer_layout->addWidget(label_com_ports,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(combobox_com_ports,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(button_update_com_ports,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(label_meter_re_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(spinbox_meter_re_phi,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(label_meter_re_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(spinbox_meter_re_theta,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(button_calc_meter_map,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(label_meter_calculated_image,0,Qt::AlignLeft | Qt::AlignTop);
    groupbox_reflectometer_layout->addWidget(button_meter_import,0,Qt::AlignLeft | Qt::AlignTop);

// END Gonioreflectometer Control

    logger = new QTextEdit(this);
    logger->setMaximumWidth(320);
    logger->setMinimumWidth(320);
    logger->setReadOnly(true);
    logger->append("Gonioreflectometer 硬件通信日志窗口");
    logger->setStyleSheet("QTextEdit { background-color: blue; color: white; padding: 5px; font-family: Consolas, Source Han Sans CN; }");

    main_layout->addWidget(logger);

    controller_layout->addWidget(groupbox_phong);
    controller_layout->addWidget(groupbox_reflectometer);

    phong_color_info = {0,0};
    updateCOMPorts();

    connect(chkbox_ray_visibility,SIGNAL(stateChanged(int)), this, SLOT(onChkboxRayVisibilityStateChanged(int)));
    connect(chkbox_sphere_visibility,SIGNAL(stateChanged(int)), this, SLOT(onChkboxSphereVisibilityStateChanged(int)));
    connect(chkbox_do_displacement,SIGNAL(stateChanged(int)), this, SLOT(onChkboxDoDisplacementStateChanged(int)));
    connect(chkbox_do_gradient,SIGNAL(stateChanged(int)), this, SLOT(onChkboxDoGradientStateChanged(int)));
    connect(button_calc_phong_map, SIGNAL(clicked(bool)), this, SLOT(onButtonCalcPhongMapClicked()));
    connect(button_phong_import, SIGNAL(clicked(bool)), this, SLOT(onButtonPhongImportClicked()));
    connect(button_update_com_ports,&QPushButton::clicked, [=](){updateCOMPorts();});
    connect(button_calc_meter_map, SIGNAL(clicked(bool)), this, SLOT(onButtonCalcMeterMapClicked()));
}

void MainWindow::updateCOMPorts(){
    QList<QSerialPortInfo> list;
    list = QSerialPortInfo::availablePorts();
    combobox_com_ports->clear();
    for(int i=0;i<list.size();++i){
        combobox_com_ports->addItem(list.at(i).portName());
    }
    if(list.size()==0){
        button_calc_meter_map->setEnabled(false);
    }
}

void MainWindow::onChkboxRayVisibilityStateChanged(int state){
    gl_widget -> setGLWidgetRayState(static_cast<bool>(state));
}

void MainWindow::onChkboxSphereVisibilityStateChanged(int state){
    gl_widget -> setGLWidgetSphereState(static_cast<bool>(state));
}

void MainWindow::onChkboxDoGradientStateChanged(int state){
    gl_widget -> setDoGradient(static_cast<bool>(state));
}



void MainWindow::onButtonCalcMeterMapClicked(){
    addLog("正在初始化反射计连接...");

    reflectometer = new Reflectometer(combobox_com_ports->currentText(),this);
    if(reflectometer->initialize()){
        addLog("反射计连接建立成功。");
        addLog("正在设置入射光参数...");

        if(!reflectometer->setLaser(spinbox_meter_re_theta->value(), spinbox_meter_re_phi->value())){
            addLog(reflectometer->error(), LogLevel::Error);
            QMessageBox msgbox(QMessageBox::Critical,"错误", "错误信息: "+reflectometer->error());
            msgbox.exec();
            delete reflectometer;
            return;
        }

        addLog("设置入射光参数成功。");

        for(int theta=0;theta<90;++theta){
            for(int phi=0;phi<360;++phi){
                for(int trials = 0; trials < 5; ++trials){
                    reflectometer->flush();
                    if(reflectometer->getIntensityInfo(theta, phi)){
                        addLog(QString("已在第 %0 次尝试中获取到 theta = %1, phi = %2 的光强数据为 %3").arg(trials+1).arg(theta).arg(phi).arg(reflectometer->getLastIntensity(), 0, 'f', 3));
                        meter_texture -> setPixelColor(phi,theta,QColor(255*reflectometer->getLastIntensity(),255*reflectometer->getLastIntensity(),255*reflectometer->getLastIntensity()));
                        label_meter_calculated_image->setPixmap(QPixmap::fromImage(*meter_texture));
                        break;
                    }else{
                        qDebug()<<QString("第 %0 次尝试获取 theta = %1, phi = %2 的光强数据出现错误，稍后重试 - ").arg(trials+1).arg(theta).arg(phi) + reflectometer->error();
                        addLog(QString("第 %0 次尝试获取 theta = %1, phi = %2 的光强数据出现错误，稍后重试 - ").arg(trials+1).arg(theta).arg(phi) + reflectometer->error(), LogLevel::Error);
                        if(trials == 4){
                            addLog(QString("在第 %0 次尝试中依然无法获取到 theta = %1, phi = %2 的光强数据。本次取数过程中止。").arg(trials+1).arg(theta).arg(phi), LogLevel::Error);
                            QMessageBox msgbox(QMessageBox::Critical,"错误", "错误信息: "+reflectometer->error());
                            msgbox.exec();
                            delete reflectometer;
                            return;
                        }
                    }
                }
            }
        }
    }else{
        addLog(reflectometer->error(), LogLevel::Error);
        QMessageBox msgbox(QMessageBox::Critical,"错误", "错误信息: "+reflectometer->error());
        msgbox.exec();
    }
    delete reflectometer;
}


double mapRange(double a1,double a2,double b1,double b2,double s) {
  return b1 + (s-a1)*(b2-b1)/(a2-a1);
}

void MainWindow::onButtonCalcPhongMapClicked(){
    float in_theta = qDegreesToRadians(spinbox_phong_in_theta->value());
    float in_phi = qDegreesToRadians(spinbox_phong_in_phi->value());
    QVector3D in_ray(sin(in_theta)*cos(in_phi),cos(in_theta),sin(in_theta)*sin(in_phi));
    float brightness_min = spinbox_ka->value() + spinbox_kd->value() * QVector3D::dotProduct(in_ray, QVector3D(0, 1, 0));
    float brightness_max = spinbox_ka->value() + spinbox_kd->value() * QVector3D::dotProduct(in_ray, QVector3D(0, 1, 0)) + spinbox_ks->value();
    for(int out_theta = 90; out_theta>=0; --out_theta){
        for(int out_phi = 0; out_phi < 360; ++out_phi){
            float out_theta_rad = qDegreesToRadians(out_theta);
            float out_phi_rad = qDegreesToRadians(out_phi);
            QVector3D out_ray(sin(out_theta_rad)*cos(out_phi_rad),cos(out_theta_rad),sin(out_theta_rad)*sin(out_phi_rad));
            QVector3D half = in_ray + out_ray;
            half.normalize();
            float brightness = spinbox_ks->value() * pow(QVector3D::dotProduct(half, QVector3D(0,1,0)),spinbox_phong_order->value()) + spinbox_ka->value() + spinbox_kd->value() * QVector3D::dotProduct(in_ray, QVector3D(0, 1, 0));
            brightness = mapRange(0,brightness_max,0,1,brightness);
            phong_texture->setPixelColor(out_phi,out_theta,QColor(static_cast<int>(255*brightness),static_cast<int>(255*brightness),static_cast<int>(255*brightness)));
        }
    }

    phong_color_info = {brightness_min, brightness_max};

    label_phong_calculated_image->setPixmap(QPixmap::fromImage(*phong_texture));
    button_phong_import->setEnabled(true);
}

void MainWindow::onButtonPhongImportClicked(){
    gl_widget -> setSphereTexture(phong_texture, qDegreesToRadians(spinbox_phong_in_theta->value()), qDegreesToRadians(spinbox_phong_in_phi->value()), phong_color_info);
}

void MainWindow::onChkboxDoDisplacementStateChanged(int state){
    gl_widget->setDoDisplacement(static_cast<bool>(state));
}

// TODO: remove duplicate
template<typename QEnum>
QString QtEnumToString (QEnum value)
{
  return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

void MainWindow::addLog(QString text, LogLevel log_level){
    logger->append("[" + QDateTime::currentDateTime().toString("HH:mm:ss") + "]["+QtEnumToString(log_level)+"] "+text);
    qApp->processEvents();
}
