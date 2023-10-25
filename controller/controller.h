#ifndef CONTROLLER_H
#define CONTROLLER_H

#include<QObject>
#include <QSerialPort>



class Controller : public QObject
{
     Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    //串口连接
    bool connectTo(const QString &com);

    //打开光源
    void turnOn(int channel);

    //设置光源
    void setValue(int channel, int intensity);

    //关闭光源
    void turnOff(int channel);

    //断开串口连接，关闭串口
    void destroy();

    //关闭所有
    void turnOffAll();

    bool isOpen = false;

    bool channel1 = false;
    bool channel2 = false;
    bool channel3 = false;
    bool channel4 = false;

private slots:
    void onTimeout();

private:
    QSerialPort serialPort;//串口对象
    QString commandBuffer;//指令
    bool commandSent;


    QString xorCheck(const QString &str);
};

#endif // CONTROLLER_H
