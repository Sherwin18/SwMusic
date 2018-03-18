#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QButtonGroup>
#include <QTcpSocket>
#include <listmodel.h>
#include <QMouseEvent>
#include <QColorDialog>
#include <QDebug>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();
signals:
    void messageListChanged();
    void styleChangedSignal(QString);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::Setting *ui;
    QButtonGroup *buttonGroup;
    QButtonGroup *styleGroup;
    QTcpSocket *socket;
    ListModel *listModel;
    QStringList messageList;
    QPoint m_dragPos;
    QString style;
    QColorDialog *colorDialog;
private slots:
    void buttonClicked(int index);
    void readyRead();
    void messageChanged();
    void on_pushButton_close_clicked();
    void on_pushButton_ok_clicked();
    void on_pushButton_color_clicked();
    void styleChanged(int index);
    void on_pushButton_send_clicked();
};

#endif // SETTING_H
