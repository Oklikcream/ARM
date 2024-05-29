#include "oper.h"
#include "ui_oper.h"
#include <QMessageBox>

Oper::Oper(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Oper)
{ // Устанавливает размеры и связи
    ui->setupUi(this);

    this->setFixedWidth(620);
    this->setFixedHeight(700);

    connect(this->ui->numCarsBox, &QSpinBox::valueChanged, this, &Oper::setNumCars); // Устанавливает значение из счётчика в переменную  numCars
}

Oper::~Oper()
{
    delete ui;
}

void Oper::saveJson(int nc) { // Сохраняет данные

    QVariantMap map; // Создание и заполнение словаря
    map.insert("Problem", problem);
    map.insert("Address", address);
    map.insert("FIO", fio);
    map.insert("NumCars", nc);
    map.insert("Date", callStartTime);
    QJsonObject object = QJsonObject::fromVariantMap(map); // Преобразование словаря в json объект

    QJsonDocument document;
    document.setObject(object); // Создание json документа на основе json объекта

    QFile jsonFile("C:/Qt/projects/112/Records/" + callStartTime + "/callInfo.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson()); // Сохранение json документа в файл
}

void Oper::on_saveChangesBtn_clicked() // Сохраняет значения из текстовых полей в соответствующие файлы
{
    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/Text.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        problem = ui->textEdit->toPlainText();
        file.write(problem.toStdString().c_str());
        file.close();
    }

    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/Address.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        address = ui->addressEdit->toPlainText();
        file.write(address.toStdString().c_str());
        file.close();
    }

    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/FIO.txt");
    if(file.open(QIODevice::WriteOnly))
    {
        fio = ui->fioEdit->toPlainText();
        file.write(fio.toStdString().c_str());
        file.close();
    }
}

void Oper::loadText() // Открывает файлы и считывает из них значения в соответствующие поля
{
    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/Text.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        problem = file.readAll();
        file.close();
    }

    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/Address.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        address = file.readAll();
        file.close();
    }

    file.setFileName("C:/Qt/projects/112/Records/" + callStartTime + "/FIO.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        fio = file.readAll();
        file.close();
    }

    ui->textEdit->setText(problem);
    ui->addressEdit->setText(address);
    ui->fioEdit->setText(fio);
}

void Oper::setCallStartTime(QString cst) // Устанавливает дату и время, а также статус кнопок
{
    callStartTime = cst;

    ui->cancelBtn->setEnabled(true);
    ui->sendBtn->setEnabled(true);
    ui->endCallBtn->setEnabled(false);
}

void Oper::setNumCars(int nc) // Меняет число машин
{
    numCars = nc;
}

void Oper::on_sendBtn_clicked() // Сообщение об успешной отравке служб, меняет ствтус кнопок
{
    QMessageBox mb;
    mb.setWindowTitle("Отправлено");
    std::string s = "По адресу отправлено " + std::to_string(numCars) + " машин";
    mb.setText(s.c_str());
    mb.exec();
    saveJson(numCars);

    ui->cancelBtn->setEnabled(false);
    ui->sendBtn->setEnabled(false);
    ui->endCallBtn->setEnabled(true);
}

void Oper::on_cancelBtn_clicked() // Сообщение об отмене вызова, меняет ствтус кнопок
{
    QMessageBox mb;
    mb.setWindowTitle("Отменено");
    std::string s = "Вызов отменён";
    mb.setText(s.c_str());
    mb.exec();
    saveJson(0);

    ui->cancelBtn->setEnabled(false);
    ui->sendBtn->setEnabled(false);
    ui->endCallBtn->setEnabled(true);
}


void Oper::on_endCallBtn_clicked() // Посылает сигнал, что звонок окончен
{
    emit endCall();
}

