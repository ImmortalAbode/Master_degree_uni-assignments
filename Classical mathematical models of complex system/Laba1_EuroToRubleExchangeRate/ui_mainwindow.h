/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QFrame *frame_title;
    QGridLayout *gridLayout;
    QLabel *label_Task;
    QLabel *label_Laba1;
    QGridLayout *gridLayout_3;
    QLabel *label_info;
    QLabel *label_info_link;
    QLabel *label_link;
    QLabel *label_uploadedFile;
    QTableView *tableView_TableData;
    QFrame *frame_Work;
    QGridLayout *gridLayout_2;
    QLabel *label_TypesOfRegressions;
    QHBoxLayout *horizontalLayout_Regressions;
    QVBoxLayout *verticalLayout_Block1;
    QPushButton *pushButton_LinearReg;
    QPushButton *pushButton_ParabReg;
    QVBoxLayout *verticalLayout_Block2;
    QPushButton *pushButton_InverseLinearReg;
    QPushButton *pushButton_LogReg;
    QVBoxLayout *verticalLayout_Block3;
    QPushButton *pushButton_ExpReg;
    QPushButton *pushButton_PowerReg;
    QVBoxLayout *verticalLayout_Block4;
    QPushButton *pushButton_HyperReg;
    QPushButton *pushButton_PolynomReg;
    QPushButton *pushButton_loadFile;
    QLabel *label_FilePath;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        MainWindow->setMinimumSize(QSize(1000, 700));
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow{\n"
"	 background-color: rgb(213, 218, 241);\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName("gridLayout_4");
        frame_title = new QFrame(centralwidget);
        frame_title->setObjectName("frame_title");
        frame_title->setStyleSheet(QString::fromUtf8("QFrame {\n"
"	background-color: #CCD6E9;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"}"));
        frame_title->setFrameShape(QFrame::Shape::StyledPanel);
        frame_title->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout = new QGridLayout(frame_title);
        gridLayout->setObjectName("gridLayout");
        label_Task = new QLabel(frame_title);
        label_Task->setObjectName("label_Task");
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(12);
        font.setItalic(true);
        label_Task->setFont(font);
        label_Task->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_Task->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_Task, 1, 0, 1, 1);

        label_Laba1 = new QLabel(frame_title);
        label_Laba1->setObjectName("label_Laba1");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Times New Roman")});
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setItalic(false);
        label_Laba1->setFont(font1);
        label_Laba1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #9595CD;\n"
"	background: qlineargradient(\n"
"        x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #3498DB,		  \n"
"        stop:1 #9B59B6			\n"
"    );\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 10px 10px;\n"
"}"));
        label_Laba1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_Laba1, 0, 0, 1, 1);


        gridLayout_4->addWidget(frame_title, 0, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        label_info = new QLabel(centralwidget);
        label_info->setObjectName("label_info");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Times New Roman")});
        font2.setPointSize(10);
        font2.setItalic(false);
        label_info->setFont(font2);
        label_info->setCursor(QCursor(Qt::ArrowCursor));
        label_info->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: #ffffff;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_info->setFrameShadow(QFrame::Shadow::Plain);
        label_info->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_info->setWordWrap(true);
        label_info->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

        gridLayout_3->addWidget(label_info, 0, 2, 2, 1);

        label_info_link = new QLabel(centralwidget);
        label_info_link->setObjectName("label_info_link");
        label_info_link->setFont(font2);
        label_info_link->setCursor(QCursor(Qt::ArrowCursor));
        label_info_link->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: #ffffff;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"	max-width: 100px;\n"
"}"));
        label_info_link->setFrameShadow(QFrame::Shadow::Plain);
        label_info_link->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_info_link->setWordWrap(true);
        label_info_link->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

        gridLayout_3->addWidget(label_info_link, 1, 0, 1, 1);

        label_link = new QLabel(centralwidget);
        label_link->setObjectName("label_link");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Segoe UI")});
        font3.setPointSize(9);
        font3.setWeight(QFont::ExtraBold);
        font3.setItalic(true);
        font3.setUnderline(true);
        label_link->setFont(font3);
        label_link->setCursor(QCursor(Qt::IBeamCursor));
        label_link->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-color: rgb(255, 255, 255);\n"
"	color: rgb(0, 0, 127);\n"
"    font-family: \"Segoe UI\";\n"
"    font-size: 9pt;\n"
"    font-weight: 800;\n"
"    padding: 5px;\n"
"	max-width: 300px;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"}\n"
""));
        label_link->setFrameShadow(QFrame::Shadow::Plain);
        label_link->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_link->setWordWrap(true);
        label_link->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_link, 1, 1, 1, 1);

        label_uploadedFile = new QLabel(centralwidget);
        label_uploadedFile->setObjectName("label_uploadedFile");
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Times New Roman")});
        font4.setPointSize(10);
        font4.setItalic(true);
        label_uploadedFile->setFont(font4);
        label_uploadedFile->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_uploadedFile->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_3->addWidget(label_uploadedFile, 0, 0, 1, 2);


        gridLayout_4->addLayout(gridLayout_3, 1, 0, 1, 1);

        tableView_TableData = new QTableView(centralwidget);
        tableView_TableData->setObjectName("tableView_TableData");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableView_TableData->sizePolicy().hasHeightForWidth());
        tableView_TableData->setSizePolicy(sizePolicy);
        tableView_TableData->setStyleSheet(QString::fromUtf8("QTableView { \n"
"	background-color: #fdfdfd; \n"
"	alternate-background-color: #f0f4f8; \n"
"	gridline-color: #d1dbe5; font: 10pt \"Segoe UI\"; \n"
"	color: #102a43;\n"
"	border: 2px solid darkgray;\n"
"	border-radius: 5px;\n"
"} \n"
"QHeaderView::section { \n"
"	background-color: #9334ff; \n"
"	color: white; \n"
"	padding: 4px; \n"
"	border: 1px solid #6925b6; \n"
"	font-weight: bold;\n"
"	text-align: center;\n"
"} \n"
"QTableView::item:selected { \n"
" 	background-color: #93c5fd;\n"
"	color: #0c1a2c; \n"
"} \n"
"QTableView::item:hover \n"
"{ \n"
"	background-color: #dbeafe; \n"
"}"));
        tableView_TableData->setFrameShadow(QFrame::Shadow::Raised);
        tableView_TableData->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView_TableData->setAlternatingRowColors(true);
        tableView_TableData->setShowGrid(true);
        tableView_TableData->setGridStyle(Qt::PenStyle::DashLine);
        tableView_TableData->horizontalHeader()->setCascadingSectionResizes(false);
        tableView_TableData->horizontalHeader()->setStretchLastSection(false);

        gridLayout_4->addWidget(tableView_TableData, 2, 0, 1, 1);

        frame_Work = new QFrame(centralwidget);
        frame_Work->setObjectName("frame_Work");
        frame_Work->setStyleSheet(QString::fromUtf8("QFrame {\n"
"	background-color: #CCD6E9;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"}"));
        frame_Work->setFrameShape(QFrame::Shape::StyledPanel);
        frame_Work->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame_Work);
        gridLayout_2->setObjectName("gridLayout_2");
        label_TypesOfRegressions = new QLabel(frame_Work);
        label_TypesOfRegressions->setObjectName("label_TypesOfRegressions");
        label_TypesOfRegressions->setFont(font4);
        label_TypesOfRegressions->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_TypesOfRegressions->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(label_TypesOfRegressions, 3, 0, 1, 2);

        horizontalLayout_Regressions = new QHBoxLayout();
        horizontalLayout_Regressions->setObjectName("horizontalLayout_Regressions");
        verticalLayout_Block1 = new QVBoxLayout();
        verticalLayout_Block1->setObjectName("verticalLayout_Block1");
        pushButton_LinearReg = new QPushButton(frame_Work);
        pushButton_LinearReg->setObjectName("pushButton_LinearReg");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_LinearReg->sizePolicy().hasHeightForWidth());
        pushButton_LinearReg->setSizePolicy(sizePolicy1);
        QFont font5;
        font5.setItalic(true);
        pushButton_LinearReg->setFont(font5);
        pushButton_LinearReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_LinearReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block1->addWidget(pushButton_LinearReg);

        pushButton_ParabReg = new QPushButton(frame_Work);
        pushButton_ParabReg->setObjectName("pushButton_ParabReg");
        sizePolicy1.setHeightForWidth(pushButton_ParabReg->sizePolicy().hasHeightForWidth());
        pushButton_ParabReg->setSizePolicy(sizePolicy1);
        pushButton_ParabReg->setFont(font5);
        pushButton_ParabReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_ParabReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block1->addWidget(pushButton_ParabReg);


        horizontalLayout_Regressions->addLayout(verticalLayout_Block1);

        verticalLayout_Block2 = new QVBoxLayout();
        verticalLayout_Block2->setObjectName("verticalLayout_Block2");
        pushButton_InverseLinearReg = new QPushButton(frame_Work);
        pushButton_InverseLinearReg->setObjectName("pushButton_InverseLinearReg");
        sizePolicy1.setHeightForWidth(pushButton_InverseLinearReg->sizePolicy().hasHeightForWidth());
        pushButton_InverseLinearReg->setSizePolicy(sizePolicy1);
        pushButton_InverseLinearReg->setFont(font5);
        pushButton_InverseLinearReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_InverseLinearReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block2->addWidget(pushButton_InverseLinearReg);

        pushButton_LogReg = new QPushButton(frame_Work);
        pushButton_LogReg->setObjectName("pushButton_LogReg");
        sizePolicy1.setHeightForWidth(pushButton_LogReg->sizePolicy().hasHeightForWidth());
        pushButton_LogReg->setSizePolicy(sizePolicy1);
        pushButton_LogReg->setFont(font5);
        pushButton_LogReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_LogReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block2->addWidget(pushButton_LogReg);


        horizontalLayout_Regressions->addLayout(verticalLayout_Block2);

        verticalLayout_Block3 = new QVBoxLayout();
        verticalLayout_Block3->setObjectName("verticalLayout_Block3");
        pushButton_ExpReg = new QPushButton(frame_Work);
        pushButton_ExpReg->setObjectName("pushButton_ExpReg");
        sizePolicy1.setHeightForWidth(pushButton_ExpReg->sizePolicy().hasHeightForWidth());
        pushButton_ExpReg->setSizePolicy(sizePolicy1);
        pushButton_ExpReg->setFont(font5);
        pushButton_ExpReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_ExpReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block3->addWidget(pushButton_ExpReg);

        pushButton_PowerReg = new QPushButton(frame_Work);
        pushButton_PowerReg->setObjectName("pushButton_PowerReg");
        sizePolicy1.setHeightForWidth(pushButton_PowerReg->sizePolicy().hasHeightForWidth());
        pushButton_PowerReg->setSizePolicy(sizePolicy1);
        pushButton_PowerReg->setFont(font5);
        pushButton_PowerReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_PowerReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block3->addWidget(pushButton_PowerReg);


        horizontalLayout_Regressions->addLayout(verticalLayout_Block3);

        verticalLayout_Block4 = new QVBoxLayout();
        verticalLayout_Block4->setObjectName("verticalLayout_Block4");
        pushButton_HyperReg = new QPushButton(frame_Work);
        pushButton_HyperReg->setObjectName("pushButton_HyperReg");
        sizePolicy1.setHeightForWidth(pushButton_HyperReg->sizePolicy().hasHeightForWidth());
        pushButton_HyperReg->setSizePolicy(sizePolicy1);
        pushButton_HyperReg->setFont(font5);
        pushButton_HyperReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_HyperReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block4->addWidget(pushButton_HyperReg);

        pushButton_PolynomReg = new QPushButton(frame_Work);
        pushButton_PolynomReg->setObjectName("pushButton_PolynomReg");
        sizePolicy1.setHeightForWidth(pushButton_PolynomReg->sizePolicy().hasHeightForWidth());
        pushButton_PolynomReg->setSizePolicy(sizePolicy1);
        pushButton_PolynomReg->setFont(font5);
        pushButton_PolynomReg->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_PolynomReg->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        verticalLayout_Block4->addWidget(pushButton_PolynomReg);


        horizontalLayout_Regressions->addLayout(verticalLayout_Block4);


        gridLayout_2->addLayout(horizontalLayout_Regressions, 4, 0, 1, 2);

        pushButton_loadFile = new QPushButton(frame_Work);
        pushButton_loadFile->setObjectName("pushButton_loadFile");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_loadFile->sizePolicy().hasHeightForWidth());
        pushButton_loadFile->setSizePolicy(sizePolicy2);
        pushButton_loadFile->setFont(font5);
        pushButton_loadFile->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_loadFile->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #000000;\n"
"    background-color: #9595CD; \n"
"    border-radius: 12px; \n"
"    border: 2px solid #BFC6E2;\n"
"    padding: 5px 10px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #AA55FF; \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #7B79AE;\n"
"}\n"
""));

        gridLayout_2->addWidget(pushButton_loadFile, 2, 1, 1, 1);

        label_FilePath = new QLabel(frame_Work);
        label_FilePath->setObjectName("label_FilePath");
        label_FilePath->setFont(font2);
        label_FilePath->setCursor(QCursor(Qt::ArrowCursor));
        label_FilePath->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_FilePath->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_FilePath->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_2->addWidget(label_FilePath, 2, 0, 1, 1);


        gridLayout_4->addWidget(frame_Work, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_Task->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\264\320\265\320\273\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\200\320\276\321\201\321\202\320\260 \320\272\321\203\321\200\321\201\320\260 \320\225\320\262\321\200\320\276 \320\272 \320\240\321\203\320\261\320\273\321\216", nullptr));
        label_Laba1->setText(QCoreApplication::translate("MainWindow", "\320\233\320\260\320\261\320\276\321\200\320\260\321\202\320\276\321\200\320\275\320\260\321\217 \321\200\320\260\320\261\320\276\321\202\320\260 \342\204\2261", nullptr));
        label_info->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\264\320\273\321\217 .xlsx \321\204\320\260\320\271\320\273\320\260 (Excel) \320\262\320\267\321\217\321\202\321\213 \320\270\320\267 \320\264\320\270\320\275\320\260\320\274\320\270\320\272\320\270 \320\276\321\204\320\270\321\206\320\270\320\260\320\273\321\214\320\275\320\276\320\263\320\276 \320\272\321\203\321\200\321\201\320\260 \320\267\320\260\320\264\320\260\320\275\320\275\320\276\320\271 \320\262\320\260\320\273\321\216\321\202\321\213 (\320\225\320\262\321\200\320\276) \321\201 \321\201\320\260\320\271\321\202\320\260 \320\246\320\221\320\240 \320\267\320\260 \321\203\320\272\320\260\320\267\320\260\320\275\320\275\321\213\320\271 \320\277\320\265\321\200\320\270\320\276\320\264.", nullptr));
        label_info_link->setText(QCoreApplication::translate("MainWindow", "\320\241\320\260\320\271\321\202 \320\246\320\221\320\240:", nullptr));
        label_link->setText(QCoreApplication::translate("MainWindow", "https://www.cbr.ru/currency_base/dynamics/", nullptr));
        label_uploadedFile->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\263\321\200\321\203\320\266\320\265\320\275\320\275\321\213\320\271 \321\204\320\260\320\271\320\273", nullptr));
        label_TypesOfRegressions->setText(QCoreApplication::translate("MainWindow", "\320\242\320\230\320\237\320\253 \320\240\320\225\320\223\320\240\320\225\320\241\320\241\320\230\320\231", nullptr));
        pushButton_LinearReg->setText(QCoreApplication::translate("MainWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_ParabReg->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\261\320\276\320\273\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_InverseLinearReg->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\200\320\260\321\202\320\275\320\260\321\217 \320\273\320\270\320\275\320\265\320\271\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_LogReg->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\260\321\200\320\270\321\204\320\274\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_ExpReg->setText(QCoreApplication::translate("MainWindow", "\320\255\320\272\321\201\320\277\320\276\320\275\320\265\320\275\321\206\320\270\320\260\320\273\321\214\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_PowerReg->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\265\320\277\320\265\320\275\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_HyperReg->setText(QCoreApplication::translate("MainWindow", "\320\223\320\270\320\277\320\265\321\200\320\261\320\276\320\273\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_PolynomReg->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\320\270\320\275\320\276\320\274\320\270\320\260\320\273\321\214\320\275\320\260\321\217 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\321\217", nullptr));
        pushButton_loadFile->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        label_FilePath->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
