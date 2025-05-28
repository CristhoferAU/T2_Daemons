# 🖥️ Temp Monitor Daemon

Este proyecto implementa un daemon en C++ que monitorea la temperatura del CPU usando `/sys/class/thermal/thermal_zone0/temp`. Si se supera un umbral de temperatura (por defecto 45 °C), el sistema registra la alerta y, si es posible, lanza una notificación gráfica mediante `notify-send`.

El proyecto utiliza CMake para la compilación y configuración automática del servicio `systemd`, que se instala en `/usr/bin` y se habilita como daemon del sistema.

## ✅ Requisitos

- CMake >= 3.10
- g++ (compilador C++)
- systemd
- notify-send (opcional, para notificaciones gráficas)
- Permisos de superusuario (sudo)

## ⚙️ Compilación e instalación con CMake

Este proyecto está completamente automatizado con CMake. Basta con crear una carpeta de compilación, generar el proyecto y luego instalarlo. El proceso instala tanto el binario como el archivo `.service` en el sistema y configura automáticamente el servicio para ejecutarse en segundo plano.

```bash
mkdir build
cd build
cmake ..
sudo make install
sudo make enable_service
```
## 📋 Comandos disponibles con CMake

- **sudo make install**  
  Compila y copia el binario del daemon a la ruta `/usr/bin`, además de instalar el archivo `temp_monitor.service` en `/etc/systemd/system/`.

- **sudo make enable_service**  
  Habilita e inicia el servicio para que se ejecute automáticamente al iniciar el sistema.

- **sudo make stop_service**  
  Detiene el servicio actualmente en ejecución.

- **sudo make restart_service**  
  Reinicia el servicio y limpia el archivo de logs.

- **sudo make disable_service**  
  Deshabilita el servicio para que no se inicie automáticamente con el sistema.

- **sudo make uninstall_service**  
  Detiene, deshabilita y elimina completamente el servicio, el binario y el archivo de logs.


## 📋 Comando disponible para observar deamon activado

- **systemctl list-units --type=service**
  Muestra los deamons activados
