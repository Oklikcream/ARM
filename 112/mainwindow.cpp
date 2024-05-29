#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ // Определяет настройки окна, создаёт плеер и диктофон
    ui->setupUi(this);

    this->setFixedSize(360, 640);
    setEmulState(0);

    player_m = new Player;
    recorder_m = new Recorder;

    connect(recorder_m, &Recorder::recorderStop, this, &MainWindow::call); // Связывает окончание записи-распознавания с началом нового "диалога"
    connect(recorder_m, &Recorder::filesUdapted, &oper, &Oper::loadText); // При изменении файлов при распознавании загружает их в окно оператора
    connect(&oper, &Oper::endCall, this, &MainWindow::on_endCallBtn_clicked); // Связывает нажатие кнопки окончить звонок с окна оператора с нажатием кнопки с эмулятора
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::call(int i) // Вызов "Диалога" по ID
{
    qDebug() << "----------------" << i << "----------------";
    if (i < 3) // "диалоги" с ID 0,1,2 вызываются с выводомо и вводом звука
    {
        player_m->start(i);
        recorder_m->start(i);
    }
    else if (i == 3) // С ID 3 вывод звука и окно оператора
    {
        player_m->start(i);
        oper.show(); // Открытие окна оператора
    }
    else // ID 4 только вывод звука
    {
        player_m->start(i);
    }
}

void MainWindow::on_startCallBtn_clicked() // Начинает звонок, запоминает время начала
{
    QString cst = QDateTime::currentDateTime().toString("hhmmss-ddMMyyyy"); // Записываем время и дату на момент начала звонка
    QDir().mkdir("C:/Qt/projects/112/Records/" + cst); // Создаём папку, для хранения записей звонка
    recorder_m->setCallStartTime(cst); // Передаём время и дату в классы recorder и oper
    oper.setCallStartTime(cst);

    setEmulState(1); // Меняем состояние эмулятора на звонок
    call(0); // начинаем "диалог" с ID 0
}


void MainWindow::on_endCallBtn_clicked() // Прекращает звонок
{
    oper.close(); // Закрываем окно оператора
    player_m->stop(); // Останавливаем вывод звука
    recorder_m->FastStop(); // Принудительно останавливаем запись
    setEmulState(0); // Меняем состояние эмулятора на главный экран
}

void MainWindow::setEmulState(bool call) // Устанавливает статус окна эмулятора
{
    if (!call) // Главное окно
    {
        ui->endCallBtn->hide();
        ui->startCallBtn->show();
        bkGnd.load(":/Img/mainWindow.png");
    }
    else // Окно звонка
    {
        ui->startCallBtn->hide();
        ui->endCallBtn->show();
        bkGnd.load(":/Img/callWindow.png");
    }
    bkGnd = bkGnd.scaled(this->size(), Qt::IgnoreAspectRatio); // Подстраиваем размер фона под размер окна
    palette.setBrush(QPalette::Window, bkGnd);
    this->setPalette(palette);
}

