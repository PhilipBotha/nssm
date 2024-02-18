#include "nssm.h"


namespace nssm {

    /* See if we were launched from a console window. */
    bool check_console() {
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

    void alloc_console(const nssm_service_t &service) {
        if (service.no_console == 0U) {
            AllocConsole();
        }
    }
} // namespace nssm