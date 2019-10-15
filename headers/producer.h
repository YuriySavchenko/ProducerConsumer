#ifndef PRODUCER_H
#define PRODUCER_H

#include <iostream>
#include <queue>
#include <deque>

template <typename T>
class Producer {
protected:
    std::deque<unsigned int> ID;

public:
    void produce(T, std::queue<T> &);
    bool accept();

public:
    std::deque<unsigned int> & getIDs();
};

template <typename T>
void Producer<T>::produce(T value, std::queue<T> & buffer) {
    std::cout << "[+] { PRODUCER ID=W } To queue has been wrote value: " << value << std::endl;
    buffer.push(value);
}

template <typename T>
bool Producer<T>::accept() {
    return (ID.size() == 1) ? true : false;
}

template <typename T>
std::deque<unsigned int> & Producer<T>::getIDs() {
    return ID;
}

#endif //PRODUCER_H