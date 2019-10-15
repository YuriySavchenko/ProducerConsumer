#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <mutex>
#include <memory>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <functional>

#include "producer.h"
#include "consumer.h"

template <typename T>
class DataManager { 
private:
    std::mutex mutexAcceptConnect;
    std::mutex mutexReadWrite;

private:
    std::queue<T> buffer;
    unsigned int countConsumers;
    std::atomic<unsigned int> countSleepConsumers;

private:
    std::unique_ptr<Producer<T>> producer;
    std::vector<std::unique_ptr<Consumer<T>>> consumers;

public:
    DataManager() = default;
    ~DataManager() = default;
    DataManager(const unsigned int &);

public:
    void execute();
};

template <typename T>
void DataManager<T>::execute() {
    // lambda expression for each consumer
    auto consumerFunc = [&](
        std::unique_ptr<Consumer<T>> & Consumer,
        std::unique_ptr<Producer<T>> & Producer,
        std::mutex & mutexOutput,
        std::mutex & mutexConnect,
        std::mutex & mutexDisconnect)
    {
        mutexAcceptConnect.lock();

        std::lock_guard<std::mutex> guardConnect(mutexConnect);
        Consumer->connect(Producer->getIDs());

        if (Producer->accept()) {
            mutexReadWrite.unlock();
        }

        else {
            mutexAcceptConnect.unlock();
        }

        clock_t end = clock() + (Consumer->getID()*100)*CLOCKS_PER_SEC/1000;

        for (;clock() < end;) {
            std::lock_guard<std::mutex> guardDelete(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(50));
            std::lock_guard<std::mutex> guardOutput(mutexOutput);
            Consumer->consume(buffer);
        }

        std::lock_guard<std::mutex> guardDisconnect(mutexDisconnect);
        Consumer->disconnect(producer->getIDs());
        countSleepConsumers--;

        if (countSleepConsumers == 0) {
            std::cout << "[!] Consumers have finished the execution!" << std::endl;
        }

        else {
            mutexAcceptConnect.unlock();
            mutexReadWrite.lock();
        }
    };

    // mutex for pretty output
    std::mutex mutexOutput;
    std::mutex mutexConnect;
    std::mutex mutexDisconnect;

    // vector for the saving consumers threads
    std::vector<std::thread> vectorThreadsConsumers;

    for (int i=0; i < countConsumers; i++)
        vectorThreadsConsumers.push_back(
            std::thread(consumerFunc,
                std::ref(consumers[i]),
                std::ref(producer),
                std::ref(mutexOutput),
                std::ref(mutexConnect),
                std::ref(mutexDisconnect)));

    // lambda expression which allows to producer start writing into buffer
    std::function<void()> funcThreadProducerW = [&]() {
        clock_t end = clock() + 500*CLOCKS_PER_SEC/1000;

        for (;clock() < end;) {
            std::lock_guard<std::mutex> guardWrite(mutexReadWrite);
            std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(50));

            std::lock_guard<std::mutex> guardOutput(mutexOutput);
            producer->produce(rand() % 1000, buffer);
        }

        std::cout << "[!] Producer has finished the execution!" << std::endl;
    };

    // running of the producer thread for writing into buffer
    std::thread threadProducerW(funcThreadProducerW);

    for (int i=0; i < countConsumers; i++)
        vectorThreadsConsumers[i].join();

    //threadProducerAC.join();
    threadProducerW.join();
}

template <typename T>
DataManager<T>::DataManager(const unsigned int & count) {
    this->countConsumers = count;
    this->countSleepConsumers = count;
    this->producer = std::make_unique<Producer<T>>();

    for (int i=0; i < countConsumers; i++)
        consumers.push_back(std::unique_ptr<Consumer<T>> (new Consumer<T>(i+1)));

    mutexReadWrite.lock();
}

#endif //DATAMANAGER_H