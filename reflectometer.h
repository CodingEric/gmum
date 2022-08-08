#ifndef SERIALHELPER_H
#define SERIALHELPER_H

#include <QObject>

#include <QtSerialPort>

#define TIME_OUT 1000

enum ScalingLevel{
    SCALEING6X,
    SCALING5x,
    SCALING3x,
    SCALING2x,
    NOSCALING
};

class Reflectometer : public QObject
{
    Q_OBJECT
public:
    explicit Reflectometer(QString port_name, QObject *parent = nullptr);

    bool initialize();
    bool setLaser(int theta, int phi);
    bool getIntensityInfo(int theta, int phi);
    float getLastIntensity() const;
    void flush();
    QString error();
    ~Reflectometer();

private:
    QSerialPort* serial;
    QString _error;
    QString rx_tmp;
    float last_intensity;
    float* intensity_buffer;

signals:

};

#endif // SERIALHELPER_H
