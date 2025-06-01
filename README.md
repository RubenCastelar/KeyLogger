# 🛡️ Keylogger en Windows con Técnicas de Evasión - Análisis y Explotación

Este repositorio contiene el código fuente de un **keylogger para sistemas Windows**, desarrollado con fines **educativos y de investigación en ciberseguridad**. También se incluye un servidor en Python que permite recibir remotamente la información capturada desde una máquina vulnerada, o bien enviarla a través de un **webhook de Discord**.

---

## ⚠️ Advertencia

Este software se proporciona **exclusivamente con fines académicos**. **No debe utilizarse en sistemas sin autorización explícita**. El uso indebido puede ser ilegal y va en contra de los términos de uso del autor.

---

## 🎯 Funcionalidades

- Registro de todas las pulsaciones del teclado, incluidas teclas especiales.
- Envío de logs de teclas mediante:
  - Socket a servidor remoto (Python).
  - Webhook de Discord (configurable).
- Persistencia opcional en el sistema.
- Técnicas básicas de evasión antivirus (ofuscación de código y nombre).
- Modo oculto: el proceso no muestra ventana visible para el usuario.

---

## 🧪 Uso

### 1. Compilar el keylogger

Compila el archivo `keylogger.cpp` con un compilador como MinGW:

```bash
x86_64-w64-mingw32-gcc keylogger.c -o keylogger.exe -mwindows -lwininet
