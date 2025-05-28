cmake_minimum_required(VERSION 3.10)
project(temp_monitor)

set(CMAKE_CXX_STANDARD 11)

add_executable(temp_monitor temp_monitor.cpp)

# MV Bin
install(TARGETS temp_monitor DESTINATION /usr/bin)

# Instala el archivo .service en /etc/systemd/system/
install(FILES ${CMAKE_SOURCE_DIR}/config/temp_monitor.service DESTINATION /etc/systemd/system/)

# Target para iniciar el daemon
add_custom_target(enable_service
    COMMAND sudo systemctl daemon-reload
    COMMAND sudo systemctl enable temp_monitor.service
    COMMAND sudo systemctl start temp_monitor.service
    COMMENT "Habilitando y arrancando el servicio temp_monitor"
    VERBATIM
)

# Target para detener el daemon
add_custom_target(stop_service
    COMMAND sudo systemctl stop temp_monitor.service
    COMMENT "Deteniendo el servicio temp_monitor"
    VERBATIM
)

# Target para reiniciar el daemon
add_custom_target(restart_service
    COMMAND sudo systemctl restart temp_monitor.service
    COMMENT "Reiniciando el servicio temp_monitor"
    VERBATIM
)

# Target para deshabilitar el daemon
add_custom_target(disable_service
    COMMAND sudo systemctl disable temp_monitor.service
    COMMENT "Deshabilitando el servicio temp_monitor"
    VERBATIM
)

# Target para desinstalar el daemon completamente
add_custom_target(uninstall_service
    COMMAND sudo systemctl stop temp_monitor
    COMMAND sudo systemctl disable temp_monitor
    COMMAND sudo rm -f /etc/systemd/system/temp_monitor.service
    COMMAND sudo rm -f /usr/bin/temp_monitor
    COMMAND sudo rm -f /tmp/temp_monitor.log
    COMMAND sudo systemctl daemon-reload
    COMMENT "Deteniendo, deshabilitando y eliminando el servicio temp_monitor, binario y log"
    VERBATIM
)