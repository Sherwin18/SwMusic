#include "savethread.h"
#include <QDebug>

SaveThread::SaveThread(QObject *parent) : QThread(parent)
{

}

void SaveThread::run()
{
//    QString currentPath = QDir::currentPath();
//    for(int i = 0; i < allMusic.count(); i++){
//        FileInfo info = allMusic.at(i);
//        QString path = currentPath + "/image/" + info.title + ".png";
//        info.image.save(path,"PNG");
//    }





    QString currentPath = QDir::currentPath();
    QFile file(currentPath + "/project.ini");

    file.open(QIODevice::ReadOnly);
//    QTextStream io(&file);
    file.close();

    file.open(QIODevice::Truncate);
    file.close();

    file.open(QIODevice::WriteOnly);
//    io<<"ctr_interface=/var/run/wpa_supplicant\nupdate_config=1\n";


//    file.open(QIODevice::Append);
    for(int i = 0; i < musicFileList.count(); i ++){
        QString musicfile = musicFileList.at(i);
        QString pro = musicfile + "##";
        file.write(pro.toLocal8Bit());
//        io << pro.toLocal8Bit();
    }
}
