#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::buildInverted(QDir dir)
{
    bool done = false;

    QStringList files = dir.entryList(QStringList() << "*.txt" ,QDir::Files);
    foreach(QString filename, files) {
    QString absolute_file_path = dir.absoluteFilePath(filename);
    qDebug() << filename;
    QFile ifile(absolute_file_path);
        ifile.open(QIODevice::ReadOnly | QIODevice::Text);

        // read whole content
        QString content = ifile.readAll();
        // extract words
        QStringList list = content.split(" ");
        foreach(QString word,list)
            qDebug() <<word;
        ifile.close();

    }
    return !done;
}
void MainWindow::on_comboReplace_2_currentTextChanged(const QString &arg1)
{
    buildInverted(arg1);
}
