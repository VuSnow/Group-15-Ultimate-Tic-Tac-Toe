#include "TcpClient.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QtNetwork/QNetworkInterface>

TcpClient::TcpClient() {
    connect(&_socket, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(&_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(&_socket, &QTcpSocket::errorOccurred, this, &TcpClient::errorOccurred);
    connect(&_socket, &QTcpSocket::stateChanged, this, &TcpClient::stateChanged);
    connect(&_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

void TcpClient::connectToDevice(QString ip_address, int port){
    _ip = ip_address;
    _port = port;
    _socket.connectToHost(_ip, _port);
    std::cout << "connected to device" << std::endl;
}

void TcpClient::disconnectFromDevice(){
    _socket.disconnectFromHost();
}


QTcpSocket& TcpClient::getSocket(){
    return _socket;
}

void TcpClient::onReadyRead(){
    auto data = _socket.readAll();
    emit dataReady(data);
}

void TcpClient::onDisconnected(){
    QJsonObject logoutMessage;
    // logoutMessage["type"] = static_cast<int>(RequestType::LOGOUT);
    logoutMessage["username"] = clientUser.username;

    QJsonDocument doc(logoutMessage);
    QByteArray jsonData = doc.toJson();

    _socket.write(jsonData);
}

QByteArray TcpClient::getServerResponse(){
    if (_socket.bytesAvailable() > 0) {
        // qDebug() << _socket.readAll() << "\n";
        return _socket.readAll();
    }
    return QByteArray();
}

void TcpClient::sendRequestToServer(RequestType type, QJsonObject data){
    QJsonObject request;
    request["type"] = static_cast<int>(type);
    request["data"] = data;

    QJsonDocument doc(request);
    QByteArray jsonData = doc.toJson();

    _socket.write(jsonData);
}

user TcpClient::getUser(){
    return clientUser;
}

void TcpClient::setUser(QString username, QString status, int wins, int loses, bool isFree, double winRate, int elo, QString ingame){
    this->clientUser.username = username;
    this->clientUser.status = status;
    this->clientUser.wins = wins;
    this->clientUser.loses = loses;
    this->clientUser.isFree = isFree;
    this->clientUser.winRate = winRate;
    this->clientUser.elo = elo;
    this->clientUser.ingame = ingame;
    // qDebug() << this->clientUser.username << "\n";
}

std::vector<user> TcpClient::getOnlineUser(){
    return onlineUser;
}

void TcpClient::setOnlineUser(std::vector<user> &onlineUser){
    this->onlineUser = onlineUser;
    emit onlineUserListChanged(onlineUser);
}

std::vector<room> TcpClient::getRoomList(){
    return roomList;
}

void TcpClient::setRoomList(std::vector<room> &roomList){
    this->roomList = roomList;
}

user TcpClient::findUserByUsername(const QString& username){
    for(const user &user : onlineUser){
        if(user.username == username){
            return user;
        }
    }

    return user();
}

void TcpClient::setUserfromUser(user &newUpdate){
    this->clientUser = newUpdate;
}

QString TcpClient::getRoomName(){
    return this->room_name;
}

void TcpClient::setRoomName(QString room_name){
    this->room_name = room_name;
}

room TcpClient::getRoomIn4(){
    return this->roomIn4;
}

void TcpClient::setRoomIn4(room &roomIn4){
    this->roomIn4 = roomIn4;
    emit roomIn4Changed(roomIn4);
}

room TcpClient::findRoomByRoomName(const QString &room_name){
    for(const room &value : roomList){
        if(value.roomName == room_name){
            return value;
        }
    }
    return room();
}

double TcpClient::calculateWinrate(int wins, int loses){
    double win_rate;
    if((wins + loses) == 0){
        win_rate = 0;
    }else{
        win_rate = (double)wins / (wins + loses);
    }
    return win_rate;
}

void TcpClient::changeRoomIn4(room &new_room){

}

void TcpClient::changeTurn(QString room_name){
    std::vector<room> room_list = TcpClient::getRoomList();
    for(room &value: room_list){
        if(value.roomName == room_name){
            if(value.isPlayerXTurn){
                value.isPlayerXTurn = false;
            }else{
                value.isPlayerXTurn = true;
            }
        }
    }

    TcpClient::setRoomList(room_list);

    room new_room = TcpClient::findRoomByRoomName(room_name);
    TcpClient::setRoomIn4(new_room);
}
