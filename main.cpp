#include <iostream>
#include <thread>
#include <chrono>
#define _DEBUG
#include "alg.hpp"
#include "sim.hpp"

int main() {

    auto simEnv = sim::Simulator<3>();
    auto padder = alg::Padder<3>(256);

    size_t runCounter = 0u;
    const size_t resetLimit = 10u;

    while (true) {
        auto [ sender, reciever ] = simEnv.run();
        auto [ pad, success ] = padder.alloc(sender);

        if (! success) {
            printf("out of pad space!");
            break;
        }

        printf("sent %c\n", pad ^ 'a');

        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));

        // if (++runCounter % resetLimit == 0) {
        //     printf("%s", simEnv.peek().c_str());
        // }
    }

    return 0;
}