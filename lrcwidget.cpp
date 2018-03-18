#include "lrcwidget.h"

LrcWidget::LrcWidget(QWidget *parent) : QQuickWidget(parent)
{
    setSource(QUrl("qrc:/qml.qml"));
}
