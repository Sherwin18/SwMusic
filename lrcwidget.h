#ifndef LRCWIDGET_H
#define LRCWIDGET_H

#include <QWidget>
#include <QtQuickWidgets/QQuickWidget>

class LrcWidget : public QQuickWidget
{
    Q_OBJECT
public:
    explicit LrcWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // LRCWIDGET_H
