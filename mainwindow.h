#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

#include <QDir>
#include <inverted_index.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_comboBox_Directory_editTextChanged(const QString &arg1);

    void on_popDirButton_clicked();

private:
    Ui::MainWindow *ui;
    bool preBuildInverted(QDir dir);
    inverted_index inv_index;
    pseudo_inverted_index p_ii;
};
#endif // MAINWINDOW_H
