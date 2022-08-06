#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QtDebug>
#include <QLabel>
#include <QImage>
#include <QVector3D>
#include <QtMath>

#include "glwidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();
private:
    QHBoxLayout* main_layout;
    QVBoxLayout* controller_layout;

// BEGIN groupbox_phong

    QGroupBox* groupbox_phong;
    QVBoxLayout* groupbox_phong_main_layout;
    QHBoxLayout* groupbox_phong_secondary_layout;
    QVBoxLayout* groupbox_phong_sublayout1;
    QVBoxLayout* groupbox_phong_sublayout2;

// END groupbox_phong

    QVBoxLayout* groupbox_reflectometer_layout;
    GLWidget* gl_widget;
    QCheckBox* chkbox_ray_visibility;
    QCheckBox* chkbox_sphere_visibility;
    QCheckBox* chkbox_do_displacement;

    QGroupBox* groupbox_reflectometer;

    QSpinBox* spinbox_phong_in_theta;
    QSpinBox* spinbox_phong_in_phi;
    QSpinBox* spinbox_phong_order;

    QLabel* label_phong_in_phi;
    QLabel* label_phong_in_theta;
    QLabel* label_phong_order;
    QLabel* label_phong_calculated_image;

    QImage* phong_texture;
    QImage* meter_texture;

    QPushButton* button_calc_phong_map;
    QPushButton* button_phong_import;

    QSpinBox* spinbox_meter_re_theta;
    QSpinBox* spinbox_meter_re_phi;
    QLabel* label_meter_re_phi;
    QLabel* label_meter_re_theta;
    QLabel* label_meter_calculated_image;

    QPushButton* button_calc_meter_map;
    QPushButton* button_meter_import;

// BEGIN groupbox_phong_sublayout2

    QLabel* label_ka;
    QDoubleSpinBox* spinbox_ka;

    QLabel* label_kd;
    QDoubleSpinBox* spinbox_kd;

    QLabel* label_ks;
    QDoubleSpinBox* spinbox_ks;

// END groupbox_phong_sublayout2

private slots:
    void onChkboxRayVisibilityStateChanged(int state);
    void onChkboxSphereVisibilityStateChanged(int state);
    void onChkboxDoDisplacementStateChanged(int state);
    void onButtonCalcPhongMapClicked();
    void onButtonPhongImportClicked();
signals:
    void signalSetGLWidgetRayState(bool is_visible);
    void signalSetGLWidgetSphereState(bool is_visible);
    void signalPhongImport(QImage* texture, float incident_theta, float incident_phi);
};

#endif // MAINWINDOW_H
