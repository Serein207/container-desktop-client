#ifndef WEB_SOCKIFY_H
#define WEB_SOCKIFY_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include <QWebSocket>

class TcpSockify : public QObject {
    Q_OBJECT

public:
    TcpSockify(QObject* parent = nullptr);
    ~TcpSockify();

    void connectToServer(const QUrl& url);
    void disconnectFromServer();

private slots:
    void onWebSocketConnected();
    void onWebSocketReceivedBinaryData(const QByteArray& data);
    void onWebSocketReceivedTextData(const QString& data);

private:
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;
    QWebSocket* webSocket;
};

#endif // WEB_SOCKIFY_H