#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    buttonGroup = new QButtonGroup();
    styleGroup = new QButtonGroup();
    socket = new QTcpSocket();
    listModel = new ListModel();
    colorDialog = new QColorDialog();
    style = "";
    socket->connectToHost("127.0.0.1",6666);
    buttonGroup->addButton(ui->pushButtonshow,0);
//    buttonGroup->addButton(ui->pushButton_setting,1);
    buttonGroup->addButton(ui->pushButton_message,1);
    buttonGroup->addButton(ui->pushButton_about,2);
    ui->pushButtonshow->setCheckable(true);
    ui->pushButton_message->setCheckable(true);
    ui->pushButtonshow->setChecked(true);
    ui->pushButton_purple->setChecked(true);
    styleGroup->addButton(ui->pushButton_blue,0);
    styleGroup->addButton(ui->pushButton_red,1);
    styleGroup->addButton(ui->pushButton_purple,2);
    styleGroup->addButton(ui->pushButton_green,3);
    styleGroup->addButton(ui->pushButton_green_2,4);
    styleGroup->addButton(ui->pushButton_blue_2,5);
    styleGroup->addButton(ui->pushButton_yellow,6);
    ui->textEdit->setPlaceholderText("请输入信息...");
    ui->textEdit_about->setText("    当前纠纠音乐的版本为1.0，此程序为个人毕业设计！打包于2018年3月10日。"
                                "当前程序还有部分功能待继续完善，敬请期待！本程序还存在一些不可避免的bug。有发现缺点望多多指正，谢谢使用！");

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowMinMaxButtonsHint);
//    ui->listView->setModel(listModel);
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonClicked(int)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(this,SIGNAL(messageListChanged()),this,SLOT(messageChanged()));
    connect(styleGroup,SIGNAL(buttonClicked(int)),this,SLOT(styleChanged(int)));
    messageList << "帅气的作者:欢迎您为我们提供宝贵的建议！";
    emit messageListChanged();
}

Setting::~Setting()
{
    delete buttonGroup;
    delete styleGroup;
    delete socket;
    delete listModel;
    delete colorDialog;
    delete ui;
}

void Setting::buttonClicked(int index)
{
//    if(index == 1){
//        messageList << "帅气的作者:欢迎您为我们提供宝贵的建议！";
//        emit messageListChanged();
//    }
    ui->stackedWidget->setCurrentIndex(index);
}

void Setting::readyRead()
{
    QString message = socket->readAll();
    message = "帅气的作者:" + message;
    messageList << message;
    emit messageListChanged();
    qDebug() << message;
    //    socket->write("hi");
}

void Setting::messageChanged()
{
    ui->listView->setModel(new ListModel());
    listModel->messageList = messageList;
    ui->listView->setModel(listModel);
}

void Setting::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_dragPos = event->globalPos() - this->geometry().topLeft();
    }
}

void Setting::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if(event->y() < 40){
            QPoint curPoint = event->globalPos() - m_dragPos;
            move(curPoint);
        }
    }
}

void Setting::on_pushButton_close_clicked()
{
    this->close();
}

void Setting::on_pushButton_ok_clicked()
{
    emit styleChangedSignal(style);
    this->close();
}

void Setting::on_pushButton_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white,this,"选择颜色");
    style = color.name();
//    emit styleChangedSignal(style);
//    this->close();
}

void Setting::styleChanged(int index)
{
    if(index == 0){
        style = "#3239c8";
    }else if(index == 1){
        style = "#bf3d3d";
    }else if(index == 2){
        style = "#5a0681";
    }else if(index == 3){
        style = "#2cdc1f";
    }else if(index == 4){
        style = "#27c954";
    }else if(index == 5){
        style = "#37b9d0";
    }else if(index == 6){
        style = "#9e991d";
    }
}

void Setting::on_pushButton_send_clicked()
{
    QString text = ui->textEdit->document()->toPlainText();
    if(!text.isEmpty()){
        socket->write(text.toUtf8());
        text = "可爱的你:" + text;
        messageList << text << "帅气的作者目前无法收不到你的信息!加微信吧!!!";
        ui->textEdit->clear();
        emit messageListChanged();
    }
}
