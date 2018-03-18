#ifndef SWTABLEWIDGET_H
#define SWTABLEWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QColor>
#include <QStandardItem>

class SWTableWidget : public QTableView
{
    Q_OBJECT
public:
    explicit SWTableWidget(QWidget *parent = nullptr);
protected:
    void leaveEvent(QEvent *event);
public slots:
    void cellEntered(int,int);
private:
    QColor lastRowBkColor;
    int previousColorRow;
    void setRowColor(int,QColor);
    QStandardItemModel *model;
};

#endif // SWTABLEWIDGET_H
