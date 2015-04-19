#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QtMultimedia/QAudio>
#include <QtWinExtras>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer(this))
{

    num_songs=0;
    ui->setupUi(this);
    ui->CurrentSongInLoop->setStyleSheet("color:Green");
    list=new QMediaPlaylist;

    //START OF DEFINING the UI

    ui->frame1->setStyleSheet("background-image: url(:/myresource/frame1_color.jpg");
    ui->frame2->setStyleSheet("background-image: url(:/myresource/frame2_color.jpG");

    ui->play->setStyleSheet("border:none");

    QPixmap *pic = new QPixmap(":/myresource/play.png");
    QIcon *icon = new QIcon(*pic);
    ui->play->setIcon(*icon);
    ui->play->setIconSize(QSize(30,30));

    QPixmap *pic1 = new QPixmap(":/myresource/stop.png");
    QIcon *icon1 = new QIcon(*pic1);
    ui->stop->setIcon(*icon1);
    ui->stop->setIconSize(QSize(50,50));
    ui->stop->setStyleSheet("border:none");

    QMovie *movie = new QMovie(":/myresource/4.gif");
    QLabel *processLabel = new QLabel(this);
    processLabel->setGeometry(50,40,170,100);
    processLabel->setMovie(movie);
    movie->start();

    //END OF starting UI
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    connect(list,SIGNAL(currentIndexChanged(int)),this,SLOT(update_song(int)));

    initialize_widget();

    player->setPlaylist(list);

    current_item=ui->playlist->currentItem();
    current_row=ui->playlist->currentRow();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize_widget()
{
    for(int i=0;i<1;i++) ui->playlist->insertColumn(i);
    titles<<"Name";

    ui->playlist->setHorizontalHeaderLabels(titles);
}


void MainWindow::update_song(int index) //selection of file being played from the playlist table
{
    ui->playlist->selectRow(index);
}

void MainWindow::on_sliderProgress_sliderMoved(int position) //ACQUIRING INFORMATION ON SLIDING OF THE PROGRESS BAR
{
    player->setPosition(position);
}

void MainWindow::on_sliderVolume_sliderMoved(int position)
{
    if(ui->mute->text()=="Mute")
    player->setVolume(position);
}

void MainWindow::on_play_clicked() //PLAY AND PAUSE FEATURE
{

    if(ui->play->text()=="Play")
    {
        QPixmap *pic = new QPixmap(":/myresource/pause.png");
        QIcon *icon = new QIcon(*pic);
        ui->play->setIcon(*icon);
        ui->play->setIconSize(QSize(30,30));
        player->play();
        ui->play->setText("Pause");
    }
    else
    {
        player->pause();
        QPixmap *pic = new QPixmap(":/myresource/play.png");
        QIcon *icon = new QIcon(*pic);
        ui->play->setIcon(*icon);
        ui->play->setIconSize(QSize(30,30));
        ui->play->setText("Play");
    }
}

void MainWindow::on_stop_clicked() //STOP FEATURE
{


    player->stop();

    QPixmap *pic1 = new QPixmap(":/myresource/stop.png");
    QIcon *icon1 = new QIcon(*pic1);
    ui->stop->setIcon(*icon1);
    ui->stop->setIconSize(QSize(50,50));

    QPixmap *pic = new QPixmap(":/myresource/play.png");
    QIcon *icon = new QIcon(*pic);
    ui->play->setIcon(*icon);
    ui->play->setIconSize(QSize(30,30));
    ui->play->setText("Play");


}

void MainWindow::on_durationChanged(qint64 duration) //SETTING THE DURATION OF SONG
{
    ui->sliderProgress->setMaximum(duration);

    this->duration = duration/1000;
    ui->sliderProgress->setValue(duration / 1000);
}

void MainWindow::on_positionChanged(qint64 position) // SYNCHRONIZING THE MUSIC PLAYER AND THE PROGRESS BAR
{
    ui->sliderProgress->setValue(position);
    show_time(position/1000);
}

void MainWindow::on_add_song_clicked()
{
    ui->add_song->setStyleSheet("color:gray");
    song_dir[num_songs]=QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "C://",
                                                     tr("MP3 files (*.mp3);;All files (*.*)"));
    add_song_cell();

    list->addMedia(QUrl(song_dir[num_songs]));

    ui->add_song->setStyleSheet("color:default");
    num_songs++;
}

void MainWindow::add_song_cell()
{

    songname[num_songs]=QFileInfo(song_dir[num_songs]).fileName();
    track[num_songs] = new QTableWidgetItem(songname[num_songs]);
    track[num_songs]->setFlags(track[num_songs]->flags() ^ Qt::ItemIsEditable); /*to make the item non-editable*/
    ui->playlist->insertRow(num_songs);
    ui->playlist->setItem(num_songs,0,track[num_songs]);//ADDING CELLS TO PLAYLIST

}


void MainWindow::on_playlist_cellDoubleClicked(int row, int column)// PLAYING THE MUSIC ON DOUBLE CLICK ON THE FILE FROM PLAYLIST
{
    if(column==0)
    {
        list->setCurrentIndex(row);
        player->play();
        ui->play->setText("Pause");

        QPixmap *pic = new QPixmap(":/myresource/pause.png");
        QIcon *icon = new QIcon(*pic);
        ui->play->setIcon(*icon);
        ui->play->setIconSize(QSize(30,30));
        ui->play->setText("Pause");
    }
}

void MainWindow::on_mute_clicked()
{
    if(ui->mute->text()=="Mute")
    {
        prev_volume = player->volume();
        player->setVolume(0);
        ui->sliderVolume->setValue(0);
        ui->mute->setText("UnMute");
    }

    else
    {
        player->setVolume(prev_volume);
        ui->sliderVolume->setValue(prev_volume);
        ui->mute->setText("Mute");
    }

}


void MainWindow::show_time(quint64 currentInfo) // DISPLAYING THE TIME ELAPSED AND TOTAL TIME FOR FILE BEING PLAYED
{

    QString tStr;

    QTime currentTime(0,(currentInfo/60)%60, currentInfo%60);
    QTime totalTime(0,(duration/60)%60, duration%60);
    QString format = "mm:ss";
    tStr = currentTime.toString(format) + " / " + totalTime.toString(format);

    ui->label->setText(tStr);

}


void MainWindow::on_DeleteSong_clicked()
{
    int i,j=ui->playlist->currentRow();
    if( j != num_songs-1 ){
        list->removeMedia(j);
        for ( i=j; i<num_songs-1; i++ ){
            song_dir[i]=song_dir[i+1];
            songname[i]=songname[i+1];
            delete track[i];
            track[i] = new QTableWidgetItem(songname[i]);
            track[i]->setFlags(track[i]->flags() ^ Qt::ItemIsEditable);
            ui->playlist->setItem(i, 0, track[i]);
        }
        delete track[i];
    }
    else {
        player->stop();
        list->removeMedia(j);
        delete track[num_songs-1];
    }
    ui->playlist->removeRow(num_songs-1);
    num_songs--;
}
void MainWindow::on_Previous_clicked()
{
    list->previous();
    player->play();
}

void MainWindow::on_Next_clicked()
{
    list->next();
    player->play();
}

void MainWindow::on_Shuffle_clicked()
{
    if(ui->Shuffle->text() == "Shuffle"){
        list->setPlaybackMode(QMediaPlaylist::Random);
        player->play();
        ui->Shuffle->setText("Revert from Shuffle");
    }

    else {
        list->setPlaybackMode(QMediaPlaylist::Sequential);
        player->play();
        ui->Shuffle->setText("Shuffle");
    }
}

void MainWindow::on_CurrentSongInLoop_clicked()
{
    ui->CurrentSongInLoop->setStyleSheet("color:Green");

    if(ui->CurrentSongInLoop->text() == "Activate Loop"){
        list->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        player->play();
        ui->CurrentSongInLoop->setText("Deactivate Loop");
        ui->CurrentSongInLoop->setStyleSheet("color:red");
    }

    else {
        list->setPlaybackMode(QMediaPlaylist::Sequential);
        player->play();
        ui->CurrentSongInLoop->setText("Activate Loop");

    }
}



void MainWindow::on_horizontalSlider_sliderMoved(int value) // SET THE PLAYBACK RATE OF MUSIC
{
    qreal v = value/100;
    float rate = value;
    float rate1 = rate/100;

    QString tStr = QString::number(rate1);
    QString tStr1=tStr + "x";
    player->setPlaybackRate(v);
    ui->label_3->setText(tStr1);
}

void MainWindow::on_SavePlaylist_clicked()
{
    list->save(QUrl::fromUserInput(QFileDialog::getSaveFileName(this, tr("Save File"),
                                                                 "C:/playlist.m3u",
                                                                 tr("M3U files (*.m3u);;All files (*.*)"))), "m3u");
}


void MainWindow::on_LoadPlaylist_clicked()
{
    list->load(QUrl::fromUserInput(QFileDialog::getOpenFileName(this, tr("Save File"),
                                                                "C://",
                                                                tr("M3U files (*.m3u);;All files (*.*)"))), "m3u");
    if(!list->isEmpty())
    {
        for(int i=0;!list->media(i).isNull();i++)
        {
            song_dir[num_songs]=list->media(i).canonicalUrl().toString();
            add_song_cell();
            num_songs++;
        }
    }
}
