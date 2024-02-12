#ifndef ONLINE_PLAYER_ITEM_H
#define ONLINE_PLAYER_ITEM_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>


namespace Ui {
class OnlinePlayerItem;
}

class OnlinePlayerItem : public QFrame
{
    Q_OBJECT

public:
    explicit OnlinePlayerItem(QWidget *parent = nullptr);
    ~OnlinePlayerItem();

    void setUserData(const QString &username, int elo, int wins, double winRate, int losses);

signals:
    void inviteButtonClicked(const QString &username); // Declare the signal

private:
    Ui::OnlinePlayerItem *ui;
    QLabel *username;
    QLabel *elo;
    QLabel *win_lose;
    QPushButton *challenge_btn;

};

#endif // ONLINE_PLAYER_ITEM_H
