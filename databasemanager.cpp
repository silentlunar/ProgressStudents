#include "databasemanager.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QRandomGenerator>
#include <QSqlError>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("academic_results.db");
}

bool DatabaseManager::connect()
{
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть БД: " + db.lastError().text());
        return false;
    }

    createTables();
    fillWithData();
    return true;
}

void DatabaseManager::createTables()
{
    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS Студенты ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "ФИО TEXT NOT NULL,"
               "Группа TEXT NOT NULL,"
               "Курс INTEGER NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS Дисциплины ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "Название TEXT NOT NULL,"
               "Преподаватель TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS Результаты ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "id_студента INTEGER NOT NULL,"
               "id_дисциплины INTEGER NOT NULL,"
               "Оценка INTEGER NOT NULL,"
               "Дата TEXT NOT NULL,"
               "FOREIGN KEY(id_студента) REFERENCES Студенты(id),"
               "FOREIGN KEY(id_дисциплины) REFERENCES Дисциплины(id))");
}

void DatabaseManager::fillWithData()
{
    QSqlQuery query;

    // Заполняем студентов
    QStringList students = {
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Петров Иван Сергеевич', 'ЗП-221', 1)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Смирнова Анна Дмитриевна', 'ЗП-221', 1)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Козлов Дмитрий Петрович', 'ЗП-222', 1)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Новикова Елена Владимировна', 'ЗП-222', 1)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Васильев Алексей Иванович', 'ЗП-201', 2)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Павлова Ольга Сергеевна', 'ЗП-201', 2)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Федоров Михаил Андреевич', 'ЗП-202', 2)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Морозова Татьяна Викторовна', 'ЗП-202', 2)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Белов Артем Николаевич', 'ЗП-301', 3)",
        "INSERT INTO Студенты (ФИО, Группа, Курс) VALUES ('Кудрявцева Наталья Игоревна', 'ЗП-301', 3)"
    };

    foreach (const QString &q, students) {
        query.exec(q);
    }

    // Заполняем дисциплины
    QStringList courses = {
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Программирование', 'Иванова Г.А.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Математика', 'Петров С.Н.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Физика', 'Сидорова М.В.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Базы данных', 'Кузнецов А.П.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Алгоритмы', 'Николаев Д.К.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Сети', 'Фролов И.Б.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('ООП', 'Максимова Е.С.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Веб-технологии', 'Андреев П.М.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Иностранный язык', 'Белова Л.И.')",
        "INSERT INTO Дисциплины (Название, Преподаватель) VALUES ('Физкультура', 'Соколов В.Д.')"
    };

    foreach (const QString &q, courses) {
        query.exec(q);
    }

    // Заполняем результаты
    for (int s = 1; s <= 10; ++s) {
        for (int c = 1; c <= 10; ++c) {
            int grade = QRandomGenerator::global()->bounded(2, 6); // Оценка от 2 до 5
            QString date = QString("2023-%1-%2")
                          .arg(QRandomGenerator::global()->bounded(1, 13), 2, 10, QChar('0'))
                          .arg(QRandomGenerator::global()->bounded(1, 29), 2, 10, QChar('0'));

            query.prepare("INSERT INTO Результаты (id_студента, id_дисциплины, Оценка, Дата) "
                          "VALUES (?, ?, ?, ?)");
            query.addBindValue(s);
            query.addBindValue(c);
            query.addBindValue(grade);
            query.addBindValue(date);
            query.exec();
        }
    }
}
