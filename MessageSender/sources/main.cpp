#include "../headers/stringops.h"
#include "../headers/receiverops.h"

int main(){


    std::queue<std::map<char, int>> buffer{}; // Creating buffer
    std::mutex mt;
    std::unique_lock<std::mutex> mtLock{mt}; //Creating lock for mutex and unlocking it
    mtLock.unlock();

    auto buffer_ptr = std::make_shared<decltype(buffer)>(buffer);  // Sharing access to buffer
    size_t initial_size = 64; // Initializing size of input string
    std::shared_ptr<std::condition_variable> cv = std::make_shared<std::condition_variable>();  // sharing conditional variable
    std::shared_ptr<std::unique_lock<std::mutex>> shPtr_Lock = std::make_shared<std::unique_lock<std::mutex>>(std::move(mtLock)); // Sharing access to lock
    
    
    ReceiverOps rops{shPtr_Lock, cv, std::move(buffer_ptr)};
    StringOps sops{shPtr_Lock, cv, std::move(buffer_ptr), initial_size}; // initializing ops_classes
    
    try{
        std::thread sopsThread{[&sops]() { sops.start(); }}; // Initializaing threads with public start methods
        std::thread ropsThread{[&rops]() { rops.start(); }};
        sopsThread.join(); // Joining threads
        ropsThread.join();
    }
    catch(std::exception& e){

        std::cout << "Something`s wrong: " << e.what() << std::endl;

    };

    return 0;

}


