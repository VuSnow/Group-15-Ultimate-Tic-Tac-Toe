#include "game_screen.h"
#include "qjsondocument.h"
#include "ui_game_screen.h"
#include <QLabel>
#include "menu_login.h"
#include <iostream>

Game_Screen::Game_Screen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game_Screen)
{
    ui->setupUi(this);

    noti = QMainWindow::findChild<QLabel*>("notification");
    noti->setVisible(false);

    surfr = QMainWindow::findChild<QFrame*>("surrenderfr");
    surfr->setVisible(false);

    setUpGrid();
    player1_name = QMainWindow::findChild<QLabel*>("name");
    player2_name = QMainWindow::findChild<QLabel*>("name_2");

    player2_elo = QMainWindow::findChild<QLabel*>("elo_2");
    player1_elo = QMainWindow::findChild<QLabel*>("elo");

    player1_win = QMainWindow::findChild<QLabel*>("win");
    player2_win = QMainWindow::findChild<QLabel*>("win_2");

    readyButton = QMainWindow::findChild<QPushButton*>("readyButton");

}

Game_Screen::~Game_Screen()
{
    delete ui;
}

void Game_Screen::setClient(TcpClient *client){
    this->client = client;
    connect(client, &TcpClient::dataReady, this, &Game_Screen::handleServerResponse);
    connect(client, &TcpClient::roomIn4Changed, this, &Game_Screen::handleRoomIn4Changed);
}

void Game_Screen::handleRoomIn4Changed(const room& newRoom){
    player1_name->setText(client->getRoomIn4().playerX.ingame);
    player1_elo->setText("Elo: " + QString::number(client->getRoomIn4().playerX.elo));
    player1_win->setText("Win: " + QString::number(client->getRoomIn4().playerX.wins) + " - " + QString::number(client->getRoomIn4().playerX.winRate) + "%");

    if(client->getRoomIn4().isFull){
        player2_name->setText(client->getRoomIn4().playerO.ingame);
        player2_elo->setText("Elo: " + QString::number(client->getRoomIn4().playerO.elo));
        player2_win->setText("Win: " + QString::number(client->getRoomIn4().playerO.wins) + " - " + QString::number(client->getRoomIn4().playerO.winRate) + "%");
    }

    if(client->getRoomIn4().player2_ready){
        if(client->getUser().username == client->getRoomIn4().playerX.username){
            readyButton->setText("START");
        }
    }else{
        if(client->getUser().username == client->getRoomIn4().playerX.username && !client->getRoomIn4().player1_ready){
            readyButton->setText("READY");
        }else if(client->getUser().username == client->getRoomIn4().playerX.username && client->getRoomIn4().player1_ready){
            readyButton->setText("UNREADY");
        }
    }

    if(!client->getRoomIn4().gameStart){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                itemButtons[i][j]->setDisabled(true);
            }
        }
    }else{
        readyButton->setText("SURRENDER");
        // first turn
        if(client->getRoomIn4().nextBoard == -2 && client->getRoomIn4().isPlayerXTurn && client->getUser().username == client->getRoomIn4().playerX.username){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    itemButtons[i][j]->setDisabled(false);
                }
            }
        }else if(client->getRoomIn4().nextBoard == -1){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    if(client->getRoomIn4().isPlayerXTurn){
                        if(client->getUser().username == client->getRoomIn4().playerX.username){
                            std::vector<int> win_boards = client->getRoomIn4().winning_board;
                            auto it = std::find(win_boards.begin(), win_boards.end(), i);
                            if(it != win_boards.end()){
                                itemButtons[i][j]->setDisabled(true);
                            }else{
                                itemButtons[i][j]->setDisabled(false);
                            }
                        }else{
                            itemButtons[i][j]->setDisabled(true);
                        }
                        itemButtons[client->getRoomIn4().last_board][client->getRoomIn4().last_cell]->setText("O");
                    }else{
                        if(client->getUser().username == client->getRoomIn4().playerO.username){
                            std::vector<int> win_boards = client->getRoomIn4().winning_board;
                            auto it = std::find(win_boards.begin(), win_boards.end(), i);
                            if(it != win_boards.end()){
                                itemButtons[i][j]->setDisabled(true);
                            }else{
                                itemButtons[i][j]->setDisabled(false);
                            }
                        }else if(client->getUser().username == client->getRoomIn4().playerX.username){
                            itemButtons[i][j]->setDisabled(true);
                        }
                        itemButtons[client->getRoomIn4().last_board][client->getRoomIn4().last_cell]->setText("X");
                    }
                }
            }
        }else if(client->getRoomIn4().nextBoard >= 0){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    if(client->getRoomIn4().isPlayerXTurn){
                        if(client->getUser().username == client->getRoomIn4().playerX.username){
                            if(i == client->getRoomIn4().nextBoard){
                                itemButtons[i][j]->setDisabled(false);
                            }else{
                                itemButtons[i][j]->setDisabled(true);
                            }
                        }else if(client->getUser().username == client->getRoomIn4().playerO.username){
                            itemButtons[i][j]->setDisabled(true);
                        }
                        itemButtons[client->getRoomIn4().last_board][client->getRoomIn4().last_cell]->setText("O");
                    }else{
                        if(client->getUser().username == client->getRoomIn4().playerO.username){
                            if(i == client->getRoomIn4().nextBoard){
                                itemButtons[i][j]->setDisabled(false);
                            }else{
                                itemButtons[i][j]->setDisabled(true);
                            }
                        }else if(client->getUser().username == client->getRoomIn4().playerX.username){
                            itemButtons[i][j]->setDisabled(true);

                        }
                        itemButtons[client->getRoomIn4().last_board][client->getRoomIn4().last_cell]->setText("X");
                    }
                }
            }
        }
    }
}

void Game_Screen::handleServerResponse(const QByteArray& responseData){
    QJsonDocument responseJson = QJsonDocument::fromJson(responseData);
    if(responseJson["type"] == static_cast<int>(ResponseType::READY)){
        std::vector<room> room_list = client->getRoomList();
        QString room_name = responseJson["room name"].toString();
        QString player_ready = responseJson["player username"].toString();

        for(room &value: room_list){
            if(value.roomName == room_name){
                if(value.playerX.username == player_ready){
                    value.player1_ready = true;
                }
                if(value.playerO.username == player_ready){
                    value.player2_ready = true;
                }
                value.playerX = client->findUserByUsername(value.playerX.username);
                value.playerO = client->findUserByUsername(value.playerO.username);
            }
        }
        client->setRoomList(room_list);
        room update_room = client->findRoomByRoomName(room_name);

        if(client->getUser().username == update_room.playerX.username || client->getUser().username == update_room.playerO.username){
            client->setRoomIn4(update_room);
        }
    }else if(responseJson["type"] == static_cast<int>(ResponseType::UNREADY)){
        std::vector<room> room_list = client->getRoomList();
        QString room_name = responseJson["room name"].toString();
        QString player_ready = responseJson["player username"].toString();

        for(room &value: room_list){
            if(value.roomName == room_name){
                if(value.playerX.username == player_ready){
                    value.player1_ready = false;
                }
                if(value.playerO.username == player_ready){
                    value.player2_ready = false;
                }
            }
        }
        client->setRoomList(room_list);
        room update_room = client->findRoomByRoomName(room_name);

        if(client->getUser().username == update_room.playerX.username || client->getUser().username == update_room.playerO.username){
            client->setRoomIn4(update_room);
        }
    }else if(responseJson["type"] == static_cast<int>(ResponseType::START)){
        QString room_name = responseJson["room name"].toString();
        std::vector<room> room_list = client->getRoomList();

        noti->setVisible(false);
        for(room &value: room_list){
            if(value.roomName == room_name){
                value.gameStart = true;
                value.nextBoard = -2;
                value.turn = 0;
                if(client->getUser().username == value.playerX.username || client->getUser().username == value.playerO.username){
                    client->setRoomIn4(value);
                    // qDebug() << "updated value: " << client->getRoomIn4().gameStart << "\n";
                }
            }
        }
        client->setRoomList(room_list);
    }else if(responseJson["type"] == static_cast<int>(ResponseType::NEXTTURN)){
        qDebug() << "next turn: " << responseJson;
        QString room_name = responseJson["room name"].toString();
        QString message = responseJson["message"].toString();
        int next_board = responseJson["next board"].toInt();
        int last_board = responseJson["current board"].toInt();
        int last_cell = responseJson["current cell"].toInt();

        std::vector<room> room_list = client->getRoomList();
        for(room &value: room_list){
            if(value.roomName == room_name){
                value.nextBoard = next_board;
                value.last_board = last_board;
                value.last_cell = last_cell;
                if(value.isPlayerXTurn){
                    value.isPlayerXTurn = false;
                }else{
                    value.isPlayerXTurn = true;
                }

                if(value.nextBoard == -1){
                    if(message == "board win"){
                        value.winning_board.push_back(last_board);
                    }else if(message == "board full"){
                        value.winning_board.push_back(last_cell);
                    }
                }
            }
        }

        client->setRoomList(room_list);
        if(client->getRoomIn4().roomName == room_name){
            room found_room = client->findRoomByRoomName(room_name);
            client->setRoomIn4(found_room);
        }

    }else if(responseJson["type"] == static_cast<int>(ResponseType::WINGAME) || responseJson["type"] == static_cast<int>(ResponseType::SURRENDER)){
        QString room_name = responseJson["room name"].toString();
        QString winner = responseJson["winner"].toString();
        QString loser = responseJson["loser"].toString();

        std::vector<user> online_user_list = client->getOnlineUser();
        for(user &value: online_user_list){
            if(value.username == winner){
                value.elo += 10;
                value.wins += 1;
                value.isFree = true;
                value.winRate = client->calculateWinrate(value.wins, value.loses);
            }else if(value.username == loser){
                value.elo -= 5;
                value.loses += 1;
                value.isFree = true;
                value.winRate = client->calculateWinrate(value.wins, value.loses);
            }
        }
        client->setOnlineUser(online_user_list);

        // update client user infor of winner and loser
        user winner_player = client->findUserByUsername(winner);
        user loser_player = client->findUserByUsername(loser);
        if(client->getUser().username == winner){
            client->setUserfromUser(winner_player);
        }else if(client->getUser().username == loser){
            client->setUserfromUser(loser_player);
        }

        // update room list of all clients
        std::vector<room> room_list = client->getRoomList();
        for(room &value: room_list){
            if(value.roomName == room_name){
                if(value.playerX.username == winner){
                    value.playerX = winner_player;
                    value.playerO = loser_player;
                    noti->setText("X win");
                    noti->setVisible(true);
                }else if(value.playerO.username == winner){
                    value.playerO = winner_player;
                    value.playerX = loser_player;
                    noti->setText("O win");
                    noti->setVisible(true);
                }
                value.nextBoard = -2;
                value.gameStart = false;
                value.winning_board.clear();
                value.player1_ready = false;
                value.player2_ready = false;

            }
        }
        client->setRoomList(room_list);

        // update room in4 of winner and loser
        if(client->getUser().username == winner || client->getUser().username == loser){
            room new_room_in4 = client->findRoomByRoomName(room_name);
            client->setRoomIn4(new_room_in4);
            readyButton->setText("READY");
        }
    }
}

void Game_Screen::setUpGrid(){
    frames[0] = QMainWindow::findChild<QFrame*>("gridFrame");
    frames[1] = QMainWindow::findChild<QFrame*>("gridFrame_2");
    frames[2] = QMainWindow::findChild<QFrame*>("gridFrame_3");
    frames[3] = QMainWindow::findChild<QFrame*>("gridFrame_4");
    frames[4] = QMainWindow::findChild<QFrame*>("gridFrame_5");
    frames[5] = QMainWindow::findChild<QFrame*>("gridFrame_6");
    frames[6] = QMainWindow::findChild<QFrame*>("gridFrame_7");
    frames[7] = QMainWindow::findChild<QFrame*>("gridFrame_8");
    frames[8] = QMainWindow::findChild<QFrame*>("gridFrame_9");

    layouts[0] = QMainWindow::findChild<QGridLayout*>("gridLayout");
    layouts[0]->parent()->setObjectName(QString::number(0));
    layouts[1] = QMainWindow::findChild<QGridLayout*>("gridLayout_2");
    layouts[1]->parent()->setObjectName(QString::number(1));
    layouts[2] = QMainWindow::findChild<QGridLayout*>("gridLayout_3");
    layouts[2]->parent()->setObjectName(QString::number(2));
    layouts[3] = QMainWindow::findChild<QGridLayout*>("gridLayout_4");
    layouts[3]->parent()->setObjectName(QString::number(3));
    layouts[4] = QMainWindow::findChild<QGridLayout*>("gridLayout_5");
    layouts[4]->parent()->setObjectName(QString::number(4));
    layouts[5] = QMainWindow::findChild<QGridLayout*>("gridLayout_6");
    layouts[5]->parent()->setObjectName(QString::number(5));
    layouts[6] = QMainWindow::findChild<QGridLayout*>("gridLayout_7");
    layouts[6]->parent()->setObjectName(QString::number(6));
    layouts[7] = QMainWindow::findChild<QGridLayout*>("gridLayout_8");
    layouts[7]->parent()->setObjectName(QString::number(7));
    layouts[8] = QMainWindow::findChild<QGridLayout*>("gridLayout_9");
    layouts[8]->parent()->setObjectName(QString::number(8));

    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            QString text = "";
            //        itemButtons[i] = new QPushButton();
            //        connect(itemButtons[i],SIGNAL(clicked()),this,SLOT(itemClicked()));
            itemButtons[i][j] = createButton(text,SLOT(itemClicked()));
            itemButtons[i][j]->setObjectName(QString::number(j));
            // Move Move(i / 3, i % 3, j / 3, j % 3);
            // std::cout << itemButtons[i][j]->objectName().toStdString() << std::endl;
        }
    }

    for(int j=0; j<9; j++){
        for(int i=0; i<9; i++){
            int row = i/3;
            int column = i%3;
            if(i / 9 == 0)
                layouts[0]->addWidget(itemButtons[0][i],row,column);
            if(i / (9*2) == 0)
                layouts[1]->addWidget(itemButtons[1][i],row,column);
            if(i / (9*3) == 0)
                layouts[2]->addWidget(itemButtons[2][i],row,column);
            if(i / (9*4) == 0)
                layouts[3]->addWidget(itemButtons[3][i],row,column);
            if(i / (9*5) == 0)
                layouts[4]->addWidget(itemButtons[4][i],row,column);
            if(i / (9*6) == 0)
                layouts[5]->addWidget(itemButtons[5][i],row,column);
            if(i / (9*7) == 0)
                layouts[6]->addWidget(itemButtons[6][i],row,column);
            if(i / (9*8) == 0)
                layouts[7]->addWidget(itemButtons[7][i],row,column);
            if(i / (9*9) == 0)
                layouts[8]->addWidget(itemButtons[8][i],row,column);
        }
    }
}

QPushButton *Game_Screen::createButton(QString &text, const QString member/*const char *member*/){
    //Creates button and sets size policy. Connects button's signal to slot

    QPushButton *button = new QPushButton;
    button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    button->setText(text);
    button->setCheckable(true);
    button->setAutoExclusive(true);

    connect(button, SIGNAL(clicked()),this,member.toStdString().c_str()); //connect button signal to SLOT(itemClicked)
    return button;
}

void Game_Screen::itemClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    clickedButton->setAutoExclusive(false);
    clickedButton->setChecked(false);
    clickedButton->setAutoExclusive(true);

    //set text
    if(!client->getRoomIn4().gameStart){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                itemButtons[i][j]->setDisabled(true);
            }
        }
    }

    //send move
    int current_cell = clickedButton->objectName().toInt();
    int current_board = clickedButton->parent()->objectName().toInt();
    int next_board = current_cell;
    QString room_name = client->getRoomIn4().roomName;
    QJsonObject data;
    data["room name"] = room_name;
    data["current_cell"] = current_cell;
    data["current_board"] = current_board;
    data["player username"] = client->getUser().username;
    client->sendRequestToServer(RequestType::MOVE, data);
}

void Game_Screen::on_readyButton_clicked()
{
    QJsonObject data;
    data["room_name"] = client->getRoomIn4().roomName;
    data["player_username"] = client->getUser().username;

    if(readyButton->text() == "READY"){
        readyButton->setText("UNREADY");
        client->sendRequestToServer(RequestType::READY, data);
    }else if(readyButton->text() == "UNREADY"){
        readyButton->setText("READY");
        client->sendRequestToServer(RequestType::UNREADY, data);
    }else if(readyButton->text() == "START"){
        client->sendRequestToServer(RequestType::START, data);
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                itemButtons[i][j]->setText("");
            }
        }
        noti->setVisible(false);
    }else if(readyButton->text() == "SURRENDER"){
        data["action"] = "surrender";
        if(client->getUser().username == client->getRoomIn4().playerX.username){
            data["other player"] = client->getRoomIn4().playerO.username;
        }else if(client->getUser().username == client->getRoomIn4().playerO.username){
            data["other player"] = client->getRoomIn4().playerX.username;
        }
        client->sendRequestToServer(RequestType::SURRENDER, data);
    }
}


void Game_Screen::on_exit_clicked()
{
    if(client->getRoomIn4().gameStart){
        surfr->setVisible(true);
    }else{
        mainmenulogin *new_screen = new mainmenulogin();
        new_screen->show();
        new_screen->setClient(client);
        this->hide();
    }

}


void Game_Screen::on_yes_clicked()
{
    QJsonObject data;
    data["room name"] = client->getRoomIn4().roomName;
    data["action"] = "exit";
    data["username"] = client->getUser().username;
    mainmenulogin *new_screen = new mainmenulogin();
    new_screen->show();
    new_screen->setClient(client);
    this->hide();
    client->sendRequestToServer(RequestType::SURRENDER, data);
}


void Game_Screen::on_no_clicked()
{
    surfr->setVisible(false);
}

