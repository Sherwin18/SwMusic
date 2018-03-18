#include "swtablewidget.h"
#include <QDebug>
#include <QMouseEvent>

SWTableWidget::SWTableWidget(QWidget *parent) : QTableView(parent)
{
    this->setMouseTracking(true);
    lastRowBkColor = QColor(0x00,0xff,0x00,0x00);//QColor(255,255,255);
    previousColorRow = -1;
    model = (QStandardItemModel*)this->model();

/*******************本段代码只是测试用，真正使用时需注释掉********************************/
//    this->setRowCount(10);   //设置行数为10
//    this->setColumnCount(5); //设置列数为5

//    for(int i = 0; i < 10; i ++)
//    {
//        for(int j = 0; j < 5; j ++)
//        {
//            QTableWidgetItem *item = new QTableWidgetItem();
//            this->setItem(i,j,item);
//        }
//    }
/*******************本段代码只是测试用，真正使用时需注释掉********************************/

    this->setSelectionBehavior(QAbstractItemView::SelectRows);   //设置整行选择
    //this->setStyleSheet("selection-background-color:rgba(0,0,0,50)");  //设置选中行颜色
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);    //禁止编辑表格
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(cellEntered(int,int)));
}

void SWTableWidget::leaveEvent(QEvent *event)
{
    QStandardItem  *item = 0;

    //还原上一行的颜色
    item = model->item(previousColorRow, 0);
    if (item != 0)
    {
        this->setRowColor(previousColorRow, lastRowBkColor);
    }
}

void SWTableWidget::cellEntered(int row, int column)
{
    QStandardItem  *item = 0;

    //还原上一行的颜色
    item = model->item(previousColorRow, 0);
    if (item != 0)
    {
        this->setRowColor(previousColorRow, lastRowBkColor);
    }

    //设置当前行的颜色
    item = model->item(row, column);
    if (item != 0 && !item->isSelected())
    {
        this->setRowColor(row, QColor(193,210,240));
    }

    //设置行的索引
    previousColorRow = row;
}

void SWTableWidget::setRowColor(int row, QColor color)
{
    for (int col=0; col<this->columnCount(); col++)
       {
           QStandardItem  *item = model->item(row, col);
           item->setBackgroundColor(color);
       }
}

