#include "maintable.h"

MainTable::MainTable(QWidget *parent) :
    QTableWidget(parent)
{
    createHeader();
    addRow();
    resizeColumnsToContents();
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn_(int)));
    setSortingEnabled(true);
}

MainTable::~MainTable()
{

}

void MainTable::setCurrency(const QString &curr)
{
    currency = curr;
    for (Row *r : rows)
    {
        r->lbuyCost->setText(currency);
        r->lsellCost->setText(currency);
        r->lstopUp->setText(currency);
        r->lstopLoss->setText(currency);
        r->labsProfit->setText(currency);
        r->lafterBank->setText(currency);
    }
}

const QString &MainTable::getCurrency() const
{
    return currency;
}

void MainTable::clear()
{
    while (rowCount() > 1)
    {
        removRow();
    }
}

void MainTable::open(QTextStream &in)
{
    QString line, word;
    int i, k = 0;
    while (!in.atEnd())
    {
        k++;
        line = in.readLine();
        i = 0;
        if (k > rowCount())
            addRow();
        word = "";
        for (QChar a : line)
        {
            if (a != del)
            {
                word.append(a);
            } else if (a == '\n')
            {
                break;
            } else {
                switch (i)
                {
                case 0:
                    rows.last()->name->setText(word);
                    break;
                case 1:
                    rows.last()->buyDate->setText(word);
                    break;
                case 2:
                    rows.last()->buyCost->setText(word);
                    break;
                case 3:
                    rows.last()->sellDate->setText(word);
                    break;
                case 4:
                    rows.last()->sellCost->setText(word);
                    break;
                case 5:
                    rows.last()->stopUp->setText(word);
                    break;
                case 6:
                    rows.last()->stopLoss->setText(word);
                    break;
                default:
                    break;
                }
                word = "";
                i++;
            }
        }
        if (i == 6)
            rows.last()->stopLoss->setText(word);
    }
}

QString MainTable::save() const
{
    QString out;
    for (Row *r : rows)
    {
        out += r->name->text() + del;
        out += r->buyDate->text() + del;
        out += r->buyCost->text() + del;
        out += r->sellDate->text() + del;
        out += r->sellCost->text() + del;
        out += r->stopUp->text() + del;
        out += r->stopLoss->text() + del;
        out.append('\n');
    }
    return out;
}

QString MainTable::recalculate(const QString &startBank)
{
    sumCost = 0;
    sumProfit = 0;
    runningCost = 0;
    bool ok;
    for (int i=0; i<rows.size(); i++)
    {
        rows[i]->stopLoss->text().toDouble(&ok);
        if (ok)
        {
            rows[i]->relRisk->setText(QString("%2 %").arg(rows[i]->stopLoss->text().toDouble() / rows[i]->buyCost->text().toDouble()*100, 0, 'f', 2));
            rows[i]->stopUp->text().toDouble(&ok);
            if (ok)
            {
                rows[i]->rewardRisk->setText(QString("%2 %").arg(rows[i]->stopUp->text().toDouble() / rows[i]->stopLoss->text().toDouble()*100, 0, 'f', 2));
            }
        }

        rows[i]->buyCost->text().toDouble(&ok);
        if (ok)
        {
            sumCost += rows[i]->buyCost->text().toDouble();
            rows[i]->sellCost->text().toDouble(&ok);
            if (ok)
            {
                rows[i]->ongoing = false;
                rows[i]->absProfit->setText(QString("%2").arg(rows[i]->sellCost->text().toDouble() - rows[i]->buyCost->text().toDouble(), 0, 'f', 2));
                rows[i]->relProfit->setText(QString("%2 %").arg(rows[i]->absProfit->text().toDouble()/rows[i]->buyCost->text().toDouble()*100, 0, 'f', 2));
                sumProfit += rows[i]->absProfit->text().toDouble();
            } else
            {
                rows[i]->ongoing = true;
                runningCost += rows[i]->buyCost->text().toDouble(&ok);
            }
        }
    }

    for (int i=0; i<rows.size(); i++)
    {
        rows[i]->absProfit->text().toDouble(&ok);
        if (ok)
        {
            if (i != 0)
            {
                rows[i]->afterBank->setText(QString("%2").arg(rows[i-1]->afterBank->text().toDouble() + rows[i]->absProfit->text().toDouble(), 0, 'f', 2));
            } else
            {
                rows[i]->afterBank->setText(QString("%2").arg(startBank.toDouble() + rows[i]->absProfit->text().toDouble(), 0, 'f', 2));
            }
        }
    }

    resizeColumnsToContents();
    short dates[] = {1, 4};
    short currencies[] = {3, 6, 8, 10, 14, 17};
    short costs[] = {2, 5, 7, 9};
    short others[] = {11, 12, 13, 15};
    for (short i : dates)
        setColumnWidth(i, 100);
    for (short i : currencies)
        setColumnWidth(i, 15);
    for (short i : costs)
        setColumnWidth(i, 80);
    for (short i : others)
        setColumnWidth(i, 100);
    setColumnWidth(16, 100);
    return rows.last()->afterBank->text();
}

void MainTable::addRow()
{
    int id = currentRow();
    if (id == -1)
    {
        id = rowCount() - 1;
    }
    id++;
    insertRow(id);

    // create new cells
    rows.append(new Row {
        new QTableWidgetItem("Name of company"),
        new QTableWidgetItem("Buy date"),
        new QTableWidgetItem("Buy cost"),
        new QLabel(currency, this),
        new QTableWidgetItem("Sell date"),
        new QTableWidgetItem("Sell cost"),
        new QLabel(currency, this),
        new QTableWidgetItem("Stop up"),
        new QLabel(currency, this),
        new QTableWidgetItem("Stop loss"),
        new QLabel(currency, this),
        new QLabel("0 %", this),
        new QLabel("0 %", this),
        new QLabel("0", this),
        new QLabel(currency, this),
        new QLabel("0 %", this),
        new QLabel("0", this),
        new QLabel(currency, this),
        true
    });

    rows.last()->buyDate->setTextAlignment(Qt::AlignCenter);
    rows.last()->buyCost->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->lbuyCost->setMaximumWidth(20);
    rows.last()->sellDate->setTextAlignment(Qt::AlignCenter);
    rows.last()->sellCost->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->lsellCost->setMaximumWidth(20);
    rows.last()->stopUp->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->lstopUp->setMaximumWidth(20);
    rows.last()->stopLoss->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->lstopLoss->setMaximumWidth(20);
    rows.last()->relRisk->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->rewardRisk->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->absProfit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->labsProfit->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    rows.last()->relProfit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->afterBank->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rows.last()->lafterBank->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    setItem(id, 0, rows.last()->name);
    setItem(id, 1, rows.last()->buyDate);
    setItem(id, 2, rows.last()->buyCost);
    setCellWidget(id, 3, rows.last()->lbuyCost);
    setItem(id, 4, rows.last()->sellDate);
    setItem(id, 5, rows.last()->sellCost);
    setCellWidget(id, 6, rows.last()->lsellCost);
    setItem(id, 7, rows.last()->stopUp);
    setCellWidget(id, 8, rows.last()->lstopUp);
    setItem(id, 9, rows.last()->stopLoss);
    setCellWidget(id, 10, rows.last()->lstopLoss);
    setCellWidget(id, 11, rows.last()->relRisk);
    setCellWidget(id, 12, rows.last()->rewardRisk);
    setCellWidget(id, 13, rows.last()->absProfit);
    setCellWidget(id, 14, rows.last()->labsProfit);
    setCellWidget(id, 15, rows.last()->relProfit);
    setCellWidget(id, 16, rows.last()->afterBank);
    setCellWidget(id, 17, rows.last()->lafterBank);
}

void MainTable::removRow()
{
    if (rowCount() == 1)
        return;
    int id = currentRow();
    if (id == -1)
    {
        id = rowCount();
    }
    for (int i=rows.size()-1; i>=0; i--)
    {
        if (rows[i]->name->text() == item(id, 0)->text() && rows[i]->buyDate->text() == item(id, 1)->text())
            rows.remove(i);
    }
    removeRow(id);
}

void MainTable::sortByColumn_(int i)
{
    if (horizontalHeader()->sortIndicatorOrder() == Qt::DescendingOrder)
        sortItems(i, Qt::DescendingOrder);
    else
        sortItems(i, Qt::AscendingOrder);
}

void MainTable::createHeader()
{
    setColumnCount(18);
    setHorizontalHeaderLabels({"Name", "Buy Date", "Buy Cost", "",
                               "Sell Date", "Sell Cost", "",
                               "Stop Up", "", "Stop Loss", "",
                               "Rel. Risk", "Reward/Risk",
                               "Abs. Profit", "", "Rel. Profit",
                               "Bank", ""});
}
