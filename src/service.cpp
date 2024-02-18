//
// Created by Philip on 2024/02/18.
//

#include "service.hpp"
#include "wexception.hpp"
#include <cstdint>
#include <format>
#include <iostream>
#include <windows.h>
namespace nssm {

    void install_service(Service_t &service) {
    }
    void pre_install_service(const std::wstring &service_name, const std::filesystem::path &service_path,
                             const std::span<const std::wstring> &args) {
        constexpr std::size_t MAX_CMD_LENGTH{8192UL};
        Service_t service{};
        service.name = service_name;
        if (!args.empty()) {
            for (const auto &arg : args) {
                service.flags += arg + L" ";
            }
            service.flags.pop_back(); // remove last space.
            if (service.flags.size() > MAX_CMD_LENGTH) {
                throw wexception{std::format(L"Too many arguments. Size {} exceed max of {}", service.flags.size(),
                                             MAX_CMD_LENGTH)};
            }
        }
        service.dir = service_path.root_directory();
        install_service(service);
    }

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
