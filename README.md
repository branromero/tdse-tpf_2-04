# TDSE – TP Final  
## Sistema de Alarma de Incendio (STM32)

### Descripción
Proyecto **Bare Metal** basado en **Sistemas Activados por Eventos (Event-Triggered Systems, ETS)**.  
La aplicación está diseñada para ejecutarse sobre un microcontrolador **STM32**, utilizando un enfoque no bloqueante y tareas periódicas temporizadas.

El sistema modela una **alarma de incendio**, integrando sensores, actuadores, menús de operación y visualización, con salida de depuración redirigida a consola mediante `printf`.

---

## Entorno de desarrollo
- **IDE:** STM32CubeIDE (versión 1.18.0 o superior)  
- **Microcontrolador objetivo:** STM32 NUCLEO-F103RB (o placa STM32 compatible)  
- **Modo de ejecución:** Bare Metal  

---

## Configuración del sistema
- **SystemCoreClock:** 64 MHz  
  - 15,625 ns por ciclo de reloj  
- **SysTick:**  
  - 1000 ticks por segundo  
  - 1 ms por tick  

---

## Arquitectura del software

La aplicación está desarrollada bajo un enfoque **Bare Metal**, utilizando un modelo de ejecución **activado por eventos (ETS)**.  
El sistema se basa en un **bucle principal infinito (superloop)** donde se ejecutan de forma secuencial las distintas tareas del sistema, todas con tiempo de ejecución acotado y sin operaciones bloqueantes.
Cada tarea evalúa su estado y se ejecuta únicamente cuando corresponde, garantizando un comportamiento predecible y determinístico.
Este esquema de trabajo permite facilitar la modularización del software.

---

## Resumen de archivos

### Aplicación principal
- **app.c / app.h**  
  Implementa el bucle principal del sistema y coordina la ejecución de las tareas.

---

### Sistema
- **task_system.c / task_system.h / task_system_attribute.h**  
  Código no bloqueante para el modelado del sistema general.

- **task_system_interface.c / task_system_interface.h**  
  Interfaz no bloqueante del sistema.

---

### Sensores
- **task_sensor.c / task_sensor.h / task_sensor_attribute.h**  
  Modelado de sensores con actualizaciones periódicas no bloqueantes.

- **task_temperature.c / task_temperature.h**  
  Modela tareas relacionadas con el sensado dela temperatura.  
  Realiza actualizaciones periodicas para la lectura de los valores de termperatura, y modela el sitema de mustreo del sensor analogico.

---

### Actuadores
- **task_actuator.c / task_actuator.h / task_actuator_attribute.h**  
  Modelado de actuadores con actualizaciones basadas en tiempo.

- **task_actuator_interface.c / task_actuator_interface.h**  
  Interfaz no bloqueante para el control de actuadores.

---

### Modos de operación y menú
- **task_mod_normal.c / task_mod_normal.h**  
  Lógica del modo normal de operación del sistema.

- **task_mod_menu.c / task_mod_menu.h**  
  Lógica del del funcionamiento del menú.

- **task_menu_interface.c / task_menu_interface.h**  
  Interfaz del sistema de menú.

- **task_menu_attribute.h**  
  Configuraciones del menú.

- **task_normal_interface.c / task_normal_interface.h**  
  Interfaz del modo normal.

---

### Visualización
- **display.c / display.h**  
  Gestión de la salida de pantalla.  
  Permite inicializar y actualizar valores de visualización.  
  Realiza la salida del:
  - Estados del sistema  
  - Valores de sensores  
  - Retroalimentación de actuadores  

---

### Registro y depuración
- **logger.c / logger.h**  
  Utilidades de registro para depuración y monitorización en tiempo real.  
  Reapunta la salida estándar (`printf`) a una consola serial.

- **dwt.h**  
  Proporciona utilidades para medir ciclos de reloj y tiempo de ejecución del código.

---

### EEPROM
- **eeprom.c / eeprom.h**  
  Manejo de memoria EEPROM.

- **eeprom_interface.c / eeprom_interface.h**  
  Interfaz no bloqueante para acceso a EEPROM.

---

### Hardware
- **board.h**  
  Definiciones específicas de la placa y configuración de hardware.

---

## Instrucciones de construcción y ejecución

### Herramientas necesarias
- STM32CubeIDE versión 1.18.0 o superior  
- STM32CubeMX (opcional, recomendado para regenerar código)  
- Placa STM32 compatible con STM32F103RB  
- Depurador ST-Link  

---

### Configuración del proyecto
1. Abrir el proyecto en **STM32CubeIDE**  
2. Verificar que el microcontrolador seleccionado sea **STM32F103RB**  

---

### Compilación
1. Seleccionar **Project > Clean**  
2. Seleccionar **Project > Build**  
3. Verificar que no existan errores en la consola  

---

### Flasheo del microcontrolador
1. Conectar la placa STM32 mediante ST-Link  
2. Seleccionar:  
   **Run > Debug**  
   o  
   **Run > Run As > STM32 Cortex-M C/C++ Application**  
3. Programar el binario en la placa  

---

## Referencias
- Guía de inicio STM32  
- Documentación oficial STM32CubeIDE  

---

## Autores
Trabajo Práctico Final – **TDSE** 
Sistema de Alarma de Incendio
- **Brandon Romero**
- **Matías**
- **Sebastián**
- **Diego** 
