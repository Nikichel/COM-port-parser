#include "WindDataLogger.h"
#include <QJsonDocument>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include <QDebug>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

WindDataLogger::WindDataLogger(){
    this->m_filePath = QDir::currentPath()+"/Logs";

    qDebug() << this->m_filePath;
}

WindDataLogger::WindDataLogger(const QString& filePath) {
    this->m_filePath = filePath;

    qDebug() << this->m_filePath;
}

bool WindDataLogger::setPath(const QString& filePath) {
    qDebug() << "Проверяем путь:" << filePath;

    QFileInfo fileInfo(filePath);
    if (fileInfo.exists()) {
        this->m_filePath = filePath;
        qDebug() << "Файл или директория существует.";
        return true;
    } else {
        qDebug() << "Файл или директория не найдены.";
        return false;
    }
}

QString WindDataLogger::getPath(){
    return this->m_filePath;
}

QJsonObject WindDataLogger::createJsonMessage(const QString& time, const QString& sensorName, const QString& windSpeed, const QString& windDirection) {
    QJsonObject jsonObject;
    jsonObject["time"] = time;
    jsonObject["sensor_name"] = sensorName;
    jsonObject["wind_speed"] = windSpeed;
    jsonObject["wind_direction"] = windDirection;
    return jsonObject;
}

/*
void WindDataLogger::appendJsonToFile(const QJsonObject& jsonObject) {
    QFile file(m_filePath + "/logs_wind.json");

    QJsonArray jsonArray;

    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Не удалось открыть файл для чтения.";
            qWarning() << file.errorString();
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument existingDoc = QJsonDocument::fromJson(fileData);

        if (existingDoc.isArray()) {
            jsonArray = existingDoc.array();
        } else {
            qWarning() << "Существующий файл не является массивом JSON.";
        }
    }

    jsonArray.append(jsonObject);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи.";
        qWarning() << file.errorString();
        return;
    }

    QJsonDocument jsonDoc(jsonArray);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);

    QTextStream stream(&file);
    stream << jsonString;
    file.close();
}
*/

void WindDataLogger::appendJsonToFile(const QJsonObject& jsonObject) {
    QFile file(m_filePath + "/logs_wind.json");

    // Открываем файл для чтения
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл.";
        qWarning() << file.errorString();
        return;
    }

    // Чтение существующих данных
    QByteArray fileData = file.readAll();
    QJsonArray jsonArray;

    if (!fileData.isEmpty()) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(fileData);
        if (existingDoc.isArray()) {
            jsonArray = existingDoc.array();
        } else {
            qWarning() << "Существующий файл не является массивом JSON.";
        }
    }

    // Добавляем новый объект в массив
    jsonArray.append(jsonObject);

    // Переходим в начало файла, чтобы перезаписать его
    file.resize(0);

    // Запись данных обратно в файл
    QJsonDocument jsonDoc(jsonArray);
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();
}

