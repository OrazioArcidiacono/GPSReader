#ifndef COMMUNICATIONCHANNEL_H
#define COMMUNICATIONCHANNEL_H

#include <QString>

class CommunicationChannel {
public:
    virtual ~CommunicationChannel() = default;
    virtual bool connectChannel() = 0;
    virtual bool sendMessage(const QString &msg) = 0;
    virtual QString receiveMessage() = 0;
};

#endif // COMMUNICATIONCHANNEL_H
