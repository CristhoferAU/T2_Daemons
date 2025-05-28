#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <string>

// Función para leer la temperatura del CPU en Linux
double obtenerTemperaturaCPU() {
    std::ifstream archivo("/sys/class/thermal/thermal_zone0/temp");
    double temp = 0.0;
    if (archivo.is_open()) {
        archivo >> temp;
        temp /= 1000.0; // Normalmente viene en miligrados Celsius
    } else {
        std::cerr << "No se pudo leer la temperatura del CPU." << std::endl;
    }
    return temp;
}

int main() {
    std::cout << "Iniciando el codigo dummy con lectura de temperatura..." << std::endl;

    while (true) {
        // Cálculo pesado para ocupar CPU
        volatile double x = 0;
        for (int i = 0; i < 1e8; ++i) {
            x += std::sin(i) * std::cos(i);
        }

        // Leer y mostrar temperatura
        double temp = obtenerTemperaturaCPU();
        std::cout << "Valor x = " << x << " | Temperatura CPU: " << temp << " °C" << std::endl;

    }

    return 0;
}
