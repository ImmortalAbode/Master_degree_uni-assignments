#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFileDialog"
#include "myfuncs.h"
#include "formregression.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_LinearReg->setEnabled(false);
    ui->pushButton_InverseLinearReg->setEnabled(false);
    ui->pushButton_ParabReg->setEnabled(false);
    ui->pushButton_ExpReg->setEnabled(false);
    ui->pushButton_HyperReg->setEnabled(false);
    ui->pushButton_LogReg->setEnabled(false);
    ui->pushButton_PowerReg->setEnabled(false);
    ui->pushButton_PolynomReg->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_loadFile_clicked()
{
    QString path{};
    path = QFileDialog::getOpenFileName(this, "Выбрать файл", "D:\\Магистратура\\Классические математические модели сложных систем",
                                       "All Files (*.*);; XLSX Files (*.xlsx);");
    ui->label_FilePath->setText("Путь к файлу: " + path);
    if (!path.isEmpty() && loadDataFromExcel(path, ui->tableView_TableData))
    {
        ui->pushButton_LinearReg->setEnabled(true);
        ui->pushButton_InverseLinearReg->setEnabled(true);
        ui->pushButton_ParabReg->setEnabled(true);
        ui->pushButton_ExpReg->setEnabled(true);
        ui->pushButton_HyperReg->setEnabled(true);
        ui->pushButton_LogReg->setEnabled(true);
        ui->pushButton_PowerReg->setEnabled(true);
        ui->pushButton_PolynomReg->setEnabled(true);
    }
}

void MainWindow::show_window()
{
    this->show();
}

void MainWindow::on_pushButton_LinearReg_clicked()
{
    FormRegression *regression_form = new FormRegression(0, ui->tableView_TableData);
    connect(regression_form, &FormRegression::backToMain, this, &MainWindow::show_window);
    this->hide();
    regression_form->show();
}



