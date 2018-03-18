#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QHeaderView>
#include <QDateTime>
#include <QPainter>
#include <QQmlContext>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("纠纠音乐");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowMinMaxButtonsHint);
    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    buttonGroup = new QButtonGroup();
    lrcWidget = new QQuickWidget(this);
    saveThread = new SaveThread();
    settingDialog = new Setting(this);
    menu = new QMenu();
//    delAction = new QAction("删除");
//    palyAction = new QAction("播放");
//    menu->addAction(palyAction);
//    menu->addAction(delAction);
    QQmlEngine *engine = lrcWidget->engine();
    imageProvider = new ImageProvider();
    lrcWidget->setSource(QUrl("qrc:/qml.qml"));
    lrcWidget->rootContext()->setContextProperty("self",this);
    engine->addImageProvider("myprovider", imageProvider );
    lrcWidget->setClearColor(QColor(Qt::transparent));
    lrcWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    lrcWidget->show();
    ui->horizontalLayout_18->insertWidget(1,lrcWidget);
    buttonGroup->addButton(ui->pushButton_list,0);
    buttonGroup->addButton(ui->pushButton_message,1);
    buttonGroup->addButton(ui->pushButtonlrc,2);
    ui->pushButtonlrc->setCheckable(true);
    ui->pushButton_list->setCheckable(true);
    ui->pushButton_message->setCheckable(true);
    ui->pushButton_list->setChecked(true);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    count = 0;
    m_currentIndex = 0;
    model = new TableModel();
    ui->horizontalSlider_soud->setMaximum(100);
    player->setVolume(50);
    ui->horizontalSlider_soud->setValue(player->volume());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    maskWidget = new QWidget(this);
    maskWidget->setGeometry(0,0,this->width(),this->height());
    maskWidget->setStyleSheet("background-color:rgba(0,0,0,0.3)");
//    maskWidget->hide();
    tip = new QLabel(maskWidget);
    tip->setObjectName("tip");
    tip->setText("正在拼命加载中，请稍后...");
    tip->setGeometry(maskWidget->width()/2 - 150,maskWidget->height()/2 - 20, 300,50);
    findLocal();
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClick(QModelIndex)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(positionChanged(qint64)));
    connect(ui->horizontalSlider_play,SIGNAL(sliderMoved(int)),this,SLOT(setPosition(int)));
    connect(ui->horizontalSlider_soud,SIGNAL(sliderMoved(int)),player,SLOT(setVolume(int)));
    connect(buttonGroup,SIGNAL(buttonClicked(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(showMessage(bool)));
    connect(ui->tableView,SIGNAL(pressed(QModelIndex)),this,SLOT(tableViewPress(QModelIndex)));
    connect(ui->pushButton_setting,SIGNAL(clicked()),this,SLOT(showSetting()));
    connect(settingDialog,SIGNAL(styleChangedSignal(QString)),this,SLOT(styleChanged(QString)));

    QFile file(":/css/blueStyle.css");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    Main_Style = in.readAll();
    QString qss = Main_Style.arg("#5a0681");
    setStyleSheet(qss);
}

Dialog::~Dialog()
{
    delete player;
    delete playlist;
    delete buttonGroup;
    delete lrcWidget;
    delete saveThread;
    delete settingDialog;
    delete ui;
}

int Dialog::currentIndex() const
{
    return this->m_currentIndex;
}

void Dialog::setCurrentIndex(const int &currentIndex)
{
    this->m_currentIndex = currentIndex;
    emit currentIndexChanged(this->m_currentIndex);
}

void Dialog::on_pushButton_add_clicked()
{
    tip->hide();
    maskWidget->show();
    disconnect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(playChanged(bool)));
    m_currentIndex = playlist->currentIndex();
    int musicCount = playlist->mediaCount();
    connect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(showMessage(bool)));
    QStringList list = QFileDialog::getOpenFileNames(this, tr("打开文件"),"",tr("music(*.mp3)"));
    if(!list.isEmpty()){
        tip->show();
        for (int i = 0; i < list.count(); i ++)
        {
            QString musicfile = list.at(i);
            bool ishad = false;
            for(int i = 0; i < musicFileList.count(); i ++){
                QString file = musicFileList.at(i);
                if(file == musicfile){
                    ishad = true;
                }
            }
            if(!ishad){
                musicFileList << musicfile;
                playlist->addMedia(QUrl::fromLocalFile(musicfile));
            }
        }
        playlist -> setCurrentIndex(musicCount);
        saveThread->musicFileList = musicFileList;
        saveThread->start();
    }else{
        maskWidget->hide();
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_dragPos = event->globalPos() - this->geometry().topLeft();
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if(event->y() < 40){
            QPoint curPoint = event->globalPos() - m_dragPos;
            move(curPoint);
        }
    }
}

void Dialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        if(event->y() < 40){
           if(this->isMaximized()){
               this->showNormal();
           }else{
               this->showMaximized();
           }
        }
    }
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    maskWidget->setGeometry(0,0,this->width(),this->height());
    tip->setGeometry(maskWidget->width()/2 - 200,maskWidget->height()/2 - 20, 300,50);
}

void Dialog::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        on_pushButton_play_clicked();
    }else if(event->key() == Qt::Key_Left){
        on_pushButton_pre_clicked();
    }else if(event->key() == Qt::Key_Right){
        on_pushButton_next_clicked();
    }
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
//    if(event->key() == Qt::Key_Space){
//        on_pushButton_play_clicked();
//    }else if(event->key() == Qt::Key_Left){
//        on_pushButton_pre_clicked();
//    }else if(event->key() == Qt::Key_Right){
//        on_pushButton_next_clicked();
//    }
}

void Dialog::showList()
{
    disconnect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(showMessage(bool)));
    TableModel *talbe = new TableModel();
    ui->tableView->setModel(talbe);
    model->musicList = allMusic;
    imageProvider->allMusic = allMusic;
    ui->tableView->setModel(model);
    playlist->setCurrentIndex(m_currentIndex);
    maskWidget->hide();
    on_pushButton_play_clicked();
    connect(player, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(playChanged(bool)));

}

void Dialog::findLocal()
{
    maskWidget->show();
    QString currentPath = QDir::currentPath();
    QFile file(currentPath + "/project.ini");
    if (!file.exists()){
        maskWidget->hide();
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QByteArray project = file.readAll();
        QTextStream in(&file);
        QString project = in.readAll();
        QStringList proList = project.split("##");
        for(int i = 0; i < proList.count(); i ++){
            QString pro = proList.at(i);
            if(!pro.isEmpty()){
                playlist->addMedia(QUrl::fromLocalFile(pro));
                musicFileList << pro;
            }
        }
    }
    playlist -> setCurrentIndex(0);
}

void Dialog::showMessage(bool ok)
{
    if(ok){
        FileInfo info;
        foreach(QString str,player->availableMetaData()){
           QString message = player->metaData(str).toString().toUtf8().data();
           if(str == "AlbumTitle"){
               if(!message.isEmpty())
                   info.album = message;
           }else if (str == "Author") {
               if(!message.isEmpty())
                   info.autor = message;
           }else if (str == "Duration") {
               if(!message.isEmpty()){
                   QDateTime duration  = QDateTime::fromMSecsSinceEpoch(message.toLong());
                   info.time = duration.toString("mm:ss");
               }
           }else if (str == "ThumbnailImage") {
               QImage image = player->metaData("ThumbnailImage").value<QImage>();
               if (image.byteCount()){
                   info.image = image;
               }
           }else if (str == "Title") {
               if(!message.isEmpty()){
                   info.title = message;
               }
           }else if(str == "AudioBitRate"){
               if(!message.isEmpty()){
                   info.audioBitRate = message;
               }
           }else if(str == "ChannelCount"){
               if(!message.isEmpty()){
                   info.chanelCount = message;
               }
           }else if(str == "ContributingArtist"){
               if(!message.isEmpty()){
                   info.contributingArist = message;
               }
           }else if(str == "Genre"){
               if(!message.isEmpty()){
                   info.genre = message;
               }
           }else if(str == "SampleRate"){
               if(!message.isEmpty()){
                   info.sampleRate = message;
               }
           }else if(str == "Year"){
               if(!message.isEmpty()){
                   info.year = message;
               }
           }
       }
       allMusic << info;
       playlist->setCurrentIndex(++count);
       if (count == playlist->mediaCount()){
           showList();
       }
   }
}

void Dialog::doubleClick(QModelIndex index)
{   
    if(QApplication::mouseButtons() == Qt::LeftButton && m_currentIndex != index.row()){
        playlist->setCurrentIndex(index.row());
    }
}

void Dialog::on_pushButton_play_clicked()
{
    if(player -> state() == QMediaPlayer::PlayingState){
        player->pause();
        ui->pushButton_play->setStyleSheet("QPushButton#pushButton_play{width: 45px;height: 45px;border-image: url(:/image/css/play_nomal.png);}"
                                           "QPushButton#pushButton_play:hover{border-image: url(:/image/css/play_click.png);}"
                                           "QPushButton#pushButton_play:pressed{border-image: url(:/image/css/play_click.png);}");
    }else if(player -> state() == QMediaPlayer::PausedState || player->state() == QMediaPlayer::StoppedState){
        player->play();
        ui->pushButton_play->setStyleSheet("QPushButton#pushButton_play{width: 45px;height: 45px;border-image: url(:/image/css/pause_nomal.png);}"
                                           "QPushButton#pushButton_play:hover{border-image: url(:/image/css/pause_click.png);}"
                                           "QPushButton#pushButton_play:pressed{border-image: url(:/image/css/pause_click.png);}");
    }
    else{
        playlist->clear();
        musicFileList.clear();
        findLocal();
        qDebug() << "卡住了";
    }
}

void Dialog::on_pushButton_pre_clicked()
{
    if(playlist->currentIndex() == 0)
        playlist->setCurrentIndex(playlist->mediaCount() - 1);
    else{
        playlist->previous();
    }
}

void Dialog::on_pushButton_next_clicked()
{
    if(playlist->currentIndex() == playlist->mediaCount() - 1)
        playlist->setCurrentIndex(0);
    else{
        playlist->next();
    }
}

void Dialog::positionChanged(qint64 value)
{
    ui->horizontalSlider_play->setMaximum(player->duration());
    ui->horizontalSlider_play->setValue(value);
    QDateTime time = QDateTime::fromMSecsSinceEpoch(player->duration());
    QDateTime playtime = QDateTime::fromMSecsSinceEpoch(value);
    ui->label_playtime->setText(playtime.toString("mm:ss"));
    ui->label_time->setText(time.toString("mm:ss"));
}

void Dialog::setPosition(int value)
{
    player->setPosition(value);
}

void Dialog::playChanged(bool ok)
{
    if(ok){
        m_currentIndex = playlist->currentIndex();
        emit currentIndexChanged(m_currentIndex);
        FileInfo info = allMusic.at(m_currentIndex);
        ui->label_autor->setText(info.autor + "-" + info.title);
        ui->msgAlbum->setText(info.album);
        ui->msgArist->setText(info.contributingArist);
        ui->msgChanel->setText(info.chanelCount);
        ui->msgAutor->setText(info.autor);
        ui->msgGenre->setText(info.genre);
        ui->msgRate->setText(info.audioBitRate);
        ui->msgSample->setText(info.sampleRate);
        ui->msgTile->setText(info.title);
        ui->msgTime->setText(info.time);
        ui->msgYear->setText(info.year);
        if(info.image.isNull()){
            ui->msgImage->setPixmap(QPixmap::fromImage(QImage(":/image/Logo4.png").scaled(ui->msgImage->size())));
            ui->label->setPixmap(QPixmap::fromImage(QImage(":/image/Logo4.png").scaled(ui->label->size())));
        }
        else{
            ui->msgImage->setPixmap(QPixmap::fromImage(info.image.scaled(ui->msgImage->size())));
            ui->label->setPixmap(QPixmap::fromImage(info.image.scaled(ui->label->size())));
        }
        this->repaint();
    }
}

void Dialog::on_pushButton_soud_clicked()
{
    if(player->isMuted()){
        player->setMuted(false);
        ui->pushButton_soud->setStyleSheet("QPushButton#pushButton_soud{border-image: url(:/image/css/open.png);}");
    }else{
        player->setMuted(true);
        ui->pushButton_soud->setStyleSheet("QPushButton#pushButton_soud{border-image: url(:/image/css/close_audio.png);}");
    }
}

void Dialog::on__setting_clicked()
{

}

void Dialog::on_pushButton_min_clicked()
{
    this->showMinimized();
}

void Dialog::on_pushButton_max_clicked()
{
    if(this->isMaximized()){
        this->showNormal();
    }else{
        this->showMaximized();
    }
}

void Dialog::on_pushButton_close_clicked()
{
    this->close();
}

void Dialog::on_pushButtontype_clicked()
{
    if(playlist->playbackMode() == QMediaPlaylist::Loop){//当前是顺序
        playlist->setPlaybackMode(QMediaPlaylist::Random);//随机
        ui->pushButtontype->setText("随机播放");
    }else if((playlist->playbackMode() == QMediaPlaylist::Random)){
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);//单曲
        ui->pushButtontype->setText("单曲播放");
    }else if((playlist->playbackMode() == QMediaPlaylist::CurrentItemOnce)){
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//单曲循环
        ui->pushButtontype->setText("单曲循环");
    }else if((playlist->playbackMode() == QMediaPlaylist::CurrentItemInLoop)){
        playlist->setPlaybackMode(QMediaPlaylist::Loop);//顺序
        ui->pushButtontype->setText("顺序播放");
    }

}

void Dialog::tableViewPress(QModelIndex index)
{
    if(QApplication::mouseButtons() == Qt::RightButton){

    }
}

void Dialog::showSetting()
{
    tip->hide();
    maskWidget->show();
    settingDialog->resize(600,400);
    settingDialog->exec();
    maskWidget->hide();
    tip->show();
}

void Dialog::styleChanged(QString style)
{
    QString qss = Main_Style.arg(style);
    setStyleSheet(qss);
    this->repaint();
}
