#ifndef WINDDATALOGGER_H
#define WINDDATALOGGER_H

#include <QObject>
#include <QJsonObject>
#include <QString>

class WindDataLogger {
public:
    WindDataLogger();
    WindDataLogger(const QString& filePath);

    bool setPath(const QString& filePath);
    QString getPath();

    QJsonObject createJsonMessage(const QString& time, const QString& sensorName, const QString& windSpeed, const QString& windDirection);

    void appendJsonToFile(const QJsonObject& jsonObject);

private:
    QString m_filePath;
};

#endif // WINDDATALOGGER_H
