#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <fileinfo.h>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <tablemodel.h>
#include <QHeaderView>
#include <QImage>
#include <QButtonGroup>
#include <lrcwidget.h>
#include <QQuickWidget>
#include <QQuickImageProvider>
#include <QCoreApplication>
#include <imageprovider.h>
#include <savethread.h>
#include <QMenu>
#include <QAction>
#include <setting.h>
#include <QWidget>
#include <QResizeEvent>
#include <QLabel>
#include <QKeyEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT 
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog(); 
    int currentIndex() const;
    void setCurrentIndex(const int & currentIndex);

private slots:
    void on_pushButton_add_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void currentIndexChanged(int currentIndex);

private:
    Ui::Dialog *ui;
    QStringList musicFileList;
    QList<FileInfo> allMusic;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    TableModel *model;
    QHeaderView *header;
    QButtonGroup *buttonGroup;
    QQuickWidget *lrcWidget;
    int count;
    int m_currentIndex;
    void showList();
    QColor m_currentColor;
    ImageProvider *imageProvider;
    QPoint m_dragPos;
    void findLocal();
    SaveThread *saveThread;
    QMenu *menu;
    QAction *palyAction;
    QAction *delAction;
    Setting *settingDialog;
    QWidget *maskWidget;
    QLabel *tip;
    QString Main_Style;

private slots:
    void showMessage(bool ok);
    void doubleClick(QModelIndex index);
    void on_pushButton_play_clicked();
    void on_pushButton_pre_clicked();
    void on_pushButton_next_clicked();
    void positionChanged(qint64 value);
    void setPosition(int value);
    void playChanged(bool ok);
    void on_pushButton_soud_clicked();
    void on__setting_clicked();
    void on_pushButton_min_clicked();
    void on_pushButton_max_clicked();
    void on_pushButton_close_clicked();
    void on_pushButtontype_clicked();
    void tableViewPress(QModelIndex index);
    void showSetting();
    void styleChanged(QString style);
};

#endif // DIALOG_H


