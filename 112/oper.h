#ifndef OPER_H
#define OPER_H

#include <QGuiApplication>
#include <QWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class Oper;
}

class Oper : public QWidget
{
    Q_OBJECT

public:
    explicit Oper(QWidget *parent = nullptr);
    ~Oper();
    void saveJson(int); // Сохраняет данные звонка в json ффайле
    void setCallStartTime(QString); // Меняет дату и время начала звонка
    void setNumCars(int); // Меняет число машин, которое необходимо отправить на вызов
public slots:
    void loadText();

private slots:
    void on_saveChangesBtn_clicked();
    void on_sendBtn_clicked();
    void on_cancelBtn_clicked();
    void on_endCallBtn_clicked();

signals:
    void endCall(); // Сигнал об окончании зваонка со стороны оператора

private:
    Ui::Oper *ui;
    QFile file; // Переменная для чтения и записи json файлов
    QString callStartTime = "hhmmss-ddmmyyyy"; // Дата и время на момент начала звонка

    QString problem = "Нет данных"; // Текст, описывающий ситуацию
    QString address = "Нет данных"; // Адрес происшествия
    QString fio = "Нет данных"; // Фамилия Имя Отчество звонившего
    int numCars = 0; // Количество машин, отправленных на вызов
};

#endif // OPER_H
