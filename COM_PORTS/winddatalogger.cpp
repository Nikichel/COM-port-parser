#include "WindDataLogger.h"
#include <QJsonDocument>
#include <QFile>
#include <QDateTime>
#include <QRegularExpression>
#include <QTextStream>
#include <QDebug>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

WindDataLogger::WindDataLogger(){
    this->m_filePath = QDir::currentPath()+"/Logs";
    //this->fileName = "//wind_logs.json";
    qDebug() << this->m_filePath;
}

WindDataLogger::WindDataLogger(const QString& filePath) {
    this->m_filePath = filePath;
    //this->fileName = QString("/wind_logs.json");
    qDebug() << this->m_filePath;
}

bool WindDataLogger::setPath(const QString& filePath) {
    qDebug() << "Проверяем путь:" << filePath;

    QFileInfo fileInfo(filePath);
    if (fileInfo.exists()) {
        this->m_filePath = filePath;
        qDebug() << "Файл или директория существует.";
        return true; // Путь существует
    } else {
        qDebug() << "Файл или директория не найдены.";
        return false; // Путь не существует
    }
}

QString WindDataLogger::getPath(){
    return this->m_filePath;
}

QJsonObject WindDataLogger::createJsonMessage(const QString& sensorName, const QString& windSpeed, const QString& windDirection) {
    QJsonObject jsonObject;
    jsonObject["time"] = QDateTime::currentDateTime().toString(Qt::ISODate);  // Текущее время
    jsonObject["sensor_name"] = sensorName;
    jsonObject["wind_speed"] = windSpeed;
    jsonObject["wind_direction"] = windDirection;
    return jsonObject;
}

void WindDataLogger::appendJsonToFile(const QJsonObject& jsonObject) {
    QFile file(m_filePath + "/logs_wind.json");

    QJsonArray jsonArray;

    // Проверяем, существует ли файл
    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Не удалось открыть файл для чтения.";
            qWarning() << file.errorString();
            return;
        }

        // Читаем существующее содержимое файла
        QByteArray fileData = file.readAll();
        file.close();

        // Парсим содержимое как JSON
        QJsonDocument existingDoc = QJsonDocument::fromJson(fileData);

        // Проверяем, является ли это массивом
        if (existingDoc.isArray()) {
            jsonArray = existingDoc.array();  // Загружаем существующий массив
        } else {
            qWarning() << "Существующий файл не является массивом JSON.";
        }
    }

    // Добавляем новый объект в массив
    jsonArray.append(jsonObject);

    // Открываем файл для записи (перезапись)
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи.";
        qWarning() << file.errorString();
        return;
    }

    // Преобразуем массив JSON в строку
    QJsonDocument jsonDoc(jsonArray);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    // Записываем массив обратно в файл
    QTextStream stream(&file);
    stream << jsonString;
    file.close();
}

