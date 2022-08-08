#include "reflectometer.h"

Reflectometer::Reflectometer(QString port_name, QObject *parent)
    : QObject{parent}
{
    serial = new QSerialPort(port_name, this);
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    intensity_buffer = new float[360*90];
}

template<typename QEnum>
QString QtEnumToString (const QEnum value)
{
  return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

bool Reflectometer::initialize()
{
    bool is_opened = serial->open(QIODeviceBase::ReadWrite);
    if(!is_opened)
    {
        _error = "串口打开失败 - "+QtEnumToString(serial->error())+"。";
        return false;
    }
    serial->write("HOLA\n");
    if(!serial->waitForBytesWritten(TIME_OUT)){
        _error = "串口数据写入失败 - "+QtEnumToString(serial->error())+"。";
        return false;
    }
    if(serial->waitForReadyRead(TIME_OUT)){
        rx_tmp = serial->readLine();
        if(rx_tmp.trimmed() == "GNRM"){
            return true;
        }else{
            _error = "握手未成功。";
            return false;
        }
    }
    _error = "串口数据读取失败 - "+QtEnumToString(serial->error())+"。";
    return false;
}

bool Reflectometer::setLaser(int theta, int phi){
    serial->write(QString("MOVL "+QString::number(theta)+" "+QString::number(phi) + "\n").toStdString().c_str());
    if(!serial->waitForBytesWritten(TIME_OUT)){
        _error = "串口数据写入失败 - "+QtEnumToString(serial->error())+"。";
        return false;
    }
    if(serial->waitForReadyRead(TIME_OUT)){

        rx_tmp = serial->readLine();
        /*
        while(serial->waitForReadyRead(0)){
            rx_tmp = serial->readLine();
        }
        */

        QStringList rx_list = rx_tmp.split(' ', Qt::SkipEmptyParts);
        if(rx_list.length()!=3 || rx_list[0] != "SUCL"){
            _error = "接收到无效数据。";
            return false;
        }else if(rx_list[1].toInt() != theta || rx_list[2].toInt() != phi){
            _error = "接收到的角度和发送角度不匹配。";
            return false;
        }else{
            return true;
        }
    }
    _error = "串口数据读取失败 - "+QtEnumToString(serial->error())+"。";
    return false;
}

bool Reflectometer::getIntensityInfo(int theta, int phi){
    serial->write(QString("MOVS "+QString::number(theta)+" "+QString::number(phi) + "\n").toStdString().c_str());
    if(!serial->waitForBytesWritten(TIME_OUT)){
        _error = "串口数据写入失败 - "+QtEnumToString(serial->error())+"。";
        return false;
    }
    if(serial->waitForReadyRead(TIME_OUT)){

        rx_tmp = serial->readLine();
        /*
        while(serial->waitForReadyRead(0)){
            rx_tmp = serial->readLine();
        }
        */

        QStringList rx_list = rx_tmp.split(' ', Qt::SkipEmptyParts);
        if(rx_list.length()!=3 || rx_list[0] != "SUCS"){
            _error = "接收到无效数据。";
            return false;
        }else if(rx_list[1].toInt() != theta || rx_list[2].toInt() != phi){
            _error = "接收到的角度和发送角度不匹配。";
            return false;
        }else{
            serial->write("GETI\n");
            if(!serial->waitForBytesWritten(TIME_OUT)){
                _error = "串口数据写入失败 - "+QtEnumToString(serial->error())+"。";
                return false;
            }
            if(serial->waitForReadyRead(TIME_OUT)){

                rx_tmp = serial->readLine();
                /*
                while(serial->waitForReadyRead(0)){
                    rx_tmp = serial->readLine();
                }
                */

                QStringList rx_list = rx_tmp.split(' ', Qt::SkipEmptyParts);
                if(rx_list.length()!=2 || rx_list[0] != "SUCI"){
                    _error = "接收到无效数据。";
                    qDebug()<<serial->readLine();
                    return false;
                }else{
                    last_intensity = rx_list[1].toFloat();
                    intensity_buffer[theta*91+phi] = rx_list[1].toInt();
                    return true;
                }
            }
            _error = "串口数据读取失败 - "+QtEnumToString(serial->error())+"。";
            return false;
        }
    }
    _error = "串口数据读取失败 - "+QtEnumToString(serial->error())+"。";
    return false;
}

QString Reflectometer::error(){
    return _error;
}

Reflectometer::~Reflectometer(){
    delete intensity_buffer;
    serial->close();
}

float Reflectometer::getLastIntensity() const
{
    return last_intensity;
}

void Reflectometer::flush()
{
    serial->flush();
}
