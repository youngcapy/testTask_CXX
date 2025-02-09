#ifndef RECEIVEROPS
#define RECEIVEROPS

#include <iostream>
#include <map>
#include <thread>
#include <queue>
#include <memory>
#include <condition_variable>
#include <shared_mutex>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class ReceiverOps{

    public:
        // Rule of five is not observed for the sake of class simplicity...
        ReceiverOps() = delete;
        ReceiverOps(std::shared_ptr<std::unique_lock<std::mutex>>& ptrLock, std::shared_ptr<std::condition_variable>& myPtr, std::shared_ptr<std::queue<std::map<char, int>>>&& buffPtr, std::string socketPath = "/tmp/infotecs_socket");
        ~ReceiverOps();
        void start();

    private:
        std::string serializer(std::map<char, int>&& data);
        bool send_data(std::map<char, int>& data);
        bool bufferPredicate();
        void buffer_ops();
        std::shared_ptr<std::unique_lock<std::mutex>> lockPtr;
        std::shared_ptr<std::queue<std::map<char, int>>> buffPointer;
        std::shared_ptr<std::condition_variable> cvPointer;
        std::unique_ptr<std::queue<std::map<char, int>>> needToSend;
        
        const char* socket_path;

};


#endif