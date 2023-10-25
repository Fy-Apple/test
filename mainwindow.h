#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Controller.h>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openSerialPort_clicked();

    void on_scanSerialPort_clicked();

    void on_openDetail_clicked();

    void on_channelOpen2_clicked();

    //同时更新滑动条和小三角的更新
    void lightValueChanged(int value);

private:
    Ui::MainWindow *ui;
    Controller* controller;
    void btnSetEnabled(bool flag);


};
#endif // MAINWINDOW_H
