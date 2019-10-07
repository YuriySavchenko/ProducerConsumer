#include <iostream>
#include "datamanager.h"

int main(int argc, char *argv[]) {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::unique_ptr<DataManager<int>> manager(new DataManager<int>(5));

    manager->execute();

    return 0;
}