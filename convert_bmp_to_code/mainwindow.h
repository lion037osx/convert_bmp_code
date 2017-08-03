#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    //void read_bmp(const QString& filename_bmp);

    QString global_filename;

private slots:

    void on_toolButton_open_file_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_run_clicked();

private:

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
