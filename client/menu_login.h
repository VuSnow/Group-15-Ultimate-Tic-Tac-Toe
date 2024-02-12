#ifndef MENU_LOGIN_H
#define MENU_LOGIN_H

#include <QWidget>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "TcpClient.h"
#include <QTimer>
#include <QListView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include "game_screen.h"

namespace Ui {
class mainmenulogin;
}

class mainmenulogin : public QWidget
{
    Q_OBJECT

public:
    explicit mainmenulogin(QWidget *parent = nullptr);
    ~mainmenulogin();

    void setClient(TcpClient *client);
    void getClient();
    void updateOnlinePlayersList(const std::vector<user>& newOnlineUserList);

private slots:
    void on_logout_clicked();
    void on_CreateRoom_clicked();
    void on_cancel_clicked();
    void on_create_clicked();
    void on_JoinRoom_clicked();
    void on_cancel_3_clicked();
    void on_join_clicked();
    void handleServerResponse(const QByteArray& responseData);
    void on_EditProfile_clicked();
    void on_pushButton_clicked();
    void inviteButtonClicked(const QString &username);

private:
    Ui::mainmenulogin *ui;
    QFrame *createRoom;
    QFrame *joinRoom;
    QFrame *profile;
    QLineEdit *createRoomInput;
    QLineEdit *joinRoomInput;
    QLabel *warning_create;
    QLabel *warning_join;
    QTimer *timer;
    QLabel *username;
    QLabel *ingame;
    QLabel *wins;
    QLabel *loses;
    QLabel *winrate;
    QLabel *status;
    QListView *onlinePlayersListView;
    QStandardItemModel *onlinePlayersModel;

    TcpClient *client;

};

#endif // MENU_LOGIN_H
