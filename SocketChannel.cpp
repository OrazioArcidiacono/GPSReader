#include "SocketChannel.h"
#include <QDebug>

SocketChannel::SocketChannel(const QString &host, quint16 port)
    : m_host(host), m_port(port)
{}

SocketChannel::~SocketChannel() {}

bool SocketChannel::connectChannel() {
    m_socket.connectToHost(m_host, m_port);
    if (!m_socket.waitForConnected(3000)) {
        qWarning() << Q_FUNC_INFO << "Errore di connessione:" << m_socket.errorString();
        return false;
    }
    return true;
}

bool SocketChannel::sendMessage(const QString &msg) {
    if (m_socket.state() == QTcpSocket::ConnectedState) {
        QByteArray data = msg.toUtf8();
        return m_socket.write(data) == data.size();
    }
    return false;
}

QString SocketChannel::receiveMessage() {
    if (m_socket.waitForReadyRead(3000)) {
        return QString::fromUtf8(m_socket.readAll());
    }
    return QString();
}
