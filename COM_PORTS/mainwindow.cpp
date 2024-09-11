#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Тестовое задание. COM-порт");
    setFixedSize(600, 400);

    initComboBox();
    initComPort();

    ui->chooseByte->setCurrentIndex(3);
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
}

MainWindow::~MainWindow()
{
    if(serialPort.isOpen())
        serialPort.close();
    delete ui;
}

void MainWindow::initComboBox() {
    for(int i=0; i<9;i++){
        portMap["COM" + QString::number(i+1)] = i;
    }
}

void MainWindow::initComPort(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    serialPort.setBaudRate(QSerialPort::Baud2400);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    for(const QSerialPortInfo& port : ports){
        serialPort.setPortName(port.portName());
        if (serialPort.open(QIODevice::ReadWrite))
            break;
    }
    if(!serialPort.isOpen()){
        QMessageBox::information(nullptr, "Нет доступных COM-портов", "Выбран по умолчанию COM1");
        ui->chooseCOM->setCurrentIndex(portMap["COM1"]);
    }
    else
        ui->chooseCOM->setCurrentIndex(portMap[serialPort.portName()]);
    ui->outputStatus->setText(ui->outputStatus->toPlainText() + "Скорость: " + QString::number(serialPort.baudRate()) + "\n");
}

void MainWindow::readData()
{
    QByteArray data = serialPort.readAll();
    QString message = QString::fromUtf8(data);

    ui->outputCOM->setText(ui->outputCOM->toPlainText() + message);
    QTextCursor cursorOutput = ui->outputCOM->textCursor();
    cursorOutput.movePosition(QTextCursor::End);
    ui->outputCOM->setTextCursor(cursorOutput);

    ui->outputStatus->setText(ui->outputStatus->toPlainText() + "Передано: " + QString::number(data.size()) + " байт\n");
    QTextCursor cursorStatus = ui->outputStatus->textCursor();
    cursorStatus.movePosition(QTextCursor::End);
    ui->outputStatus->setTextCursor(cursorStatus);
}

void MainWindow::on_inputCOM_returnPressed()
{
    serialPort.write((ui->inputCOM->text() + "\n").toUtf8());
    ui->inputCOM->clear();
}


void MainWindow::on_chooseCOM_currentIndexChanged(int index)
{
    if(serialPort.isOpen())
        serialPort.close();
    serialPort.setPortName("COM" + QString::number(index+1));
    serialPort.open(QIODevice::ReadWrite);
}


void MainWindow::on_chooseByte_currentIndexChanged(int index)
{
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    switch (index) {
    case 0:
        dataBits = QSerialPort::Data5;
        break;
    case 1:
        dataBits = QSerialPort::Data6;
        break;
    case 2:
        dataBits = QSerialPort::Data7;
        break;
    }
    serialPort.setDataBits(dataBits);
}

