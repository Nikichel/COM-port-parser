#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QMap>
#include <QMessageBox>

#include "WindDataLogger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initComPort();
    void initComboBox();

private slots:
    void on_dirToSave_returnPressed();
    void on_chooseCOM_currentIndexChanged(int index);
    void readData();

    void on_chooseSpeed_currentIndexChanged(int index);

private:
    QSerialPort serialPort;
    QMap<QString, int> portMap;
    Ui::MainWindow *ui;

    WindDataLogger windLogger;
};
#endif // MAINWINDOW_H
