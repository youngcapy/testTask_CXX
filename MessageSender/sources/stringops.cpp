#include "../headers/stringops.h"

//Big and terrifying ctor 
StringOps::StringOps(std::shared_ptr<std::unique_lock<std::mutex>>& ptrLock, std::shared_ptr<std::condition_variable>& myPtr, std::shared_ptr<std::queue<std::map<char, int>>>&& buffPtr, size_t& string_size) {
    
    lockPtr = ptrLock;
    this->buffPointer = buffPtr;
    str_size = string_size;
    this->cvPointer = myPtr;
    

}

void StringOps::start(){

    this->stringThread = std::thread(&StringOps::stringOpsStart, this);
    stringThread.join();

}


StringOps::~StringOps(){

    std::cout << "Deal`s done" << std::endl;
    if(stringThread.joinable())
        stringThread.join();

}


bool StringOps::ifLessThan(const std::string& str, size_t& size) noexcept{

    return str.length() <= size; // Checks for size

}

bool StringOps::isLetterString(const std::string& str) noexcept{

    
    return std::regex_match(str, std::regex("^[a-zA-Z]+$")); // Checks if string contains letters only

}


std::map<char, int> StringOps::everyCharQuantity(std::string& str){ // Function sorts string due to its uselessness and counts every char quantity


    std::map<char, int> charQuantity{};
    std::sort(str.begin(), str.end());
    size_t i{0};
    while(i < str.length()){

        auto quantity = std::count(str.begin(), str.end(), str[i]);
        charQuantity.insert(std::pair<char, int>(str[i], quantity));
        i += quantity;

    }

    return charQuantity; 

}

void StringOps::stringOpsStart(){

    std::string input{};
    std::cout << "If you want to stop, just press Ctrl-C" << std::endl;

    while(true){
        std::getline(std::cin, input);
        //if(input == "stop")
            //break;
        if(!ifLessThan(input, str_size) || !(isLetterString(input))){

            std::cout << "Incorrect string!" << std::endl;
            continue;

        }

        this->lockPtr->lock();
        buffPointer->push(everyCharQuantity(input));
        this->lockPtr->unlock();
        cvPointer->notify_one();

    }
}

