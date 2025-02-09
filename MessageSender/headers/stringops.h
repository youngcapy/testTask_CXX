#ifndef STRINGOPS
#define STRINGOPS

#include <iostream>
#include <map>
#include <algorithm>
#include <regex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <shared_mutex>

class StringOps{

    public:
        // Rule of five is not observed for the sake of class simplicity...
        StringOps() = delete;
        StringOps(std::shared_ptr<std::unique_lock<std::mutex>>& ptrLock, std::shared_ptr<std::condition_variable>& myPtr, std::shared_ptr<std::queue<std::map<char, int>>>&& buffPtr, size_t& string_size);
        ~StringOps();
        void start();
    
    private:
        void stringOpsStart();
        bool ifLessThan(const std::string& str, size_t& size) noexcept;
        bool isLetterString(const std::string& str) noexcept;
        std::map<char, int> everyCharQuantity(std::string& str);  

        size_t str_size;
        std::thread stringThread;
        std::shared_ptr<std::unique_lock<std::mutex>> lockPtr;
        std::shared_ptr<std::queue<std::map<char, int>>> buffPointer;
        std::shared_ptr<std::condition_variable> cvPointer;
};

#endif