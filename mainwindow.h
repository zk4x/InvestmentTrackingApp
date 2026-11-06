#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShortcut>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QIcon>

#include "maintable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCurrency(const QString &currency);
    void clear() const;

private slots:
    void closeEvent(QCloseEvent *event);
    void openFile();
    void saveFile();
    void saveAsFile();
    void recalculate();
    void changeCurrency(int id);
    void about();

private:
    QPushButton *button;
    QLabel *l6;
    QString fileName = "";
    QLineEdit* startBank;
    QLabel* runningBank;
    Ui::MainWindow *ui;
    MainTable *mainTable;
};
#endif // MAINWINDOW_H
