#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainTable = new MainTable(this);
    setCentralWidget(mainTable);

    QLabel* l1 = new QLabel;
    l1->setText("Start bank:");
    l1->setParent(this);
    l1->setAlignment(Qt::AlignRight);
    l1->setGeometry(220, 4, 100, 15);
    l1->show();

    startBank = new QLineEdit;
    startBank->setParent(this);
    startBank->setAlignment(Qt::AlignRight);
    startBank->setGeometry(320, 3, 100, 18);
    startBank->show();

    QComboBox* l3 = new QComboBox;
    l3->insertItems(0, currencies);
    l3->setCurrentText(mainTable->getCurrency());
    l3->setParent(this);
    l3->setGeometry(420, 3, 40, 18);
    l3->show();

    connect(l3, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrency(int)));

    QLabel* l4 = new QLabel;
    l4->setText("Running bank:");
    l4->setParent(this);
    l4->setAlignment(Qt::AlignRight);
    l4->setGeometry(460, 4, 100, 15);
    l4->show();

    runningBank = new QLabel;
    runningBank->setParent(this);
    runningBank->setAlignment(Qt::AlignRight);
    runningBank->setGeometry(560, 4, 100, 15);
    runningBank->show();

    l6 = new QLabel;
    l6->setText(mainTable->getCurrency());
    l6->setParent(this);
    l6->setAlignment(Qt::AlignLeft);
    l6->setGeometry(660, 4, 20, 15);
    l6->show();

    connect(ui->action_Row, SIGNAL(triggered()), this->mainTable, SLOT(addRow()));
    connect(ui->action_Row_2, SIGNAL(triggered()), this->mainTable, SLOT(removRow()));

    QShortcut *sh = new QShortcut(QKeySequence(tr("Enter")), this);
    connect(sh, SIGNAL(activated()), this, SLOT(recalculate()));
    QShortcut *sh2 = new QShortcut(QKeySequence(tr("Ctrl+R")), this);
    connect(sh2, SIGNAL(activated()), this, SLOT(recalculate()));

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsFile()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionRecalculate, SIGNAL(triggered()), this, SLOT(recalculate()));

    button = new QPushButton(this);
    button->setMinimumSize(5, 5);
    button->setIcon(QIcon("./icons/recalculateIcon.png"));
    button->setToolTip("Recalculate Ctrl+R, Enter");
    button->setGeometry(680, 3, 18, 18);
    button->show();

    connect(button, SIGNAL(clicked()), this, SLOT(recalculate()));

    setWindowIcon(QIcon("./icons/icon.png"));
    setWindowTitle("Trading App - New File");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCurrency(const QString &currency)
{
    mainTable->setCurrency(currency);
    l6->setText(currency);
}

void MainWindow::clear() const
{
    mainTable->clear();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if (mainTable->changed)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exit"),
                                                                    tr("There are unsaved changes.\nAre you sure to exit?\n"),
                                                                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (resBtn == QMessageBox::Save)
        {
            event->ignore();
            saveFile();
        } else if (resBtn == QMessageBox::Discard)
        {
            event->accept();
        } else
        {
            event->ignore();
        }
    } else
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exit"),
                                                                    tr("Are you sure to exit?\n"),
                                                                    QMessageBox::Yes | QMessageBox::Cancel);
        if (resBtn != QMessageBox::Yes)
        {
            event->ignore();
        } else
        {
            event->accept();
        }
    }
}

void MainWindow::openFile()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open"), "",
                                                 tr("Text files (*.txt);;Any files (*);;"));
    if (s.isNull())
        return;
    QFile file(s);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    fileName = s;
    QTextStream in(&file);
    QString line = in.readLine();
    startBank->setText(line);
    mainTable->open(in);
    recalculate();
    mainTable->changed = false;
    setWindowTitle("Trading App - " + fileName.split("/").last());
}

void MainWindow::saveFile()
{
    if (fileName == "")
    {
        saveAsFile();
    } else
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QIODevice::Text))
             return;

        QTextStream out(&file);
        out << startBank->text() << '\n';

        out << mainTable->save();

        file.close();

        QMessageBox msgBox;
        msgBox.setText("Save");
        msgBox.setInformativeText("The file has been saved");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        mainTable->changed = false;
        setWindowTitle("Trading App - " + fileName.split("/").last());
    }
}

void MainWindow::saveAsFile()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save As.."), QDir::home().absolutePath(),
                                                 tr("Text files (*.txt);;Any files (*);;"));
    if (!fileName.isNull())
    {
        saveFile();
    }
}

void MainWindow::recalculate()
{
    runningBank->setText(mainTable->recalculate(startBank->text()));
    ui->label_2->setText(QString("%2").arg(mainTable->sumCost, 0, 'f', 2));
    ui->label_5->setText(QString("%2").arg(mainTable->sumProfit, 0, 'f', 2));
    ui->label_7->setText(QString("(%2 %)").arg(mainTable->sumProfit / mainTable->sumCost*100, 0, 'f', 2));
    ui->label_11->setText(QString("%2").arg(mainTable->runningCost, 0, 'f', 2));
    mainTable->changed = true;
}

void MainWindow::changeCurrency(int id)
{
    setCurrency(currencies.at(id));
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setText("This is just funny app.");
    msgBox.exec();
}
