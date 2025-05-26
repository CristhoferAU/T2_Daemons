#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cstring>
#include <chrono>
#include <cstdlib>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "temp_log.txt"
//#define LOG_PATH "/var/log/tempdaemon.log"
#define UMBRAL 65000 // 65 grados

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

int main(){
    pid_t pid, sid;
    pid = fork();
    if(pid > 0) exit(EXIT_SUCCESS);
    else if(pid < 0) exit(EXIT_FAILURE);

    umask(0);
    openlog("tempdaemon", LOG_NOWAIT | LOG_PID, LOG_USER);
    syslog(LOG_NOTICE, "Successfully started tempdaemon");

    sid = setsid();
    if(sid < 0) {
        syslog(LOG_ERR, "Could not generate session ID");
        exit(EXIT_FAILURE);
    }

    if((chdir("/")) < 0) {
        syslog(LOG_ERR, "Could not change directory to /");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(true) {
        int temp = leer_temperatura();
        std::cout << "Temperatura: " << (temp / 1000.0) << " °C\n";
        log_temp(temp);

        if (temp >= UMBRAL) {
            system("notify-send '⚠️ Alerta de Temperatura' 'CPU supera los 65°C'");
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    syslog(LOG_NOTICE, "Stopping tempdaemon");
    closelog();
    exit(EXIT_SUCCESS);
}
