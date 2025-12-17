#include "include/oop_lr7.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    oop::Simulator simulator;
    simulator.start();

    const auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < oop::kSimulationTime) {
        simulator.print_map(std::cout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    simulator.stop();
    simulator.print_survivors(std::cout);

    return 0;
}
