#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "temp_log.txt"
#define UMBRAL 65000 // 65 grados

int leer_temperatura() {
    std::ifstream temp_file(TEMP_PATH);
    int temp = 0;
    if (temp_file.is_open()) {
        temp_file >> temp;
        temp_file.close();
    } else {
        std::cerr << "Error leyendo temperatura\n";
    }
    return temp;
}

void log_temp(int temp) {
    std::ofstream log(LOG_PATH, std::ios::app);
    if (log.is_open()) {
        log << "Temperatura actual: " << (temp / 1000.0) << " °C\n";
        log.close();
    }
}

int main() {
    while (true) {
        int temp = leer_temperatura();
        std::cout << "Temperatura: " << (temp / 1000.0) << " °C\n";
        log_temp(temp);

        if (temp >= UMBRAL) {
            system("notify-send '⚠️ Alerta de Temperatura' 'CPU supera los 65°C'");
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
