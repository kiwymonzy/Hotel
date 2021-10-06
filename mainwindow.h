#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers.h"



using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_pushButton_checkin_clicked();

    void on_pushButton_checkout_clicked();

    void on_tableView_room_activated(const QModelIndex &index);


    void on_pushButton_clicked();

    void on_exit_clicked();

    void on_report_clicked();

    void on_clear_checkin_clicked();

    void on_clear_checkout_clicked();

    void on_pushButton_time_clicked();

    void checkin();

    void checkout();

    void showTime();

    void loadCheckout();

    void checkRoom();

    void loadRoomType();

    void updateRoom();

    void updateDetails();

    void on_listView_activated(const QModelIndex &index);

    void on_pushButton_update_clicked();

    void UpdateCustomer();

private:
    Ui::MainWindow *ui;
    QSqlDatabase myDB;
    Report *report;
};
#endif // MAINWINDOW_H
