#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <fileinfo.h>

class ImageProvider : public QQuickImageProvider
{
public:
//    ImageProvider();
    ImageProvider(): QQuickImageProvider(QQuickImageProvider::Image){}
    ~ImageProvider(){}
    QList<FileInfo> allMusic;
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize){
        QImage image;
        image.load(":/image/Logo4.png");
        int index = id.toInt();
        if(index < allMusic.count()){
            FileInfo info = allMusic.at(index);
            if(!info.image.isNull()){
                return info.image;
            }else {
                return image;
            }
        }
    }
//    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
//    {
//        QString strFileName = QDir::currentPath() + "/image/" + id + ".png";
//        qDebug() << strFileName;
//        qDebug() << "application:" << QCoreApplication::applicationDirPath();
//        QImage image;
//        image.load(strFileName);
//        qDebug() << image.size();
//        QPixmap pixmap(strFileName);
//        qDebug() << pixmap.height();
//        return pixmap;
//    }
};

#endif // IMAGEPROVIDER_H
