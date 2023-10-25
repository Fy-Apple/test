#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化控制器
    controller = new Controller(this);

    //绑定值改变时的函数
    connect(ui->channel2, SIGNAL(valueChanged(int)), this, SLOT(lightValueChanged(int)));
    connect(ui->channelSpin2, SIGNAL(valueChanged(int)), this, SLOT(lightValueChanged(int)));

    // 获取所有已连接的串口设备信息
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    // 遍历串口设备列表，将它们添加到ComboBox中
    for (const QSerialPortInfo &portInfo : availablePorts)
    {
        ui->serialPort->addItem(portInfo.portName());
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//串口的打开与关闭
void MainWindow::on_openSerialPort_clicked()
{
    if(!controller->isOpen){
        //未打开串口状态
        QString serialPortName = ui->serialPort->currentText();

        if(controller->connectTo(serialPortName)){
            //成功打开串口
            controller->isOpen = true;
            ui->textEdit->append("连接设备成功！");

            controller->turnOn(2);
            controller->setValue(2,0);

            btnSetEnabled(true);
            ui->openSerialPort->setText("关闭串口");
            QMessageBox::information(this, "串口连接", "连接设备成功！");
        }
        else{
            //未成功打开串口
            controller->isOpen = false;
            ui->textEdit->append("连接设备失败！");
            QMessageBox::critical(this, "串口连接", "连接设备失败！");
        }
    }
    else{
        //已经打开串口状态,点击关闭所有通道，并关闭串口
        controller->isOpen = false;
        controller->destroy();
        btnSetEnabled(false);
        ui->openSerialPort->setText("打开串口");
        ui->textEdit->append("成功断开连接！");
        QMessageBox::critical(this, "串口连接", "关闭设备成功！");
    }

}
//设置按钮状态
void MainWindow::btnSetEnabled(bool flag)
{
    ui->channel1->setEnabled(flag);
    ui->channelOpen1->setEnabled(flag);
    ui->channelSpin1->setEnabled(flag);

    ui->channel2->setEnabled(flag);
    ui->channelOpen2->setEnabled(flag);
    ui->channelSpin2->setEnabled(flag);

    ui->channel3->setEnabled(flag);
    ui->channelOpen3->setEnabled(flag);
    ui->channelSpin3->setEnabled(flag);

    ui->channel4->setEnabled(flag);
    ui->channelOpen4->setEnabled(flag);
    ui->channelSpin4->setEnabled(flag);
}

//重新扫描
void MainWindow::on_scanSerialPort_clicked()
{
    ui->serialPort->clear();

    // 获取所有已连接的串口设备信息
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    if(!availablePorts.empty()){
        ui->openDetail->setEnabled(true);
        // 遍历串口设备列表，将它们添加到ComboBox中
        for (const QSerialPortInfo &portInfo : availablePorts)
        {
            ui->serialPort->addItem(portInfo.portName());
        }
    }else{
        ui->openDetail->setEnabled(false);
    }

}

//详细信息
void MainWindow::on_openDetail_clicked()
{
    //获取所有可用串口信息
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : availablePorts)
    {
        if (portInfo.portName() == ui->serialPort->currentText())
        {
            // 找到了特定名称的串口，输出详细信息
            qDebug() << "端口名称: " << portInfo.portName();
            qDebug() << "制造商: " << portInfo.manufacturer();
            qDebug() << "描述: " << portInfo.description();
            qDebug() << "系统位置: " << portInfo.systemLocation();
            qDebug() << "供应商 ID: " << portInfo.vendorIdentifier();
            qDebug() << "产品 ID: " << portInfo.productIdentifier();
            break; // 找到后退出循环
        }
    }

}

//通道2按钮单击
void MainWindow::on_channelOpen2_clicked()
{
    if(controller->channel2){
        //通道2是打开的,关闭
        controller->setValue(2,0);
        controller->turnOff(2);
        controller->channel2 = false;
        ui->channelOpen2->setText("ON");
        ui->textEdit->append("通道2关闭成功");
    }
    else{
        //通道2是关闭的，打开
        int value = ui->channelSpin2->text().toInt();
        controller->turnOn(2);
        controller->setValue(2,value);
        controller->channel2 = true;
        ui->channelOpen2->setText("OFF");
        ui->textEdit->append("通道2打开成功，光源为" + QString::number(value));
    }
}

//滑动条改变时改变灯光亮度
void MainWindow::lightValueChanged(int value)
{
    ui->channel2->setValue(value);
    ui->channelSpin2->setValue(value);
    //若通道未打开
    if(!controller->channel2){
        controller->channel2 = true;
        ui->channelOpen2->setText("OFF");
        controller->turnOn(2);
        controller->setValue(2,value);
    }
    else{
        controller->setValue(2,value);
    }

}



