#include "recorder.h"

#include <stdio.h>
#include "json.hpp"
#include "Windows.h"
#include "qdir.h"
#include "qurl.h"

Recorder::Recorder() // Настройка ввода звука и распознавателя
{
    model = vosk_model_new("C:/Qt/projects/112/vosk/vosk-model-small-ru-0.22"); // Инициализация модели распознавания голоса
    m_recognizer = vosk_recognizer_new(model, sample_rate); // Подключение модели к распознавателю

    group = {"пожар", "дым", "огон", "возгорание"}; // Список ключевых слова

    names = {"Text", "Address", "FIO"}; // Список имён файлов

    QAudioFormat format; // Настройка формата записи звука для распознавания
    format.setSampleRate(sample_rate);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_recorder = new QAudioSource(format, this); // Установка настроек

    session.setAudioInput(&audioInput); // Установка устройства вывода

    session.setRecorder(&m2_recorder); // Установка записывателя

    QMediaFormat m2_format(QMediaFormat::Wave); // Формат записи звука в файл
    m2_recorder.setMediaFormat(m2_format);
}

Recorder::~Recorder()
{
    delete timer;
    delete m_recorder;
    vosk_recognizer_free(m_recognizer);
    vosk_model_free(model);
}

void Recorder::start(int newID) // Начинает запись в буфер и в файл, устанавливает таймер
{
    ID = newID; // Присваивание ID операции
    name = names[ID]; // Присваивание имени файла из списка имён по ID
    status = 1; // Изменение статуса на запись

    timer = new QTimer; // Создание таймера
    bufer = new QBuffer; // и Буфера

    if (ID == 0) // если это первая операция, то начинаем запись в файл
    {
        file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/callRecord.mp4");
        m2_recorder.setOutputLocation(QUrl("C:/Qt/projects/112/Records/" + callStartTime + "/callRecord"));
        m2_recorder.record();
    }

    bufer->open(QBuffer::ReadWrite); // Открывает буфер
    m_recorder->start(bufer); //        и начинаем запись в него

    connect(timer, &QTimer::timeout, this, &Recorder::recognize); // Подключаем таймер к функции заспознавания звука
    timer->start(6 * 1000); // В первый раз ставим таймер на 8 секунд

    qDebug() << "In start ID:" << ID;
}

void Recorder::FastStop() // Остановка ввода по кнопке
{
    if (status) // Если запись идёт, останавливаем её, удаляем таймер и буфер
    {
        qDebug() << "In end by pressing the button";

        m2_recorder.stop();

        m_recorder->stop();
        status = 0;

        bufer->close();
        delete bufer;

        timer->stop();
        delete timer;
    }

    isFoundStr = 0; // Сбрасываем нахождение ключевого слова
}

void Recorder::stop() // Остановка ввода по тишине
{
    qDebug() << "In end";

    m_recorder->stop();
    status = 0;

    bufer->close();
    delete bufer;

    timer->stop();
    delete timer;
}

void Recorder::recognize() // Распознавание речи
{
    int final;
    std::string text1 = " ";
    final = vosk_recognizer_accept_waveform(m_recognizer, bufer->data(), bufer->size()); // Распознаём речь по записи из буфера

    {
        txtout.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/" + name + ".txt");

        text1 = nlohmann::json::parse(vosk_recognizer_result(m_recognizer))["text"]; // Получаем распознанный текст и парсером выводим его в переменную text1

        txtout.open(QIODevice::WriteOnly | QIODevice::Text); // Открываем текстовый файл и записываем в него распознанный текст
        txtout.write(text1.c_str());

        txtout.close();

        if (ID == 0) // Если это первая операция, то проходимся по тексту и ицщем слова из набора ключевых слов
        {
            for (int j = 0; j < group.size(); j++)
            {
                if (text1.find(group[j].toStdString()) != std::string::npos)
                    isFoundStr = 1;
            }
        }


        qDebug() << QString::fromStdString(text1);
    }

    if (!final) // Если тишина не распознана
    {
        timer->start(5000); // Стваим таймер на 5 секунд
    }
    else // Инчае
    {
        stop(); // останавливаем запись

        emit filesUdapted();
        if (isFoundStr) // Если ключевое слово найдено отправляем сигнал на опреацию с ID = ID+1
            emit recorderStop(ID+1);
        else // Иначе на операцию c ID = 4
            emit recorderStop(4);
    }
}

void Recorder::setCallStartTime(QString cst) // Устанавливает дату и время
{
    callStartTime = cst;
}
