#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "/tmp/temp_monitor.log"
#define UMBRAL 90000 // 45 grados

int leer_temperatura() {
    std::ifstream temp_file(TEMP_PATH);
    int temp = 0;
    if (temp_file.is_open()) {
        temp_file >> temp;
        temp_file.close();
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
    // Clean del log file al inicio
    std::ofstream log(LOG_PATH, std::ios::trunc);
    if (log.is_open()) log.close();

    openlog("temp_monitor", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_NOTICE, "Iniciando temp_monitor");

    // Detectar el usuario actual (usualmente el que ejecuta X o la sesión gráfica)
    uid_t uid = geteuid();
    struct passwd *pwd = getpwuid(uid);
    if (!pwd) {
        syslog(LOG_ERR, "No se pudo obtener info del usuario con UID %d", uid);
        return 1;
    }

    std::string dbus_path = "/run/user/" + std::to_string(uid) + "/bus";

    while (true) {
        int temp = leer_temperatura();

        syslog(LOG_INFO, "Temperatura actual: %.2f °C", temp / 1000.0);
        log_temp(temp);

        if (temp >= UMBRAL) {
            syslog(LOG_WARNING, "⚠️ Temperatura alta: %.2f °C", temp / 1000.0);

            if (system("which notify-send > /dev/null 2>&1") == 0) {
                std::string cmd = "DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=" + dbus_path +
                                  " notify-send '⚠️ Alerta de Temperatura' 'CPU supera los 45°C'";
                system(cmd.c_str());
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    closelog();
    return 0;
}