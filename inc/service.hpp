//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_SERVICE_HPP
#define NSSM_SERVICE_HPP
#include "helper.hpp"
#include "wexception.hpp"
#include <windows.h>
namespace nssm {
    //enum class Services { NSSM, ALL };
    /// Function to list all the windows services.
    /// \return Vectori containing all of the windows services.
    [[nodiscard]] std::vector<ENUM_SERVICE_STATUS_PROCESSW> get_services();


    class SCManager {
    public:
        SCManager(DWORD access) {
            mpHandle = OpenSCManagerW(nullptr, SERVICES_ACTIVE_DATABASE, access);
            if (!mpHandle) {
                throw wexception{L"Failed to open SCManager. Error: " + getLastErrorMsg()};
            }
        }
        // Disallow copy constructor and assignment operator
        SCManager(const SCManager &other)            = delete;
        SCManager &operator=(const SCManager &other) = delete;
        // Add move constructor and assignment operator
        SCManager(SCManager &&other) noexcept : mpHandle{other.mpHandle} {
            other.mpHandle = nullptr;
        }
        SCManager &operator=(SCManager &&other) noexcept {
            if (this != &other) {
                CloseServiceHandle(mpHandle);
                mpHandle       = other.mpHandle;
                other.mpHandle = nullptr;
            }
            return *this;
        }

        ~SCManager() {
            if (mpHandle) {
                CloseServiceHandle(mpHandle);
                mpHandle = nullptr;
            }
        }
        SC_HANDLE &handle() {
            return mpHandle;
        }

    private:
        SC_HANDLE mpHandle{nullptr};
    };
} // namespace nssm

#endif //NSSM_SERVICE_HPP