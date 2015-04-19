#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDebug>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QMediaPlaylist>
#include <QTableWidgetItem>
#include <QStringList>
#include <QMediaMetaData>
#include <QTime>
#include <QLabel>
#include <QKeyEvent>
#include <QFile>
#include <QDataStream>
#include <QMovie>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QtMultimedia>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sliderProgress_sliderMoved(int position);

    void on_sliderVolume_sliderMoved(int position);

    void on_play_clicked();

    void on_stop_clicked();

    void on_durationChanged(qint64 duration);

    void on_positionChanged(qint64 position);

    void on_add_song_clicked();

    void on_playlist_cellDoubleClicked(int row, int column);

    void on_mute_clicked();

    void show_time(quint64 currentInfo);

    void on_DeleteSong_clicked();

    void on_Previous_clicked();

    void on_Next_clicked();

    void on_Shuffle_clicked();

    void on_CurrentSongInLoop_clicked();

    void update_song(int index);

    void on_horizontalSlider_sliderMoved(int position);

    void on_SavePlaylist_clicked();

    void on_LoadPlaylist_clicked();

private:

    void initialize_widget();

    void add_song_cell();

    Ui::MainWindow *ui;

    QMediaPlayer *player;

    QMediaPlaylist *list;

    QTableWidgetItem *track[100],*current_item,*time_item[100],*artist[100];

    QString filename,songname[100],time[100],song_dir[100],artist_name[100];

    QStringList titles;

    qint32 num_songs,current_row,current_song_row,prev_volume,duration;


};

#endif // MAINWINDOW_H
