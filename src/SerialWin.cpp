#include "SerialWin.h"
#include <iostream>
#include <cstring>

SerialWin::SerialWin()
{
#ifdef _WIN32
    hComm = INVALID_HANDLE_VALUE;
#endif
}

SerialWin::~SerialWin()
{
    close();
}

bool SerialWin::open(const char* portName, unsigned long baud)
{
#ifdef _WIN32
    if (!portName) return false;
    close();

    char fullName[64];
    // Formato especial para puertos COM>9
    std::snprintf(fullName, sizeof(fullName), "\\\\.\\%s", portName);

    hComm = CreateFileA(fullName,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        std::cerr << "[Serial] No se pudo abrir " << portName << "\n";
        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(hComm, &dcb)) {
        std::cerr << "[Serial] GetCommState fallo\n";
        close();
        return false;
    }
    dcb.BaudRate = baud;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity   = NOPARITY;
    if (!SetCommState(hComm, &dcb)) {
        std::cerr << "[Serial] SetCommState fallo\n";
        close();
        return false;
    }

    COMMTIMEOUTS to = {0};
    to.ReadIntervalTimeout = 50;
    to.ReadTotalTimeoutMultiplier = 10;
    to.ReadTotalTimeoutConstant = 50;
    SetCommTimeouts(hComm, &to);
    return true;
#else
    (void)portName; (void)baud;
    return false;
#endif
}

void SerialWin::close()
{
#ifdef _WIN32
    if (hComm != INVALID_HANDLE_VALUE) {
        CloseHandle(hComm);
        hComm = INVALID_HANDLE_VALUE;
    }
#endif
}

bool SerialWin::readLine(char* out, std::size_t maxLen, unsigned int timeoutMs)
{
#ifdef _WIN32
    if (hComm == INVALID_HANDLE_VALUE || !out || maxLen == 0) return false;
    DWORD bytesRead = 0;
    DWORD total = 0;
    unsigned int waited = 0;

    while (total + 1 < maxLen) {
        char ch;
        if (!ReadFile(hComm, &ch, 1, &bytesRead, NULL)) {
            return total > 0; // error, devolver lo que haya
        }
        if (bytesRead == 0) {
            Sleep(10);
            waited += 10;
            if (waited >= timeoutMs) break;
            continue;
        }
        out[total++] = ch;
        if (ch == '\n') break;
    }
    out[total] = '\0';
    return total > 0;
#else
    (void)out; (void)maxLen; (void)timeoutMs;
    return false;
#endif
}
