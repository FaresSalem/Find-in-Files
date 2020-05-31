#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::preBuildInverted(QDir dir)
{
    bool done = false;
    QString absolute_file = dir.absoluteFilePath(".ii");
    QFile file(absolute_file);
    if (!file.exists())
    {

    QStringList files = dir.entryList(QStringList() << "*.txt" ,QDir::Files);
    int filesCount=files.size();
    foreach(QString filename, files) {
        filesCount--;
        if (filesCount==0)
            done=true;
    QString absolute_file_path = dir.absoluteFilePath(filename);
    qDebug() << filename;
    QFile ifile(absolute_file_path);
        ifile.open(QIODevice::ReadOnly | QIODevice::Text);

        // read whole content
        QString content = ifile.readAll();
        // extract words
        QStringList list = content.split(" ");
            inv_index.build_inverted_index(list,filename,done);
        ifile.close();

    }
    inv_index.createDB(dir);
    }
    p_ii.returnDB(dir);
    return done;
}

void MainWindow::on_popDirButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString path;
    path = dialog.QFileDialog::getExistingDirectory();
    ui->comboBox_Directory->setEditText(path);
}

void MainWindow::on_buttonFindAll_clicked()
{
    QString path = ui->comboBox_Directory->currentText() + "/.ii";
    QFile file(path);

    // if .ii isn't built, call preBuildInverted(directory)
    if(!file.exists(path))
    {
        ui->statusbar->showMessage("Building ...");
        QString temp = ui->comboBox_Directory->currentText();
        preBuildInverted(temp);
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->tableWidget->clearContents();
        ui->statusbar->showMessage("File wasn't opened");
    }
    QTextStream file_text(&file);
    QString find_this = ui->comboBox_FindWhat->currentText();

    while(!file_text.atEnd())
    {
        ui->statusbar->showMessage("Searching ...");
        QString line = file_text.readLine();
        if (line == find_this)
        {
            ui->statusbar->showMessage("Found Successfully");
            QString found = file_text.readLine();
            QStringList list = found.split(" ");
            ui->tableWidget->setRowCount(list.length()/2);

            QStringList file_name;
            QString file_path;

            for (int row(0), i(0); row < list.length()/2; ++row)
            {
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(list[i]));

                QTableWidgetItem* word_count = new QTableWidgetItem(list[i+1]);
                word_count->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget->setItem(row, 1, word_count);
                i+=2;

                if (ui->AutoOpen->isChecked()){
                    QProcess *proc = new QProcess();
                    file_path = ui->comboBox_Directory->currentText();
                    file_path += "/";
                    if (list[i].length() > 1)
                    {
                        file_path += list[i];
                        file_name.clear();
                        file_name.append(file_path);

                        #ifdef WIN32
                        proc->start("notepad.exe", file_name);
                        #else   // use nano for UNIX
                        proc->start("code", file_name);
                        #endif
                    }
                }
            }


            return;
        }

        else
        {
            ui->tableWidget->clearContents();
            ui->statusbar->showMessage("Found nothing");
        }
    }
}

void MainWindow::on_tableWidget_cellDoubleClicked()
{
    if (ui->tableWidget->currentItem()->column() == 0)
    {
        QProcess *proc = new QProcess(this);
        QStringList file_name;
        QString file_path = ui->comboBox_Directory->currentText();
        file_path += "/";
        file_path += ui->tableWidget->currentItem()->text();
        file_name.append(file_path);

        #ifdef WIN32
        proc->start("notepad.exe", file_name);
        #else   // use nano for UNIX
        proc->start("code", file_name);
        #endif
    }
}

