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
    // деактивация кнопок
    ui->pushButton_LinearReg->setEnabled(false);
    ui->pushButton_InverseLinearReg->setEnabled(false);
    ui->pushButton_ParabReg->setEnabled(false);
    ui->pushButton_ExpReg->setEnabled(false);
    ui->pushButton_HyperReg->setEnabled(false);
    ui->pushButton_LogReg->setEnabled(false);
    ui->pushButton_PowerReg->setEnabled(false);
    ui->pushButton_PolynomReg->setEnabled(false);
    // Подключение сигналов
    connect(ui->pushButton_LinearReg, &QPushButton::clicked, this, &MainWindow::openRegressionForm);
    connect(ui->pushButton_InverseLinearReg, &QPushButton::clicked, this, &MainWindow::openRegressionForm);


    connect(ui->pushButton_ParabReg, &QPushButton::clicked, this, &MainWindow::openRegressionForm);


    connect(ui->pushButton_PolynomReg, &QPushButton::clicked, this, &MainWindow::openRegressionForm);
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
    if (!path.isEmpty() && loadDataFromExcel(path, ui->tableView_TableData))
    {
        ui->label_FilePath->setText("Путь к файлу: " + path);

        ui->pushButton_LinearReg->setEnabled(true);
        ui->pushButton_InverseLinearReg->setEnabled(true);
        ui->pushButton_ParabReg->setEnabled(true);
        ui->pushButton_ExpReg->setEnabled(true);
        ui->pushButton_HyperReg->setEnabled(true);
        ui->pushButton_LogReg->setEnabled(true);
        ui->pushButton_PowerReg->setEnabled(true);
        ui->pushButton_PolynomReg->setEnabled(true);

        ui->label_FilePath->setCursor(Qt::IBeamCursor);
        ui->label_FilePath->setTextInteractionFlags(Qt::TextSelectableByMouse);
    }
    else
    {
        ui->tableView_TableData->setModel(nullptr);
        ui->label_FilePath->clear();
        ui->label_FilePath->setCursor(Qt::ArrowCursor);
        ui->label_FilePath->setTextInteractionFlags(Qt::NoTextInteraction);
        // деактивация кнопок
        ui->pushButton_LinearReg->setEnabled(false);
        ui->pushButton_InverseLinearReg->setEnabled(false);
        ui->pushButton_ParabReg->setEnabled(false);
        ui->pushButton_ExpReg->setEnabled(false);
        ui->pushButton_HyperReg->setEnabled(false);
        ui->pushButton_LogReg->setEnabled(false);
        ui->pushButton_PowerReg->setEnabled(false);
        ui->pushButton_PolynomReg->setEnabled(false);
    }
}

void MainWindow::show_window()
{
    this->show();
}

void MainWindow::openRegressionForm()
{
    // Определение нажатой кнопки
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int idx{};
    if (button->objectName() == "pushButton_LinearReg")
        idx = 0;
    else if (button->objectName() == "pushButton_InverseLinearReg")
        idx = 1;
    else if (button->objectName() == "pushButton_ParabReg")
        idx = 4;
    else if (button->objectName() == "pushButton_PolynomReg")
        idx = 7;

    // Переменные для записи эксперементальных данных из .xlsx файла
    QVector<QString> dataColumns{};
    QVector<double> numericDates{};
    QVector<double> cursValues{};
    // Проверка загруженной таблицы перед построением регрессионной модели.
    bool check = readDataAndCurs(ui->tableView_TableData, dataColumns, numericDates, cursValues);
    if (check)
    {

        FormRegression *regression_form = new FormRegression(idx, dataColumns, numericDates, cursValues);
        connect(regression_form, &FormRegression::backToMain, this, &MainWindow::show_window);
        this->hide();
        regression_form->show();
    }
}
