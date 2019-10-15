#ifndef CONSUMER_H
#define CONSUMER_H

#include <iostream>
#include <queue>
#include <deque>

template<typename T>
class Consumer {
private:
    unsigned int ID;

public:
    void consume(std::queue<T> &);
    void connect(std::deque<unsigned int> &);
    void disconnect(std::deque<unsigned int> &);

public:
    Consumer() = default;
    ~Consumer() = default;
    explicit Consumer(const unsigned int &);

public:
    const unsigned int & getID() const noexcept;
};

template<typename T>
void Consumer<T>::consume(std::queue<T> & buffer) {
    if (!buffer.empty()) {
        std::cout << "[-] { CONSUMER ID=" << ID << " } From queue has deleted value: " << buffer.front() << std::endl;
        buffer.pop();
    }
}

template <typename T>
void Consumer<T>::connect(std::deque<unsigned int> & deque) {
    std::cout << "[!] { CONSUMER ID=" << ID << " } has connected!" << std::endl;
    deque.push_back(ID);
}

template <typename T>
Consumer<T>::Consumer(const unsigned int & id) {
    this->ID = id;
}

template <typename T>
void Consumer<T>::disconnect(std::deque<unsigned int> & deque) {
    std::cout << "[!] { CONSUMER ID=" << ID << " } has disconnected!" << std::endl;
    deque.pop_back();
}

template <typename T>
const unsigned int & Consumer<T>::getID() const noexcept {
    return ID;
}

#endif //CONSUMER_H