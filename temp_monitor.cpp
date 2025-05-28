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

// Path para leer la temperatura actual del CPU
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
// Path para loguear las temperaturas leídas
#define LOG_PATH "/tmp/temp_monitor.log"
#define UMBRAL 65000 // 65 grados

// Función para leer la temperatura del CPU
int leer_temperatura() {
    std::ifstream temp_file(TEMP_PATH);
    int temp = 0;
    if (temp_file.is_open()) {
        temp_file >> temp;
        temp_file.close();
    }
    return temp;
}

// Función para escribir la temperatura del CPU en el log
void log_temp(int temp) {
    std::ofstream log(LOG_PATH, std::ios::app);
    if (log.is_open()) {
        log << "Temperatura actual: " << (temp / 1000.0) << " °C\n";
        log.close();
    }
}

int main() {
    // Clean del log file al inicio de la ejecución
    std::ofstream log(LOG_PATH, std::ios::trunc);
    if (log.is_open()) log.close();

    openlog("temp_monitor", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_NOTICE, "Iniciando temp_monitor");

    // Obtiene la información del usuario para la notificación
    uid_t uid = geteuid();
    struct passwd *pwd = getpwuid(uid);
    if (!pwd) {
        syslog(LOG_ERR, "No se pudo obtener info del usuario con UID %d", uid);
        return 1;
    }

    std::string dbus_path = "/run/user/" + std::to_string(uid) + "/bus";

    while (true) {
        int temp = leer_temperatura();

        // Escribe la temperatura actual en el log
        syslog(LOG_INFO, "Temperatura actual: %.2f °C", temp / 1000.0);
        log_temp(temp);

        // Valida si la temperatura está por encima del Umbral
        if (temp >= UMBRAL) {
            // Lanza log de un warnig si la temp es mal alta que Umbral
            syslog(LOG_WARNING, "⚠️ Temperatura alta: %.2f °C", temp / 1000.0);
            // Verifica si está disponible notify-send para utilizarlo
            if (system("which notify-send > /dev/null 2>&1") == 0) {
                // Lanza la notificación para el usuario
                std::string cmd = "DISPLAY=:0 DBUS_SESSION_BUS_ADDRESS=unix:path=" + dbus_path +
                                  " notify-send '⚠️ Alerta de Temperatura' 'CPU supera los 65°C'";
                system(cmd.c_str());
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    closelog();
    return 0;
}