# 🛡️ Keylogger en Windows con Técnicas de Evasión - Análisis y Explotación

Este repositorio contiene el código fuente de un **keylogger para sistemas Windows**, desarrollado con fines **educativos y de investigación en ciberseguridad**. También se incluye un servidor en Python que permite recibir remotamente la información capturada desde una máquina vulnerada.

---

## ⚠️ Advertencia

Este software se proporciona **exclusivamente con fines académicos**. **No debe utilizarse en sistemas sin autorización explícita**. El uso indebido puede ser ilegal y va en contra de los términos de uso del autor.

---

## 🧠 Descripción técnica

El keylogger implementado cuenta con las siguientes características:

- Registro de todas las pulsaciones del teclado, incluidas teclas especiales.
- Ejecución en **segundo plano**, con nombre de proceso disfrazado (`WinUpdate.exe`).
- Mecanismo de **persistencia** mediante modificación del registro:

- **Evasión de entornos de análisis** (sandbox, máquinas virtuales, debugging).
- Ofuscación de cadenas y aumento de la entropía del binario para reducir detección por antivirus.

---

## 📁 Archivos del repositorio

- `keylogger.c`: Código fuente del keylogger en C. Registra las pulsaciones y las guarda localmente en `log.txt`.
- `servidor.py`: Script en Python que actúa como **servidor receptor**, permitiendo recibir el archivo `log.txt` a través de una conexión por sockets.

---

## 🛠️ Compilación

Compilar el keylogger en entorno Windows:

```bash
gcc -o keylogger keylogger.c
