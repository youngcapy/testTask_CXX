#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
//#include 
#include <map>
#include <string>
#include <sstream>

void printer(std::map<char, int>& data){

    std::multimap<int, char> temp{};
    std::cout << "New message letters ascending: ";
    for(auto pair: data)
        temp.insert(std::pair<int, char>{pair.second, pair.first});
    for(auto pair: temp)
        std::cout << pair.second;
    std::cout << std::endl;


}


std::map<char, int> deserialize(const std::string& data) {
    std::map<char, int> myMap;
    std::istringstream iss(data);
    std::string item;

    while (std::getline(iss, item, ';')) {
        size_t pos = item.find(':');
        if (pos != std::string::npos) {
            char key = item.substr(0, pos)[0]; 
            int value = std::stoi(item.substr(pos + 1));  
            myMap[key] = value;  
        }
    }
    return myMap;
}

const char *SOCKET_PATH = "/tmp/infotecs_socket";

int main() {
    int server_fd, client_socket;
    struct sockaddr_un address;
    char buffer[1024] = {0};

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);

    std::cout << "Waiting for messages..." << std::endl;

    try{
        while (true) {
            int client_socket = accept(server_fd, nullptr, nullptr);

            while (true) {
                ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '0'; 
                    auto map = deserialize(buffer);
                    printer(map);
                    memset(buffer, 0, sizeof(buffer)); 
                    break;
                } 
            }

            
            close(client_socket);
        }
    }

    catch(std::exception& someException){

        std::cout << someException.what() << std::endl;

    }
    unlink(SOCKET_PATH);

    return 0;
}