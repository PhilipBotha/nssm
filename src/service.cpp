//
// Created by Philip on 2024/02/18.
//
#include "account.hpp"
#include "service.hpp"
#include "nssm.hpp"
#include "wexception.hpp"
#include <cstdint>
#include <format>
#include <iostream>
#include <shlwapi.h>
#include <windows.h>
namespace nssm {


    constexpr std::size_t PATH_LENGTH{32627UL};

    [[nodiscard]] static constexpr DWORD toWindowsVal(const Startup startup_type) {
        switch (startup_type) {
            case Startup::Automatic:
                return SERVICE_AUTO_START;
            case Startup::Manual:
                return SERVICE_DEMAND_START;
            case Startup::Disabled:
                return SERVICE_DISABLED;
            default:
                return 0;
        }
    }


    [[nodiscard]] bool edit_service(Service_t &service, const bool editting) {
        bool ret_flag{true};


        //The only two valid flags for service type are SERVICE_WIN32_OWN_PROCESS and SERVICE_INTERACTIVE_PROCESS.
        service.type &= SERVICE_INTERACTIVE_PROCESS;
        service.type |= SERVICE_WIN32_OWN_PROCESS;
        if (service.displayname.empty()) {
            service.displayname = service.name;
        }
        bool virtual_account{false};
        std::wstring canon{};
        if (!service.username.empty()) {
            virtual_account = is_virtual_account(service);
            canon = service.username;
        }else{
            canon = canonicalise_username(service.username);
        }
#if 0
        if(!virtual_account){
            if(well_known_username(canon)){
                password = L"";
            }else{
                if(grant_logon_as_service(canon)){
                    throw wexception{L"Grant logon as service failed"};
                }
            }
        }
#endif
        if (! ChangeServiceConfigW(service.handle, service.type, toWindowsVal(service.startup), SERVICE_NO_CHANGE, 0, 0, 0, service.dependencies.c_str(), canon.c_str(), service.password.c_str(), service.displayname.c_str())) {
            throw wexception{L"Change service config failed."};
        }

        return ret_flag;
    }

    [[nodiscard]] std::wstring nssm_imagepath() {
        std::array<wchar_t, PATH_LENGTH> temp{};
        GetModuleFileNameW(nullptr, temp.data(), temp.size());
        PathQuoteSpacesW(temp.data());
        return std::wstring{temp.data()};
    }

    void install_service(Service_t &service) {
        SCManager sc_service{SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE};
        service.image  = nssm_imagepath();
        service.handle = CreateServiceW(sc_service.handle(), service.name.c_str(), service.name.c_str(),
                                        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
                                        SERVICE_ERROR_NORMAL, service.image.c_str(), nullptr, nullptr, nullptr, nullptr, nullptr);
        if (edit_service(service, false)) {
            DeleteService(service.handle);
            service.handle = nullptr;
        }
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
