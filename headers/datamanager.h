#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <mutex>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <functional>

#include "producer.h"
#include "consumer.h"

template <typename T>
class DataManager {
private:
    std::vector<std::mutex> mutexesConsumers {3};
    std::mutex mutexAcceptConnect;
    std::mutex mutexReadWrite;

private:
    std::queue<T> buffer;
    unsigned int countConsumers;

private:
    std::unique_ptr<Producer<T>> producer;
    std::unique_ptr<Consumer<T>> consumer1;
    std::unique_ptr<Consumer<T>> consumer2;
    std::unique_ptr<Consumer<T>> consumer3;

public:
    DataManager() = default;
    ~DataManager() = default;
    explicit DataManager(const unsigned int &);

public:
    void execute();
};

template <typename T>
void DataManager<T>::execute() {
    std::function<void()> funcThreadConsumer1 = [&]() {
        mutexAcceptConnect.lock();
        consumer1->connect(producer->getIDs());
        mutexAcceptConnect.unlock();

        mutexesConsumers[0].lock();

        for (;;) {
            std::lock_guard<std::mutex> guardDelete(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(1));
            consumer1->consume(buffer);
        }

        mutexesConsumers[0].unlock();
    };

    std::function<void()> funcThreadConsumer2 = [&]() {
        mutexAcceptConnect.lock();
        consumer2->connect(producer->getIDs());
        mutexAcceptConnect.unlock();

        mutexesConsumers[1].lock();

        for (;;) {
            std::lock_guard<std::mutex> guardDelete(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(1));
            consumer2->consume(buffer);
        }

        mutexesConsumers[1].unlock();

    };

    std::function<void()> funcThreadConsumer3 = [&]() {
        mutexAcceptConnect.lock();
        consumer3->connect(producer->getIDs());
        mutexAcceptConnect.unlock();

        mutexesConsumers[2].lock();

        for (;;) {
            std::lock_guard<std::mutex> guardDelete(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(1));
            consumer3->consume(buffer);
        }

        mutexesConsumers[2].unlock();
    };

    std::function<void()> funcThreadProducerAC = [&]() {
        for (;;) {
            if (producer->accept()) {
                mutexAcceptConnect.lock();
                mutexesConsumers[producer->getIDs().front()-1].unlock();
                mutexesConsumers[producer->getIDs().back()-1].unlock();
                mutexReadWrite.unlock();
            }
        }
    };

    std::function<void()> funcThreadProducerW = [&]() {
        for (;;) {
            std::lock_guard<std::mutex> guardWrite(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(1));
            producer->produce(rand() % 1000, buffer);
        }
    };

    std::thread thrConsumer1(funcThreadConsumer1);
    std::thread thrConsumer2(funcThreadConsumer2);
    std::thread thrConsumer3(funcThreadConsumer3);
    std::thread threadProducerAC(funcThreadProducerAC);
    std::thread threadProducerW(funcThreadProducerW);

    thrConsumer1.join();
    thrConsumer2.join();
    thrConsumer3.join();
    threadProducerAC.join();
    threadProducerW.join();
}

template <typename T>
DataManager<T>::DataManager(const unsigned int &count) {
    this->countConsumers = count;
    this->producer = std::make_unique<Producer<T>>();
    this->consumer1 = std::make_unique<Consumer<T>>(1);
    this->consumer2 = std::make_unique<Consumer<T>>(2);
    this->consumer3 = std::make_unique<Consumer<T>>(3);

    mutexesConsumers[0].lock();
    mutexesConsumers[1].lock();
    mutexesConsumers[2].lock();
    mutexReadWrite.lock();
}

#endif //DATAMANAGER_H