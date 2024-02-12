#include "online_player_item.h"
#include "ui_online_player_item.h"

OnlinePlayerItem::OnlinePlayerItem(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::OnlinePlayerItem)
{
    ui->setupUi(this);

    username = QFrame::findChild<QLabel*>("username");
    elo = QFrame::findChild<QLabel*>("elo");
    win_lose = QFrame::findChild<QLabel*>("win_lose");
    challenge_btn = QFrame::findChild<QPushButton*>("challenge");
}

OnlinePlayerItem::~OnlinePlayerItem()
{
    delete ui;
}

void OnlinePlayerItem::setUserData(const QString &username, int elo, int wins, double winRate, int losses)
{
    // Set the data to the UI elements in the OnlinePlayerItem widget
    this->username->setText(username);
    this->elo->setText("elo: " + QString::number(elo) + " - win rate: " + QString::number(winRate));
    this->win_lose->setText("wins: " + QString::number(wins) + " - loses: " + QString::number(losses));

    connect(this->challenge_btn, &QPushButton::clicked, this, [this, username](){
        emit inviteButtonClicked(username);
    });
}
