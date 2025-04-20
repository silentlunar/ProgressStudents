// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Учёт успеваемости");

    DatabaseManager dbManager;
    if (!dbManager.connect()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к БД");
        return;
    }

    // Настройка моделей
    studentsModel = new QSqlTableModel(this);
    studentsModel->setTable("Студенты");
    studentsModel->select();
    ui->studentsView->setModel(studentsModel);

    coursesModel = new QSqlTableModel(this);
    coursesModel->setTable("Дисциплины");
    coursesModel->select();
    ui->coursesView->setModel(coursesModel);

    resultsModel = new QSqlTableModel(this);
    resultsModel->setTable("Результаты");
    resultsModel->select();
    ui->resultsView->setModel(resultsModel);

    // Подключение кнопок
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddRecord);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteRecord);
    connect(ui->reportButton, &QPushButton::clicked, this, &MainWindow::onGenerateReport);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddRecord()
{
    int tab = ui->tabWidget->currentIndex();
    QSqlTableModel *model = nullptr;

    if (tab == 0) model = studentsModel;
    else if (tab == 1) model = coursesModel;
    else model = resultsModel;

    if (model) {
        model->insertRow(model->rowCount());
    }
}

void MainWindow::onDeleteRecord()
{
    int tab = ui->tabWidget->currentIndex();
    QTableView *view = nullptr;

    if (tab == 0) view = ui->studentsView;
    else if (tab == 1) view = ui->coursesView;
    else view = ui->resultsView;

    QModelIndexList selected = view->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления");
        return;
    }

    int row = selected.first().row();
    if (tab == 0) studentsModel->removeRow(row);
    else if (tab == 1) coursesModel->removeRow(row);
    else resultsModel->removeRow(row);
}

void MainWindow::onGenerateReport()
{
    QSqlQuery query;
    query.exec("SELECT s.ФИО, d.Название, AVG(r.Оценка) "
               "FROM Результаты r "
               "JOIN Студенты s ON r.id_студента = s.id "
               "JOIN Дисциплины d ON r.id_дисциплины = d.id "
               "GROUP BY s.id, d.id");

    QString report = "Средние баллы студентов:\n\n";
    while (query.next()) {
        report += QString("%1 | %2: %3\n")
                 .arg(query.value(0).toString())
                 .arg(query.value(1).toString())
                 .arg(query.value(2).toDouble(), 0, 'f', 2);
    }

    QMessageBox::information(this, "Отчёт", report);
}
