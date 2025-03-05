#ifndef SOCKETCHANNEL_H
#define SOCKETCHANNEL_H

#include "CommunicationChannel.h"
#include <QTcpSocket>

class SocketChannel : public CommunicationChannel {
public:
    SocketChannel(const QString &host, quint16 port);
    ~SocketChannel();

    bool connectChannel() override;
    bool sendMessage(const QString &msg) override;
    QString receiveMessage() override;

private:
    QString m_host;
    quint16 m_port;
    QTcpSocket m_socket;
};

#endif // SOCKETCHANNEL_H
