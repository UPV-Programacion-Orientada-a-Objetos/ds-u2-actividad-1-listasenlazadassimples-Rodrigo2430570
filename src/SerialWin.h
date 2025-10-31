#ifndef SERIAL_WIN_H
#define SERIAL_WIN_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstddef>

class SerialWin {
public:
    SerialWin();
    ~SerialWin();

    bool open(const char* portName, unsigned long baud = 9600);
    void close();

    // Lee hasta encontrar '\n' o agotar el buffer. Devuelve true si leyó algo.
    bool readLine(char* out, std::size_t maxLen, unsigned int timeoutMs = 2000);

private:
#ifdef _WIN32
    HANDLE hComm;
#endif
};

#endif // SERIAL_WIN_H
