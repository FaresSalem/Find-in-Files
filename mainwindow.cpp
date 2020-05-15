#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>

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
            inv_index.build_inverted_index(list,filename,done);
//        foreach(QString word,list)
//            qDebug() <<word;
        ifile.close();

    }
    //inv_index.traverse_inverted_index();
    //inv_index.createDB(dir);
    inv_index.returnDB(dir);
    return !done;
}


void MainWindow::on_comboBox_Directory_editTextChanged(const QString &arg1)
{
    preBuildInverted(arg1);
}

void MainWindow::on_popDirButton_clicked()
{
    QFileDialog dialog(this);
    QString path;
    dialog.setFileMode(QFileDialog::Directory);
    //path = dialog.QFileDialog::getOpenFileName(this, tr("Browse For Folder"));
    path = dialog.QFileDialog::getExistingDirectory();
    ui->comboBox_Directory->setEditText(path);

    // we should start indexing when pressing Find All
    QProgressDialog progress(this);
    progress.show();
   // progress.setWindowModality(Qt::WindowModality);
}


