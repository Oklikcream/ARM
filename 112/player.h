#ifndef PLAYER_H
#define PLAYER_H

#include <Qtimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    Player();
    ~Player();
    void start(int); // Начать вывод звука

public slots:
    void stop(); // Принудительно закончить вывод

private:
    QMediaPlayer *m_player; // Плеер
    QAudioOutput m_output; // Устройство вывода

    std::string name = ""; // Имя файла
    std::vector<std::string> names; // Список имён файлов

    int ID; // ID операции вывода
};

#endif // PLAYER_H
