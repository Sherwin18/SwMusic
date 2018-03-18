#ifndef SAVETHREAD_H
#define SAVETHREAD_H

#include <QObject>
#include <QThread>
#include <fileinfo.h>
#include <QDir>

class SaveThread : public QThread
{
    Q_OBJECT
public:
    explicit SaveThread(QObject *parent = 0);
    QStringList musicFileList;

protected:
    void run();

signals:

public slots:
};

#endif // SAVETHREAD_H
