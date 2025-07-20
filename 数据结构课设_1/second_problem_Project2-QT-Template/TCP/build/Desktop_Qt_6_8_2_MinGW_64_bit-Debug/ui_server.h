/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Server
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit_read;
    QTextEdit *textEdit_write;
    QPushButton *pushButton_send;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_close;

    void setupUi(QWidget *Server)
    {
        if (Server->objectName().isEmpty())
            Server->setObjectName("Server");
        Server->resize(498, 407);
        gridLayout = new QGridLayout(Server);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        textEdit_read = new QTextEdit(Server);
        textEdit_read->setObjectName("textEdit_read");
        textEdit_read->setReadOnly(true);

        gridLayout->addWidget(textEdit_read, 0, 0, 1, 3);

        textEdit_write = new QTextEdit(Server);
        textEdit_write->setObjectName("textEdit_write");

        gridLayout->addWidget(textEdit_write, 1, 0, 1, 3);

        pushButton_send = new QPushButton(Server);
        pushButton_send->setObjectName("pushButton_send");
        pushButton_send->setMinimumSize(QSize(0, 27));

        gridLayout->addWidget(pushButton_send, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(315, 28, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        pushButton_close = new QPushButton(Server);
        pushButton_close->setObjectName("pushButton_close");
        pushButton_close->setMinimumSize(QSize(0, 27));

        gridLayout->addWidget(pushButton_close, 2, 2, 1, 1);


        retranslateUi(Server);

        QMetaObject::connectSlotsByName(Server);
    } // setupUi

    void retranslateUi(QWidget *Server)
    {
        Server->setWindowTitle(QCoreApplication::translate("Server", "Widget", nullptr));
        pushButton_send->setText(QCoreApplication::translate("Server", "\345\217\221 \351\200\201", nullptr));
        pushButton_close->setText(QCoreApplication::translate("Server", "\345\205\263 \351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Server: public Ui_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
