#ifndef CONSUMER_H
#define CONSUMER_H

#include <iostream>
#include <queue>

template<typename T>
class Consumer {
private:
    unsigned int ID;

public:
    void consume(std::queue<T> &);
    void connect(std::queue<unsigned int> &);

public:
    Consumer() = default;
    ~Consumer() = default;
    explicit Consumer(const unsigned int &);
};

template<typename T>
void Consumer<T>::consume(std::queue<T> & buffer) {
    if (!buffer.empty()) {
        std::cout << "[-] { CONSUMER ID=" << ID << " } From queue has deleted value: " << buffer.front() << std::endl;
        buffer.pop();
    }
}

template <typename T>
void Consumer<T>::connect(std::queue<unsigned int> & queue) {
    std::cout << "[!] Connection of Consumer with ID = " << ID << std::endl;
    queue.push(ID);
}

template <typename T>
Consumer<T>::Consumer(const unsigned int & id) {
    this->ID = id;
}

#endif //CONSUMER_H