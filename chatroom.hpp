#include "message.hpp"
#include <iostream>
#include <set>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <boost/asio.hpp>
#include <deque>

#ifndef CHATROOM_HPP
#define CHATROOM_HPP

using boost::asio::ip::tcp;

typedef std::shared_ptr<Participant> ParticipantPointer;

class Participant {
    public:
        virtual void deliver(Message &message) = 0;
        virtual void write(Message &message) = 0;
        virtual ~Participant() = default;
};

class Session: public Participant, public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket s, Room &room);
        void start();
        void deliver(Message &message);
        void write(Message &message);
        void async_read();
        void async_write(std::string messageBody, size_t messageLength);
    
    private:
        tcp::socket clientSocket;
        boost::asio::streambuf buffer;
        Room &room;
        std::deque<Message> messageQueue;
};

class Room {
    public:
        void join(ParticipantPointer participant);
        void leave(ParticipantPointer participant);
        void deliver(ParticipantPointer participant, Message &Message);
    private:
        enum {
            MaxParticipants = 100
        };
        std::deque<Message> messageQueue;
        std::set<ParticipantPointer> participants;
};

#endif CHATROOM_HPP