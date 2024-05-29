#include "player.h"

Player::Player() // Создаёт список имён файлов и сам диктофон
{
    names = {"Hello", "Address", "FIO", "Operator", "Operator112"}; // Список имён файлов

    m_player = new QMediaPlayer;
    m_output.setVolume(100); // Громкость вывода
    m_player->setAudioOutput(&m_output); // Подключеие к плееру устройства вывода
}

Player::~Player()
{
    delete m_player;
}

void Player::start(int newID) // Начало вывода звука
{
    ID = newID; // Присваивание ID операции
    name = names[ID]; // Присваивание имени файла из списка по ID

    m_player->setSource(QUrl::fromLocalFile(QString::fromStdString("C:/Qt/projects/112/sounds/"+name+".mp3"))); // Файл с записанным голосом
    m_player->play();
    qDebug() << "Out start";
}

void Player::stop()
{
    m_player->stop();
}
