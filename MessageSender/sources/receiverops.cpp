#include "../headers/receiverops.h"

std::string ReceiverOps::serializer(std::map<char, int>&& data){ // Data serializer to send through socket

    std::ostringstream oss;
    for (const auto& pair : data) {
        oss << pair.first << ":" << pair.second << ";";
    }
    return oss.str();

}


bool ReceiverOps::bufferPredicate(){ // Cv predicate

    return !(this->buffPointer->empty());

}

void ReceiverOps::start(){ // ops starter

    buffer_ops();

}

ReceiverOps::ReceiverOps(std::shared_ptr<std::unique_lock<std::mutex>>& ptrLock, std::shared_ptr<std::condition_variable>& myPtr, std::shared_ptr<std::queue<std::map<char, int>>>&& buffPtr, std::string socketPath)  {

    lockPtr = ptrLock;
    this->buffPointer = buffPtr;
    cvPointer = myPtr;
    socket_path = socketPath.c_str();

    needToSend = std::make_unique<std::queue<std::map<char, int>>>(std::queue<std::map<char, int>>{});

}


ReceiverOps::~ReceiverOps(){

    //std::cout << "Receiver stopped" << std::endl;

}


void ReceiverOps::buffer_ops(){ // Function to get data from buffer

    while(true){

        cvPointer->wait(*lockPtr, [this]() { return bufferPredicate(); });
        
        while(!(buffPointer->empty())){
           
            auto str_data = buffPointer->front();
            for(auto pair : str_data)
                std::cout << pair.first << ":" << pair.second << std::endl;
            if(!send_data(str_data))
                needToSend->push(std::move(str_data));
            buffPointer->pop();


        }

    }


}


bool ReceiverOps::send_data(std::map<char, int>& data){ // Function to send data

    const char* path = "/tmp/infotecs_socket";
    int data_socket = socket(AF_UNIX, SOCK_STREAM, 0); // Defining socket on client
    sockaddr_un serv_addr;
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, path);
    if(connect(data_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        return false;
    while(!needToSend->empty()){

        auto previous_serialized = serializer(std::move(needToSend->front())).c_str();
        needToSend->pop();
        
    }
    auto another = serializer(std::move(data));
    auto current_serialized = another.c_str();
    
    send(data_socket, current_serialized, strlen(current_serialized), 0);
    close(data_socket);
    return true;

}