//
// Created by Philip on 2024/02/18.
//

#include "console.hpp"
#include "helper.hpp"
#include "wexception.hpp"
#include <fcntl.h>
#include <iostream>
#include <windows.h>

namespace nssm {

    [[nodiscard]] bool is_console() {
        bool cons{false};
        //If we're running in a service context there will be no console window.
        const auto console{GetConsoleWindow()};
        if (console) {
            DWORD pid{0};
            if (!GetWindowThreadProcessId(console, &pid)) {
                throw wexception{L"Error getting window thread process ID. Error: " + getLastErrorMsg()};
            }

            if (GetCurrentProcessId() != pid) {
                // If the process and window handle are different, programmed launched from within console.
                cons = true;
            } else {
                // If the process associated with the console window handle is the same as this process, we were not
                // launched from an existing console.  The user probably double-clicked our executable. Close the
                // new console so that messages open in pop-ups.
                FreeConsole();
            }
        }
        return cons;
    }

    Utf8Console::~Utf8Console() {
        if (cp) {
            SetConsoleOutputCP(cp);
            cp = 0U;
        }
    }
    Utf8Console::Utf8Console(Utf8Console &&rhs) noexcept {
        cp     = rhs.cp;
        rhs.cp = 0U;
    }
    Utf8Console &Utf8Console::operator=(Utf8Console &&rhs) noexcept {
        cp     = rhs.cp;
        rhs.cp = 0U;
        return *this;
    }
    Utf8Console::Utf8Console(const bool isConsole) {
        if(isConsole){
            cp = GetConsoleOutputCP();
            SetConsoleOutputCP(CP_UTF8);
            _setmode(_fileno(stdout), _O_U8TEXT);
            _setmode(_fileno(stderr), _O_U8TEXT);
        }
    }
} // namespace nssm