#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

int main() {
    std::cout << "Iniciando el codigo dummy..." << std::endl;

    while (true) {
        // Ocupa CPU con cálculos innecesarios
        volatile double x = 0;
        for (int i = 0; i < 1e8; ++i) {
            x += std::sin(i) * std::cos(i);
            std::cout << "Valor  x = " << x << std::endl;

        }

        // Delay
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
