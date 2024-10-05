#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include "CredsTextBox.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // UsernameTB
    void onMouseOver();
protected:
    void resizeEvent(QResizeEvent *) override;
private slots:
    void on_loginButton_clicked();
};
#endif // MAINWINDOW_H
