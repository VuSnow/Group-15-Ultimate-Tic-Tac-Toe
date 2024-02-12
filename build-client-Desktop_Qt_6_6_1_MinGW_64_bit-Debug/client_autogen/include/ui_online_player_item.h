/********************************************************************************
** Form generated from reading UI file 'online_player_item.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINE_PLAYER_ITEM_H
#define UI_ONLINE_PLAYER_ITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OnlinePlayerItem
{
public:
    QLabel *avatar;
    QLabel *username;
    QLabel *elo;
    QPushButton *challenge;
    QLabel *win_lose;

    void setupUi(QFrame *OnlinePlayerItem)
    {
        if (OnlinePlayerItem->objectName().isEmpty())
            OnlinePlayerItem->setObjectName("OnlinePlayerItem");
        OnlinePlayerItem->resize(300, 60);
        avatar = new QLabel(OnlinePlayerItem);
        avatar->setObjectName("avatar");
        avatar->setGeometry(QRect(0, 15, 30, 30));
        avatar->setStyleSheet(QString::fromUtf8(""));
        avatar->setPixmap(QPixmap(QString::fromUtf8(":/user-solid.svg")));
        avatar->setScaledContents(false);
        avatar->setAlignment(Qt::AlignCenter);
        username = new QLabel(OnlinePlayerItem);
        username->setObjectName("username");
        username->setGeometry(QRect(30, 0, 191, 16));
        elo = new QLabel(OnlinePlayerItem);
        elo->setObjectName("elo");
        elo->setGeometry(QRect(30, 20, 191, 16));
        challenge = new QPushButton(OnlinePlayerItem);
        challenge->setObjectName("challenge");
        challenge->setGeometry(QRect(220, 5, 80, 50));
        win_lose = new QLabel(OnlinePlayerItem);
        win_lose->setObjectName("win_lose");
        win_lose->setGeometry(QRect(30, 40, 191, 16));

        retranslateUi(OnlinePlayerItem);

        QMetaObject::connectSlotsByName(OnlinePlayerItem);
    } // setupUi

    void retranslateUi(QFrame *OnlinePlayerItem)
    {
        OnlinePlayerItem->setWindowTitle(QCoreApplication::translate("OnlinePlayerItem", "Frame", nullptr));
        avatar->setText(QString());
        username->setText(QCoreApplication::translate("OnlinePlayerItem", "Username", nullptr));
        elo->setText(QCoreApplication::translate("OnlinePlayerItem", "Elo", nullptr));
        challenge->setText(QCoreApplication::translate("OnlinePlayerItem", "Challenge", nullptr));
        win_lose->setText(QCoreApplication::translate("OnlinePlayerItem", "Win-lose", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OnlinePlayerItem: public Ui_OnlinePlayerItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINE_PLAYER_ITEM_H
