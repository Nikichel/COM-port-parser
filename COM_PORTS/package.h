#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <QList>
#include <QDebug>
class Package
{
public:
    //Package();
    Package(QString data, QString source);
    QString getFrame();

    static QString getData(QString frame);
    static QString deBitStuffing(QString package);
    static QString highlight(QString package);
    static QList<QString> split(QString package);
private:
    QString frame;
    void bitStuffing();
};

#endif // PACKAGE_H
