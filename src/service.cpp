//
// Created by Philip on 2024/02/18.
//

#include "service.hpp"
#include "wexception.hpp"
#include <cstdint>
#include <iostream>
#include <windows.h>
namespace nssm {

    [[nodiscard]] std::vector<ENUM_SERVICE_STATUS_PROCESSW> get_services() {
        DWORD required{0U};
        DWORD count{0U};
        DWORD resume{0U};
        SCManager scManager{SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE};
        if (0 == EnumServicesStatusExW(scManager.handle(), SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
                                       nullptr, 0, &required, &count, &resume, nullptr)) {
            if (GetLastError() != ERROR_MORE_DATA) {
                throw wexception{L"Error getting service status. Error: " + getLastErrorMsg()};
            }
        }

        std::vector<ENUM_SERVICE_STATUS_PROCESSW> services{};
        services.resize(required / sizeof(ENUM_SERVICE_STATUS_PROCESSW));
        if (0 == EnumServicesStatusExW(scManager.handle(), SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
                                       reinterpret_cast<LPBYTE>(services.data()),
                                       static_cast<DWORD>(services.size() * sizeof(ENUM_SERVICE_STATUS_PROCESSW)),
                                       &required, &count, &resume, nullptr)) {
            if (GetLastError() != ERROR_MORE_DATA) {
                throw wexception{L"Error getting service status. Error: " + getLastErrorMsg()};
            }
        }

        services.resize(count);
        return services;
    }
} // namespace nssm
