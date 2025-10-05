/********************************************************************************
** Form generated from reading UI file 'formregression.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMREGRESSION_H
#define UI_FORMREGRESSION_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_FormRegression
{
public:
    QGridLayout *gridLayout_6;
    QFrame *frame_SecondBlock;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QLabel *label_Sy2;
    QLabel *label_SyMean;
    QLabel *label_Sregr;
    QLabel *label_block1_3;
    QLabel *label_R2Concl;
    QLabel *label_Spoln;
    QSpacerItem *horizontalSpacer_6;
    QTextEdit *textEdit_RegCon;
    QTextEdit *textEdit_RegCoef;
    QLabel *label_Sost;
    QLabel *label_Sx2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_SxMean;
    QLabel *label_n;
    QLabel *label_R2;
    QLabel *label_MSE;
    QLabel *label_Forecast;
    QDateEdit *dateEdit;
    QPushButton *pushButton_Forecast;
    QLabel *label_Conclusion;
    QLabel *label_block1_2;
    QLabel *label_predict;
    QTableView *tableView_CalcPredicts;
    QFrame *frame_ThirdBlock;
    QGridLayout *gridLayout_4;
    QLabel *label_graphic;
    QCustomPlot *QCustomPlot_graphic;
    QFrame *frame_FirstBlock;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_meanY;
    QPushButton *pushButton_Back;
    QTextEdit *textEdit_Sums;
    QLabel *label_meanX;
    QLabel *label_block1;
    QTableView *tableView_Calculations;

    void setupUi(QWidget *FormRegression)
    {
        if (FormRegression->objectName().isEmpty())
            FormRegression->setObjectName("FormRegression");
        FormRegression->resize(1000, 700);
        FormRegression->setMinimumSize(QSize(1000, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8("regression_icon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        FormRegression->setWindowIcon(icon);
        FormRegression->setStyleSheet(QString::fromUtf8(""));
        gridLayout_6 = new QGridLayout(FormRegression);
        gridLayout_6->setObjectName("gridLayout_6");
        frame_SecondBlock = new QFrame(FormRegression);
        frame_SecondBlock->setObjectName("frame_SecondBlock");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_SecondBlock->sizePolicy().hasHeightForWidth());
        frame_SecondBlock->setSizePolicy(sizePolicy);
        frame_SecondBlock->setStyleSheet(QString::fromUtf8("background-color: #d9daff;"));
        frame_SecondBlock->setFrameShape(QFrame::Shape::StyledPanel);
        frame_SecondBlock->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_5 = new QGridLayout(frame_SecondBlock);
        gridLayout_5->setObjectName("gridLayout_5");
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        label_Sy2 = new QLabel(frame_SecondBlock);
        label_Sy2->setObjectName("label_Sy2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_Sy2->sizePolicy().hasHeightForWidth());
        label_Sy2->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(9);
        font.setBold(false);
        label_Sy2->setFont(font);
        label_Sy2->setCursor(QCursor(Qt::IBeamCursor));
        label_Sy2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Sy2->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Sy2, 1, 3, 1, 1);

        label_SyMean = new QLabel(frame_SecondBlock);
        label_SyMean->setObjectName("label_SyMean");
        sizePolicy1.setHeightForWidth(label_SyMean->sizePolicy().hasHeightForWidth());
        label_SyMean->setSizePolicy(sizePolicy1);
        label_SyMean->setFont(font);
        label_SyMean->setCursor(QCursor(Qt::IBeamCursor));
        label_SyMean->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_SyMean->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_SyMean, 2, 3, 1, 1);

        label_Sregr = new QLabel(frame_SecondBlock);
        label_Sregr->setObjectName("label_Sregr");
        sizePolicy1.setHeightForWidth(label_Sregr->sizePolicy().hasHeightForWidth());
        label_Sregr->setSizePolicy(sizePolicy1);
        label_Sregr->setFont(font);
        label_Sregr->setCursor(QCursor(Qt::IBeamCursor));
        label_Sregr->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Sregr->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Sregr, 1, 4, 1, 1);

        label_block1_3 = new QLabel(frame_SecondBlock);
        label_block1_3->setObjectName("label_block1_3");
        sizePolicy1.setHeightForWidth(label_block1_3->sizePolicy().hasHeightForWidth());
        label_block1_3->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setItalic(true);
        label_block1_3->setFont(font1);
        label_block1_3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_block1_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_3->addWidget(label_block1_3, 5, 0, 1, 5);

        label_R2Concl = new QLabel(frame_SecondBlock);
        label_R2Concl->setObjectName("label_R2Concl");
        sizePolicy1.setHeightForWidth(label_R2Concl->sizePolicy().hasHeightForWidth());
        label_R2Concl->setSizePolicy(sizePolicy1);
        label_R2Concl->setFont(font);
        label_R2Concl->setCursor(QCursor(Qt::IBeamCursor));
        label_R2Concl->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_R2Concl->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_R2Concl->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_R2Concl, 4, 0, 1, 5);

        label_Spoln = new QLabel(frame_SecondBlock);
        label_Spoln->setObjectName("label_Spoln");
        sizePolicy1.setHeightForWidth(label_Spoln->sizePolicy().hasHeightForWidth());
        label_Spoln->setSizePolicy(sizePolicy1);
        label_Spoln->setFont(font);
        label_Spoln->setCursor(QCursor(Qt::IBeamCursor));
        label_Spoln->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Spoln->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Spoln, 3, 4, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 3, 3, 1, 1);

        textEdit_RegCon = new QTextEdit(frame_SecondBlock);
        textEdit_RegCon->setObjectName("textEdit_RegCon");
        textEdit_RegCon->setEnabled(true);
        sizePolicy.setHeightForWidth(textEdit_RegCon->sizePolicy().hasHeightForWidth());
        textEdit_RegCon->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Times New Roman")});
        font2.setPointSize(9);
        textEdit_RegCon->setFont(font2);
        textEdit_RegCon->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        textEdit_RegCon->setMouseTracking(false);
        textEdit_RegCon->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	background-color:  #97cbc3;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"	max-height: 80px;\n"
"}"));
        textEdit_RegCon->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(textEdit_RegCon, 0, 0, 1, 3);

        textEdit_RegCoef = new QTextEdit(frame_SecondBlock);
        textEdit_RegCoef->setObjectName("textEdit_RegCoef");
        sizePolicy.setHeightForWidth(textEdit_RegCoef->sizePolicy().hasHeightForWidth());
        textEdit_RegCoef->setSizePolicy(sizePolicy);
        textEdit_RegCoef->setFont(font2);
        textEdit_RegCoef->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        textEdit_RegCoef->setMouseTracking(false);
        textEdit_RegCoef->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	background-color: #d8bcc4;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"	max-height: 80px;\n"
"}"));
        textEdit_RegCoef->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(textEdit_RegCoef, 0, 3, 1, 2);

        label_Sost = new QLabel(frame_SecondBlock);
        label_Sost->setObjectName("label_Sost");
        sizePolicy1.setHeightForWidth(label_Sost->sizePolicy().hasHeightForWidth());
        label_Sost->setSizePolicy(sizePolicy1);
        label_Sost->setFont(font);
        label_Sost->setCursor(QCursor(Qt::IBeamCursor));
        label_Sost->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Sost->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Sost, 2, 4, 1, 1);

        label_Sx2 = new QLabel(frame_SecondBlock);
        label_Sx2->setObjectName("label_Sx2");
        sizePolicy1.setHeightForWidth(label_Sx2->sizePolicy().hasHeightForWidth());
        label_Sx2->setSizePolicy(sizePolicy1);
        label_Sx2->setFont(font);
        label_Sx2->setCursor(QCursor(Qt::IBeamCursor));
        label_Sx2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Sx2->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Sx2, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        label_SxMean = new QLabel(frame_SecondBlock);
        label_SxMean->setObjectName("label_SxMean");
        sizePolicy1.setHeightForWidth(label_SxMean->sizePolicy().hasHeightForWidth());
        label_SxMean->setSizePolicy(sizePolicy1);
        label_SxMean->setFont(font);
        label_SxMean->setCursor(QCursor(Qt::IBeamCursor));
        label_SxMean->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_SxMean->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_SxMean, 2, 2, 1, 1);

        label_n = new QLabel(frame_SecondBlock);
        label_n->setObjectName("label_n");
        sizePolicy1.setHeightForWidth(label_n->sizePolicy().hasHeightForWidth());
        label_n->setSizePolicy(sizePolicy1);
        label_n->setFont(font);
        label_n->setCursor(QCursor(Qt::IBeamCursor));
        label_n->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_n->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_n, 1, 0, 1, 2);

        label_R2 = new QLabel(frame_SecondBlock);
        label_R2->setObjectName("label_R2");
        sizePolicy1.setHeightForWidth(label_R2->sizePolicy().hasHeightForWidth());
        label_R2->setSizePolicy(sizePolicy1);
        label_R2->setFont(font);
        label_R2->setCursor(QCursor(Qt::IBeamCursor));
        label_R2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_R2->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_R2, 2, 0, 1, 2);

        label_MSE = new QLabel(frame_SecondBlock);
        label_MSE->setObjectName("label_MSE");
        sizePolicy1.setHeightForWidth(label_MSE->sizePolicy().hasHeightForWidth());
        label_MSE->setSizePolicy(sizePolicy1);
        label_MSE->setFont(font);
        label_MSE->setCursor(QCursor(Qt::IBeamCursor));
        label_MSE->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_MSE->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_MSE, 3, 0, 1, 2);

        label_Forecast = new QLabel(frame_SecondBlock);
        label_Forecast->setObjectName("label_Forecast");
        sizePolicy1.setHeightForWidth(label_Forecast->sizePolicy().hasHeightForWidth());
        label_Forecast->setSizePolicy(sizePolicy1);
        label_Forecast->setFont(font);
        label_Forecast->setCursor(QCursor(Qt::IBeamCursor));
        label_Forecast->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #9ad499;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Forecast->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_Forecast->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_3->addWidget(label_Forecast, 6, 1, 2, 3);

        dateEdit = new QDateEdit(frame_SecondBlock);
        dateEdit->setObjectName("dateEdit");
        sizePolicy1.setHeightForWidth(dateEdit->sizePolicy().hasHeightForWidth());
        dateEdit->setSizePolicy(sizePolicy1);
        dateEdit->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        dateEdit->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    background-color: #eeeeee;\n"
"    color: #000000;\n"
"    border: 2px solid;\n"
"    border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QDateEdit::drop-down {\n"
"     width: 0px;\n"
"     border: none;\n"
"}\n"
""));
        dateEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
        dateEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);

        gridLayout_3->addWidget(dateEdit, 6, 0, 2, 1);

        pushButton_Forecast = new QPushButton(frame_SecondBlock);
        pushButton_Forecast->setObjectName("pushButton_Forecast");
        sizePolicy1.setHeightForWidth(pushButton_Forecast->sizePolicy().hasHeightForWidth());
        pushButton_Forecast->setSizePolicy(sizePolicy1);
        pushButton_Forecast->setFont(font1);
        pushButton_Forecast->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_Forecast->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        gridLayout_3->addWidget(pushButton_Forecast, 6, 4, 2, 1);


        gridLayout_5->addLayout(gridLayout_3, 4, 1, 1, 1);

        label_Conclusion = new QLabel(frame_SecondBlock);
        label_Conclusion->setObjectName("label_Conclusion");
        sizePolicy1.setHeightForWidth(label_Conclusion->sizePolicy().hasHeightForWidth());
        label_Conclusion->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Times New Roman")});
        font3.setPointSize(9);
        font3.setBold(false);
        font3.setItalic(true);
        label_Conclusion->setFont(font3);
        label_Conclusion->setCursor(QCursor(Qt::IBeamCursor));
        label_Conclusion->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #d5fffe;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_Conclusion->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_Conclusion->setWordWrap(true);
        label_Conclusion->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout_5->addWidget(label_Conclusion, 1, 1, 1, 1);

        label_block1_2 = new QLabel(frame_SecondBlock);
        label_block1_2->setObjectName("label_block1_2");
        sizePolicy1.setHeightForWidth(label_block1_2->sizePolicy().hasHeightForWidth());
        label_block1_2->setSizePolicy(sizePolicy1);
        label_block1_2->setFont(font1);
        label_block1_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_block1_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_5->addWidget(label_block1_2, 0, 0, 1, 2);

        label_predict = new QLabel(frame_SecondBlock);
        label_predict->setObjectName("label_predict");
        sizePolicy1.setHeightForWidth(label_predict->sizePolicy().hasHeightForWidth());
        label_predict->setSizePolicy(sizePolicy1);
        label_predict->setFont(font1);
        label_predict->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_predict->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_5->addWidget(label_predict, 1, 0, 1, 1);

        tableView_CalcPredicts = new QTableView(frame_SecondBlock);
        tableView_CalcPredicts->setObjectName("tableView_CalcPredicts");
        sizePolicy.setHeightForWidth(tableView_CalcPredicts->sizePolicy().hasHeightForWidth());
        tableView_CalcPredicts->setSizePolicy(sizePolicy);
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Segoe UI")});
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setItalic(false);
        tableView_CalcPredicts->setFont(font4);
        tableView_CalcPredicts->setStyleSheet(QString::fromUtf8("QTableView { \n"
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
"} \n"
"QTableView::item:selected { \n"
" 	background-color: #93c5fd;\n"
"	color: #0c1a2c; \n"
"} \n"
"QTableView::item:hover \n"
"{ \n"
"	background-color: #dbeafe; \n"
"}\n"
"\n"
""));
        tableView_CalcPredicts->setFrameShadow(QFrame::Shadow::Raised);
        tableView_CalcPredicts->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView_CalcPredicts->setAlternatingRowColors(true);
        tableView_CalcPredicts->setShowGrid(true);
        tableView_CalcPredicts->setGridStyle(Qt::PenStyle::DashLine);

        gridLayout_5->addWidget(tableView_CalcPredicts, 3, 0, 2, 1);


        gridLayout_6->addWidget(frame_SecondBlock, 0, 1, 1, 1);

        frame_ThirdBlock = new QFrame(FormRegression);
        frame_ThirdBlock->setObjectName("frame_ThirdBlock");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_ThirdBlock->sizePolicy().hasHeightForWidth());
        frame_ThirdBlock->setSizePolicy(sizePolicy2);
        frame_ThirdBlock->setStyleSheet(QString::fromUtf8("background-color: #d9daff;"));
        frame_ThirdBlock->setFrameShape(QFrame::Shape::StyledPanel);
        frame_ThirdBlock->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_4 = new QGridLayout(frame_ThirdBlock);
        gridLayout_4->setObjectName("gridLayout_4");
        label_graphic = new QLabel(frame_ThirdBlock);
        label_graphic->setObjectName("label_graphic");
        sizePolicy1.setHeightForWidth(label_graphic->sizePolicy().hasHeightForWidth());
        label_graphic->setSizePolicy(sizePolicy1);
        label_graphic->setFont(font1);
        label_graphic->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_graphic->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_4->addWidget(label_graphic, 4, 0, 1, 2);

        QCustomPlot_graphic = new QCustomPlot(frame_ThirdBlock);
        QCustomPlot_graphic->setObjectName("QCustomPlot_graphic");
        sizePolicy2.setHeightForWidth(QCustomPlot_graphic->sizePolicy().hasHeightForWidth());
        QCustomPlot_graphic->setSizePolicy(sizePolicy2);
        QCustomPlot_graphic->setStyleSheet(QString::fromUtf8("QWidget { \n"
"	background-color: #fdfdfd; \n"
"	alternate-background-color: #f0f4f8; \n"
"	gridline-color: #d1dbe5; font: 10pt \"Segoe UI\"; \n"
"	color: #102a43;\n"
"	border: 2px solid darkgray;\n"
"	border-radius: 5px;\n"
"} "));

        gridLayout_4->addWidget(QCustomPlot_graphic, 5, 0, 1, 2);


        gridLayout_6->addWidget(frame_ThirdBlock, 1, 1, 1, 1);

        frame_FirstBlock = new QFrame(FormRegression);
        frame_FirstBlock->setObjectName("frame_FirstBlock");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_FirstBlock->sizePolicy().hasHeightForWidth());
        frame_FirstBlock->setSizePolicy(sizePolicy3);
        frame_FirstBlock->setStyleSheet(QString::fromUtf8("background-color: #d9daff;"));
        frame_FirstBlock->setFrameShape(QFrame::Shape::StyledPanel);
        frame_FirstBlock->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_2 = new QGridLayout(frame_FirstBlock);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        label_meanY = new QLabel(frame_FirstBlock);
        label_meanY->setObjectName("label_meanY");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_meanY->sizePolicy().hasHeightForWidth());
        label_meanY->setSizePolicy(sizePolicy4);
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Times New Roman")});
        font5.setPointSize(10);
        font5.setBold(false);
        label_meanY->setFont(font5);
        label_meanY->setCursor(QCursor(Qt::IBeamCursor));
        label_meanY->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_meanY->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout->addWidget(label_meanY, 1, 1, 1, 1);

        pushButton_Back = new QPushButton(frame_FirstBlock);
        pushButton_Back->setObjectName("pushButton_Back");
        sizePolicy4.setHeightForWidth(pushButton_Back->sizePolicy().hasHeightForWidth());
        pushButton_Back->setSizePolicy(sizePolicy4);
        pushButton_Back->setFont(font1);
        pushButton_Back->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_Back->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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

        gridLayout->addWidget(pushButton_Back, 2, 0, 1, 1);

        textEdit_Sums = new QTextEdit(frame_FirstBlock);
        textEdit_Sums->setObjectName("textEdit_Sums");
        textEdit_Sums->setEnabled(true);
        sizePolicy3.setHeightForWidth(textEdit_Sums->sizePolicy().hasHeightForWidth());
        textEdit_Sums->setSizePolicy(sizePolicy3);
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Times New Roman")});
        font6.setPointSize(10);
        textEdit_Sums->setFont(font6);
        textEdit_Sums->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        textEdit_Sums->setMouseTracking(false);
        textEdit_Sums->setStyleSheet(QString::fromUtf8("QTextEdit {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"	max-height: 100px;\n"
"}"));
        textEdit_Sums->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout->addWidget(textEdit_Sums, 0, 0, 1, 2);

        label_meanX = new QLabel(frame_FirstBlock);
        label_meanX->setObjectName("label_meanX");
        sizePolicy4.setHeightForWidth(label_meanX->sizePolicy().hasHeightForWidth());
        label_meanX->setSizePolicy(sizePolicy4);
        label_meanX->setFont(font5);
        label_meanX->setCursor(QCursor(Qt::IBeamCursor));
        label_meanX->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #eeeeee;\n"
"	color: #000000;\n"
"	border: 2px solid;\n"
"	border-top: 2px solid gray;\n"
"    border-left: 2px solid gray;\n"
"    border-bottom: 2px solid darkgray;\n"
"    border-right: 2px solid darkgray;\n"
"	border-radius: 10px;\n"
"	padding: 5px;\n"
"}"));
        label_meanX->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextSelectableByMouse);

        gridLayout->addWidget(label_meanX, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

        label_block1 = new QLabel(frame_FirstBlock);
        label_block1->setObjectName("label_block1");
        sizePolicy4.setHeightForWidth(label_block1->sizePolicy().hasHeightForWidth());
        label_block1->setSizePolicy(sizePolicy4);
        label_block1->setFont(font1);
        label_block1->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	background-color: #909CBE;\n"
"	color: white;\n"
"	border: 2px solid #B1B6DB;\n"
"	border-radius: 12px;\n"
"	padding: 5px 10px;\n"
"}"));
        label_block1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_2->addWidget(label_block1, 0, 0, 1, 1);

        tableView_Calculations = new QTableView(frame_FirstBlock);
        tableView_Calculations->setObjectName("tableView_Calculations");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(tableView_Calculations->sizePolicy().hasHeightForWidth());
        tableView_Calculations->setSizePolicy(sizePolicy5);
        tableView_Calculations->setFont(font4);
        tableView_Calculations->setStyleSheet(QString::fromUtf8("QTableView { \n"
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
"} \n"
"QTableView::item:selected { \n"
" 	background-color: #93c5fd;\n"
"	color: #0c1a2c; \n"
"} \n"
"QTableView::item:hover \n"
"{ \n"
"	background-color: #dbeafe; \n"
"}\n"
"\n"
""));
        tableView_Calculations->setFrameShadow(QFrame::Shadow::Raised);
        tableView_Calculations->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView_Calculations->setAlternatingRowColors(true);
        tableView_Calculations->setShowGrid(true);
        tableView_Calculations->setGridStyle(Qt::PenStyle::DashLine);

        gridLayout_2->addWidget(tableView_Calculations, 1, 0, 1, 1);


        gridLayout_6->addWidget(frame_FirstBlock, 0, 0, 2, 1);


        retranslateUi(FormRegression);

        QMetaObject::connectSlotsByName(FormRegression);
    } // setupUi

    void retranslateUi(QWidget *FormRegression)
    {
        FormRegression->setWindowTitle(QCoreApplication::translate("FormRegression", "Form", nullptr));
        label_Sy2->setText(QCoreApplication::translate("FormRegression", "(Sy)^2 =", nullptr));
        label_SyMean->setText(QCoreApplication::translate("FormRegression", "Sy_\321\201\321\200. = ", nullptr));
        label_Sregr->setText(QCoreApplication::translate("FormRegression", "S\321\200\320\265\320\263\321\200. = ", nullptr));
        label_block1_3->setText(QCoreApplication::translate("FormRegression", "\320\243\320\272\320\260\320\267\320\260\320\275\320\270\320\265 \320\261\321\203\320\264\321\203\321\211\320\265\320\271 \320\264\320\260\321\202\321\213 \320\270 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        label_R2Concl->setText(QCoreApplication::translate("FormRegression", "\320\222\320\276\320\267\320\274\320\276\320\266\320\275\320\276\321\201\321\202\321\214 \320\277\321\200\320\276\320\263\320\275\320\276\320\267\320\260 (>=75%): ", nullptr));
        label_Spoln->setText(QCoreApplication::translate("FormRegression", "S\320\277\320\276\320\273\320\275. =", nullptr));
        textEdit_RegCon->setHtml(QCoreApplication::translate("FormRegression", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI';\">\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\320\276\320\275\320\275\320\276\320\271 \321\201\320\262\321\217\320\267\320\270</span></p></body></html>", nullptr));
        textEdit_RegCoef->setHtml(QCoreApplication::translate("FormRegression", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\321\213 \321\200\320\265\320\263\321\200\320\265\321\201\321\201\320\270\320\270 \320\270 \320\262\321\201\320\277\320\276\320\274\320\276\320\263\320\260\321\202\320\265\320\273\321\214\320\275\321\213\320\265 \320\262\320\265\320\273\320\270\321\207\320\270\320\275\321\213</p></body></html>", nullptr));
        label_Sost->setText(QCoreApplication::translate("FormRegression", "S\320\276\321\201\321\202. =", nullptr));
        label_Sx2->setText(QCoreApplication::translate("FormRegression", "(Sx)^2 = ", nullptr));
        label_SxMean->setText(QCoreApplication::translate("FormRegression", "Sx_\321\201\321\200. = ", nullptr));
        label_n->setText(QCoreApplication::translate("FormRegression", "n =", nullptr));
        label_R2->setText(QCoreApplication::translate("FormRegression", "R^2 = ", nullptr));
        label_MSE->setText(QCoreApplication::translate("FormRegression", "MSE =", nullptr));
        label_Forecast->setText(QCoreApplication::translate("FormRegression", "\320\237\321\200\320\276\320\263\320\275\320\276\320\267 \320\272\321\203\321\200\321\201\320\260 \320\225\320\262\321\200\320\276: \320\275\320\265 \320\276\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275.", nullptr));
        pushButton_Forecast->setText(QCoreApplication::translate("FormRegression", "\320\240\320\260\321\201\321\201\321\207\320\270\321\202\320\260\321\202\321\214", nullptr));
        label_Conclusion->setText(QCoreApplication::translate("FormRegression", "\320\230\321\202\320\276\320\263\320\276\320\262\320\276\320\265 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265", nullptr));
        label_block1_2->setText(QCoreApplication::translate("FormRegression", "\320\243\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\265 \320\270 \320\272\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\321\213", nullptr));
        label_predict->setText(QCoreApplication::translate("FormRegression", "\320\237\321\200\320\265\320\264\321\201\320\272\320\260\320\267\320\260\320\275\320\270\321\217 \320\277\320\276 \320\274\320\276\320\264\320\265\320\273\320\270", nullptr));
        label_graphic->setText(QCoreApplication::translate("FormRegression", "\320\223\321\200\320\260\321\204\320\270\320\272", nullptr));
        label_meanY->setText(QCoreApplication::translate("FormRegression", "mean(y) =", nullptr));
        pushButton_Back->setText(QCoreApplication::translate("FormRegression", "\320\235\320\260\320\267\320\260\320\264", nullptr));
        textEdit_Sums->setHtml(QCoreApplication::translate("FormRegression", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">\320\241\321\203\320\274\320\274\321\213 \320\270 \320\277\321\200\320\276\320\270\320\267\320\262\320\265\320\264\320\265\320\275\320\270\321\217</span></p></body></html>", nullptr));
        label_meanX->setText(QCoreApplication::translate("FormRegression", "mean(x) =", nullptr));
        label_block1->setText(QCoreApplication::translate("FormRegression", "\320\242\320\260\320\261\320\273\320\270\321\206\320\260 \320\270 \321\200\320\260\321\201\321\207\320\265\321\202\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormRegression: public Ui_FormRegression {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMREGRESSION_H
