#ifndef WINDDATALOGGER_H
#define WINDDATALOGGER_H

#include <QObject>
#include <QJsonObject>
#include <QString>

class WindDataLogger {
public:
    // Конструктор принимает путь к файлу для записи данных
    WindDataLogger();
    WindDataLogger(const QString& filePath);

    bool setPath(const QString& filePath);
    QString getPath();

    // Метод для создания JSON-объекта из данных
    QJsonObject createJsonMessage(const QString& sensorName, const QString& windSpeed, const QString& windDirection);

    // Метод для записи JSON в файл
    void appendJsonToFile(const QJsonObject& jsonObject);

private:
    QString m_filePath;  // Путь к файлу
    //QString fileName;
};

#endif // WINDDATALOGGER_H
