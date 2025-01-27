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

    ui->dirToSave->setText(windLogger.getPath());

    ui->chooseSpeed->setCurrentIndex(1);
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

    QRegularExpression regex(R"(\$([\d.,]+),([\d.,]+)\r\n)");
    QRegularExpressionMatch match = regex.match(message);

    if (match.hasMatch()) {
        QString windSpeed = match.captured(1);
        QString windDirection = match.captured(2);
        QString time = QDateTime::currentDateTime().toString(Qt::ISODate);

        QString outputMessage = QString("Время: %1\n"
                                        "Скорость ветра (среднее значение): %2 м/с\n"
                                        "Направление ветра (среднее значение): %3 °\n")
                                    .arg(time)
                                    .arg(windSpeed)
                                    .arg(windDirection);

        QJsonObject json = this->windLogger.createJsonMessage(time, "WMT700", windSpeed, windDirection);
        this->windLogger.appendJsonToFile(json);

        ui->outputCOM->setText(ui->outputCOM->toPlainText() + outputMessage);


        ui->outputStatus->setText(ui->outputStatus->toPlainText() + message);

        QTextCursor cursorOutput = ui->outputCOM->textCursor();
        cursorOutput.movePosition(QTextCursor::End);
        ui->outputCOM->setTextCursor(cursorOutput);
    } else {
        QString outputMessage = "Ошибка: данные не соответствуют ожидаемому формату.\n";

        ui->outputStatus->setText(ui->outputStatus->toPlainText() + outputMessage);
    }

    QTextCursor cursorStatus = ui->outputStatus->textCursor();
    cursorStatus.movePosition(QTextCursor::End);
    ui->outputStatus->setTextCursor(cursorStatus);
}


void MainWindow::on_dirToSave_returnPressed()
{
    QString newFilePath = ui->dirToSave->text();
    if(!windLogger.setPath(newFilePath)){
        ui->outputStatus->setText(ui->outputStatus->toPlainText() + newFilePath + " не существует\n");
        QTextCursor cursorStatus = ui->outputStatus->textCursor();
        cursorStatus.movePosition(QTextCursor::End);
        ui->outputStatus->setTextCursor(cursorStatus);
    }
    ui->dirToSave->setText(windLogger.getPath());
}



void MainWindow::on_chooseCOM_currentIndexChanged(int index)
{
    if(serialPort.isOpen())
        serialPort.close();
    serialPort.setPortName("COM" + QString::number(index+1));
    serialPort.open(QIODevice::ReadWrite);
}


void MainWindow::on_chooseSpeed_currentIndexChanged(int index)
{
    qint32 baudRate = serialPort.baudRate();

    switch (index) {
    case 0:
        baudRate = 1200;
        break;
    case 1:
        baudRate = 2400;
        break;
    case 2:
        baudRate = 4800;
        break;
    case 3:
        baudRate = 9600;
        break;
    }

    serialPort.setBaudRate(baudRate);
    qDebug() << serialPort.baudRate();
}

