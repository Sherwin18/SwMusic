#ifndef FILEINFO_H
#define FILEINFO_H
#include <QString>
#include <QImage>


class FileInfo
{
public:
    FileInfo();
    QString title;     //歌曲名
    QString autor;     //作者
    QString album;     //专辑
    QString time;      //播放时长
    QImage image;      //图片
    QString audioBitRate;//音频比特率
    QString chanelCount; //通道
    QString contributingArist;//特约艺术家
    QString genre; //类型
    QString sampleRate;//采样率
    QString year;//年
};

#endif // FILEINFO_H
