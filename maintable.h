#ifndef MAINTABLE_H
#define MAINTABLE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QVector>
#include <QTableWidgetItem>
#include <QLabel>
#include <QHeaderView>
#include <QString>
#include <QTextStream>
#include <QPushButton>
#include <QIcon>

const QStringList currencies = {"$", "€", "£"};
const char del = '\t';

struct Row
{
    QTableWidgetItem *name;
    QTableWidgetItem *buyDate;
    QTableWidgetItem *buyCost;
    QLabel *lbuyCost;
    QTableWidgetItem *sellDate;
    QTableWidgetItem *sellCost;
    QLabel *lsellCost;
    QTableWidgetItem *stopUp;
    QLabel *lstopUp;
    QTableWidgetItem *stopLoss;
    QLabel *lstopLoss;
    QLabel *relRisk;
    QLabel *rewardRisk;
    QLabel *absProfit;
    QLabel *labsProfit;
    QLabel *relProfit;
    QLabel *afterBank;
    QLabel *lafterBank;
    bool ongoing;
};

class MainTable : public QTableWidget
{
    Q_OBJECT

public:
    MainTable(QWidget *parent=nullptr);
    ~MainTable();
    QString recalculate(const QString &startBank);
    void setCurrency(const QString &curr);
    const QString &getCurrency() const;
    void clear();
    void open(QTextStream &in);
    QString save() const;
    double sumCost;
    double runningCost;
    double sumProfit;
    bool changed = false;

private slots:
    void addRow();
    void removRow();
    void sortByColumn_(int i);

private:
    QString currency = "$";
    QVector<Row *> rows;
    void createHeader();
};

#endif // MAINTABLE_H
