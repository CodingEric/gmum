#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtMath>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QtDebug>
#include <QLabel>
#include <QImage>
#include <QVector3D>
#include <QComboBox>
#include <QMessageBox>
#include <QPainter>

#include <QSerialPortInfo>

#include "glwidget.h"
#include "reflectometer.h"


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();

    enum LogLevel{
        Info,
        Warning,
        Error
    };
    Q_ENUM(LogLevel)
private:

    QHBoxLayout* main_layout;
    QVBoxLayout* controller_layout;

    GLWidget* gl_widget;

// BEGIN Viewport Control

    QCheckBox* chkbox_ray_visibility;
    QCheckBox* chkbox_sphere_visibility;
    QCheckBox* chkbox_do_displacement;
    QCheckBox* chkbox_do_gradient;

// END Viewport Control

// BEGIN Phong Lighting Model Simulation

    QGroupBox* groupbox_phong;
    QVBoxLayout* groupbox_phong_main_layout;
    QHBoxLayout* groupbox_phong_secondary_layout;
    QVBoxLayout* groupbox_phong_sublayout1;
    QVBoxLayout* groupbox_phong_sublayout2;

    QLabel* label_phong_in_theta;
    QSpinBox* spinbox_phong_in_theta;
    QLabel* label_phong_in_phi;
    QSpinBox* spinbox_phong_in_phi;
    QLabel* label_phong_order;
    QSpinBox* spinbox_phong_order;

    QLabel* label_ka;
    QDoubleSpinBox* spinbox_ka;
    QLabel* label_kd;
    QDoubleSpinBox* spinbox_kd;
    QLabel* label_ks;
    QDoubleSpinBox* spinbox_ks;

    QPushButton* button_calc_phong_map;
    QLabel* label_phong_calculated_image;
    QPushButton* button_phong_import;

// END Phong Lighting Model Simulation

// BEGIN Gonioreflectometer Control

    QGroupBox* groupbox_reflectometer;
    QVBoxLayout* groupbox_reflectometer_layout;

    QLabel* label_com_ports;
    QComboBox* combobox_com_ports;
    QPushButton* button_update_com_ports;
    QLabel* label_meter_re_phi;
    QSpinBox* spinbox_meter_re_phi;
    QLabel* label_meter_re_theta;
    QSpinBox* spinbox_meter_re_theta;
    QLabel* label_meter_calculated_image;

    QPushButton* button_calc_meter_map;
    QPushButton* button_meter_import;

// END Gonioreflectometer Control

    QTextEdit* logger;

// BEGIN Necessary Elements

    Reflectometer* reflectometer;
    void updateCOMPorts();

    QImage* phong_texture;
    QImage* meter_texture;

    QVector2D phong_color_info;

    void addLog(QString text, LogLevel log_level = LogLevel::Info);

// END Necessary Elements

private slots:
    void onChkboxRayVisibilityStateChanged(int state);
    void onChkboxSphereVisibilityStateChanged(int state);
    void onChkboxDoDisplacementStateChanged(int state);
    void onChkboxDoGradientStateChanged(int state);
    void onButtonCalcPhongMapClicked();
    void onButtonPhongImportClicked();
    void onButtonCalcMeterMapClicked();
};

#endif // MAINWINDOW_H
