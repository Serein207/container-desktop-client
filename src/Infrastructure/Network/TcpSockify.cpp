#include "TcpSockify.h"
#include <QHostAddress>

TcpSockify::TcpSockify(QObject* parent) : QObject(parent) {
    tcpServer = new QTcpServer(this);
    webSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    connect(webSocket, &QWebSocket::connected, this, &TcpSockify::onWebSocketConnected);
    connect(webSocket, &QWebSocket::binaryMessageReceived, this,
            &TcpSockify::onWebSocketReceivedBinaryData);
    connect(webSocket, &QWebSocket::textMessageReceived, this,
            &TcpSockify::onWebSocketReceivedTextData);
    connect(webSocket, &QWebSocket::errorOccurred, this,
            [](QAbstractSocket::SocketError error) { qDebug() << "web socket error: " << error; });

    tcpServer->listen(QHostAddress::LocalHost, 5900);
}

TcpSockify::~TcpSockify() {
    tcpServer->close();
    webSocket->close();
}

void TcpSockify::connectToServer(const QUrl& url) {
    qDebug() << "try to connect" << url;
    connect(tcpServer, &QTcpServer::newConnection, this, [this, url] {
        tcpSocket = tcpServer->nextPendingConnection();
        qDebug() << "tcp server connected!";
        connect(tcpSocket, &QTcpSocket::readyRead, this,
                [this] { webSocket->sendBinaryMessage(tcpSocket->readAll()); });
        webSocket->open(url);
    });
}

void TcpSockify::onWebSocketReceivedBinaryData(const QByteArray& data) {
    tcpSocket->write(data);
}

void TcpSockify::onWebSocketReceivedTextData(const QString& data) {
    onWebSocketReceivedBinaryData(data.toUtf8());
}

void TcpSockify::disconnectFromServer() {
    webSocket->close();
    tcpSocket->close();
}
