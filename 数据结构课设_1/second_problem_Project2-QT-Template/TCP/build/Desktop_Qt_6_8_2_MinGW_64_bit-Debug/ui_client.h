/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit_write;
    QPushButton *pushButton_close;
    QLineEdit *lineEdit_ip;
    QPushButton *pushButton_stop;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_port;
    QPushButton *pushButton_connect;
    QTextEdit *textEdit_read;
    QPushButton *pushButton_send;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName("Client");
        Client->resize(430, 396);
        gridLayout = new QGridLayout(Client);
        gridLayout->setObjectName("gridLayout");
        textEdit_write = new QTextEdit(Client);
        textEdit_write->setObjectName("textEdit_write");

        gridLayout->addWidget(textEdit_write, 3, 0, 1, 4);

        pushButton_close = new QPushButton(Client);
        pushButton_close->setObjectName("pushButton_close");
        pushButton_close->setMinimumSize(QSize(0, 27));

        gridLayout->addWidget(pushButton_close, 4, 3, 1, 1);

        lineEdit_ip = new QLineEdit(Client);
        lineEdit_ip->setObjectName("lineEdit_ip");
        lineEdit_ip->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(lineEdit_ip, 1, 1, 1, 1);

        pushButton_stop = new QPushButton(Client);
        pushButton_stop->setObjectName("pushButton_stop");

        gridLayout->addWidget(pushButton_stop, 4, 1, 1, 1);

        label = new QLabel(Client);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(Client);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_port = new QLineEdit(Client);
        lineEdit_port->setObjectName("lineEdit_port");
        lineEdit_port->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(lineEdit_port, 0, 1, 1, 1);

        pushButton_connect = new QPushButton(Client);
        pushButton_connect->setObjectName("pushButton_connect");
        pushButton_connect->setMinimumSize(QSize(0, 28));

        gridLayout->addWidget(pushButton_connect, 0, 3, 2, 1);

        textEdit_read = new QTextEdit(Client);
        textEdit_read->setObjectName("textEdit_read");

        gridLayout->addWidget(textEdit_read, 2, 0, 1, 4);

        pushButton_send = new QPushButton(Client);
        pushButton_send->setObjectName("pushButton_send");
        pushButton_send->setMinimumSize(QSize(0, 27));

        gridLayout->addWidget(pushButton_send, 4, 0, 1, 1);


        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "Form", nullptr));
        pushButton_close->setText(QCoreApplication::translate("Client", "\346\226\255 \345\274\200", nullptr));
        pushButton_stop->setText(QCoreApplication::translate("Client", "\344\270\255\346\255\242", nullptr));
        label->setText(QCoreApplication::translate("Client", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Client", "\346\234\215\345\212\241\345\231\250IP\357\274\232", nullptr));
        pushButton_connect->setText(QCoreApplication::translate("Client", "\350\277\236 \346\216\245", nullptr));
        pushButton_send->setText(QCoreApplication::translate("Client", "\345\217\221 \351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
