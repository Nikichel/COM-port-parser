#include "package.h"

Package::Package(QString data, QString sourceAdr){
    QString flag = "00000100";
    QString destinationAdr = "0000";
    QString fcs = "0";
    while(data.length()<4){
        data+=("0");
    }

    frame = flag + destinationAdr + sourceAdr + data + fcs;

    bitStuffing();
}

QString Package::getFrame(){
    return frame;
}

void Package::bitStuffing(){
    for(int i=1;i<frame.length();i++){
        if(frame.mid(i, 7) == "0000010"){
            frame.insert(i+7, "1");
            i++;
        }
    }
}

QString Package::deBitStuffing(QString package){
    for(int i=0;i < package.length();i++){
        if(package.mid(i, 8) == "00000101"){
            package.remove(i + 7, 1);
        }
    }
    return package;
}

QString Package::getData(QString package){
    QString data = "";
    int size = 21;
    int count = package.size() / size;

    for(int i = 0; i<count ; i++){
        data += package.mid(16 + size * i ,4);
    }

    return data;
}

QList<QString> Package::split(QString package){
    QList<QString> frames;
    int iStart = 0;
    int i=0;
    for(i=0;i < package.length();i++){
        if(package.mid(i, 9) == "000001000" && i>0){
            frames.append(package.mid(iStart, i-iStart) + "\n");
            iStart = i;
        }
    }
    frames.append(package.mid(iStart, i-iStart));
    return frames;
}

QString Package::highlight(QString frame){
    QString highlightFrame = frame;
    for(int i=0;i < frame.length();i++){
        if(frame.mid(i, 8) == "00000101"){
            highlightFrame.insert(i+7, "[");
            highlightFrame.insert(i+9, "]");
        }
    }
    return highlightFrame;
}
