#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QPushButton>
#include <vosk_api.h>
#include "player.h"
#include "recorder.h"
#include "oper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void call(int i); // Вызов "Диалога"

private slots:
    void on_startCallBtn_clicked();
    void on_endCallBtn_clicked();
    void setEmulState(bool call); // Меняет состояние эмулятора


private:
    Ui::MainWindow *ui;
    QPixmap bkGnd; // Фон эмулятора
    QPalette palette;
    Player *player_m; // Подключение динамика
    Recorder *recorder_m; // Подключение микрофона

    QString output_file;
    Oper oper; // Окно оператора
};
#endif // MAINWINDOW_H
