#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddRecord();
    void onDeleteRecord();
    void onGenerateReport();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *studentsModel, *coursesModel, *resultsModel;
};

#endif // MAINWINDOW_H
