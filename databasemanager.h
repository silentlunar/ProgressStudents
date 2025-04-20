#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>  // Добавлено
#include <QSqlQuery>     // Добавлено
#include <QSqlError>     // Добавлено

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool connect(); // Подключение и инициализация БД

private:
    void createTables();   // Создание таблиц
    void fillWithData();   // Заполнение данными

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
