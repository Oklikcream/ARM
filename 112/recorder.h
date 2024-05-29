#ifndef RECORDER_H
#define RECORDER_H

#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QMediaFormat>

#include <vosk_api.h>
#include <QObject>
#include <Windows.h>
#include <QAudioSource>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QBuffer>

class Recorder : public QObject {
    Q_OBJECT
public:
    Recorder();
    ~Recorder();
    void stop(); // Остановка записи
    void FastStop(); // остановка записи по кнопке
    void setCallStartTime(QString); // Устанавливает дату и время

public slots:
    void start(int newID); // Начало записи по ID = newID
    void recognize(); // Распознавание голоса

signals:
    void recorderStop(int f); // Сигнал об остановке записи
    void filesUdapted(); // Сигнал об изменении фала(-ов)

private:
    VoskModel *model; // Модель распознавания речи
    VoskRecognizer *m_recognizer; // Распознаватель
    QAudioSource *m_recorder; // Записыватель ниского качества
    QMediaRecorder m2_recorder; // Записыватель высокого качества
    QMediaCaptureSession session; // Текущая сессия, связывающая устройство с записывателем
    QAudioInput audioInput; // Устройство ввода

    QTimer *timer; // Таймер
    QBuffer *bufer; // Буфер
    QFile file; // Файл для записи звука
    QFile txtout; // Файл для сохранения текстовых файлов

    QString name; // Имя текстового файла
    std::vector<QString> group; // Набор ключевых слов
    std::vector<QString> names; // Набор имён файлов

    int ID; // ID операции
    int isFoundStr = 0; // Найдено ли ключевое слово
    bool status = 0; // Записвает ли утройство звук
    QString callStartTime; // Дата и время

    const float sample_rate = 44100; // Частота дискретизации
};

#endif // RECORDER_H
