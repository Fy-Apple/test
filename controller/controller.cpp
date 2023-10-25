#include "controller.h"

#include <QDebug>

Controller::Controller(QObject *parent)
    : QObject{parent}
{

}

Controller::~Controller()
{

}

//打开串口
bool Controller::connectTo(const QString &com)
{
    // 在连接前先断开
    destroy();

    serialPort.setPortName(com);
    serialPort.setBaudRate(QSerialPort::Baud9600);//波特率
    serialPort.setDataBits(QSerialPort::Data8);//数据位
    serialPort.setParity(QSerialPort::NoParity);//奇偶校验
    serialPort.setStopBits(QSerialPort::OneStop);//停止位
    //serialPort.setWriteTimeout(1000);

    if (serialPort.open(QIODevice::ReadWrite))
    {
        qDebug() << "串口已打开";
        return true;
    }
    qDebug() << "无法打开串口:" << serialPort.errorString();
    return false;
}


//打开特定通道的光源
void Controller::turnOn(int channel)
{
    commandBuffer.clear();
    commandBuffer.append("$");//特征字
    commandBuffer.append("1");//1表示打开通道电源
    commandBuffer.append(QString::number(channel));//通道
    commandBuffer.append("000");//数据

    // 发送打开命令
    serialPort.write(xorCheck(commandBuffer).toUtf8());

    // 已发送数据
    commandSent = true;

}

//设置光源
void Controller::setValue(int channel, int intensity)
{
    commandBuffer.clear();
    commandBuffer.append("$");//特种字
    commandBuffer.append("3");//3表示设置参数
    commandBuffer.append(QString::number(channel));//通道

    QString intensityHex = QString::number(intensity, 16).rightJustified(3, '0');
    commandBuffer.append(intensityHex);//亮度


    qDebug() << xorCheck(commandBuffer).toUtf8();
    // 发送设置亮度命令
    serialPort.write(xorCheck(commandBuffer).toUtf8());

}


//关闭光源
void Controller::turnOff(int channel)
{
    commandBuffer.clear();
    commandBuffer.append("$");//特征字
    commandBuffer.append("2");//2表示关闭通道电源
    commandBuffer.append(QString::number(channel));//通道号
    commandBuffer.append("000");//数据

    // 发送关闭命令
    serialPort.write(xorCheck(commandBuffer).toUtf8());

    // 已发送数据
    commandSent = true;

}

//断开串口连接关闭串口
void Controller::destroy()
{
    commandBuffer.clear();
    commandSent = false;
    serialPort.close();

}

//关闭所有通道电源
void Controller::turnOffAll()
{
    turnOff(1);
    turnOff(2);
    turnOff(3);
    turnOff(4);

}

//超时槽函数
void Controller::onTimeout()
{
    if (commandSent)
    {
        // 处理命令执行状态，例如检查串口是否有响应或其他情况
        // 根据实际情况进行处理
        // 如果命令执行完成，可以停止定时器
        commandSent = false;
    }

}


QString Controller::xorCheck(const QString &str)
{
    //获取s应字节数组
    QByteArray byteArray = str.toUtf8();
    // xorResult 存放校验结注意：初值首元素值
    char xorResult = byteArray[0];
    // 求xor校验注意：XOR运算第二元素始
    for (int i = 1; i < byteArray.size(); i++)
    {
        xorResult ^= byteArray[i];
    }
    // 运算xorResultXOR校验结果
    return str + QString::number(xorResult, 16).rightJustified(2, '0');

}
