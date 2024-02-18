//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_IMPORTS_HPP
#define NSSM_IMPORTS_HPP
#include "helper.hpp"
#include "wexception.hpp"
#include <filesystem>
#include <format>
#include <windows.h>
namespace nssm {

    class DllImport {
    public:
        DllImport()                             = delete;
        DllImport(const DllImport &)            = delete;
        DllImport &operator=(const DllImport &) = delete;
        DllImport(DllImport &&other) noexcept : mpDll(other.mpDll) {
            other.mpDll = nullptr;
        }

        DllImport &operator=(DllImport &&other) noexcept {
            if (this != &other) {
                FreeLibrary(mpDll);
                mpDll       = other.mpDll;
                other.mpDll = nullptr;
            }
            return *this;
        }

        DllImport(const std::filesystem::path &dll) {
            mpDll = LoadLibraryW(dll.c_str());
            if (!mpDll) {
                throw wexception{L"Could not load " + dll.wstring() + L": " + getLastErrorMsg()};
            }
        }

        ~DllImport() {
            if (mpDll) {
                FreeLibrary(mpDll);
                mpDll = nullptr;
            }
        }

        HMODULE &dll() {
            return mpDll;
        }

    private:
        HMODULE mpDll{nullptr};
    };

    static FARPROC getProcPtr(DllImport &dll, const std::string &proc_name);

    class DllImports {
    public:
        typedef BOOL(WINAPI *AttachConsole_ptr)(DWORD);
        typedef BOOL(WINAPI *SleepConditionVariableCS_ptr)(PCONDITION_VARIABLE, PCRITICAL_SECTION, DWORD);
        typedef BOOL(WINAPI *QueryFullProcessImageName_ptr)(HANDLE, unsigned long, LPWSTR, unsigned long *);
        typedef void(WINAPI *WakeConditionVariable_ptr)(PCONDITION_VARIABLE);
        typedef BOOL(WINAPI *CreateWellKnownSid_ptr)(WELL_KNOWN_SID_TYPE, SID *, SID *, unsigned long *);
        typedef BOOL(WINAPI *IsWellKnownSid_ptr)(SID *, WELL_KNOWN_SID_TYPE);
        DllImports() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
            mpAttachConsole = reinterpret_cast<AttachConsole_ptr>(getProcPtr(mpKernel, "AttachConsole"));

            mpSleepConditionVariableCS =
                    reinterpret_cast<SleepConditionVariableCS_ptr>(getProcPtr(mpKernel, "SleepConditionVariableCS"));
            mpQueryFullProcessImageName =
                    reinterpret_cast<QueryFullProcessImageName_ptr>(getProcPtr(mpKernel, "QueryFullProcessImageNameW"));
            mpWakeConditionVariable =
                    reinterpret_cast<WakeConditionVariable_ptr>(getProcPtr(mpKernel, "WakeConditionVariable"));
            mpCreateWellKnowsSid = reinterpret_cast<CreateWellKnownSid_ptr>(getProcPtr(mpAdvapi, "CreateWellKnownSid"));
            mpIsWellKnowsSid     = reinterpret_cast<IsWellKnownSid_ptr>(getProcPtr(mpAdvapi, "IsWellKnownSid"));
#pragma GCC diagnostic pop
        }
        DllImports(const DllImports &)            = delete;
        DllImports &operator=(const DllImports &) = delete;

        void AttacheConsole(DWORD process_id) {
            if (0 == mpAttachConsole(process_id)) {
                throw wexception{std::format(L"Could not attached console to process id {}. Error: {}", process_id,
                                             getLastErrorMsg())};
            }
        }

        void SleepConditionVariableCS(PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection,
                                      DWORD Milliseconds) {
            if (0 == mpSleepConditionVariableCS(ConditionVariable, CriticalSection, Milliseconds)) {
                throw wexception{std::format(L"SleepConditionVariableCS failed. Error: {}", getLastErrorMsg())};
            }
        }

        void QueryFullProcessImageNameW(HANDLE hProcess, unsigned long flags, LPWSTR ExeName, unsigned long *size) {
            if (0 == mpQueryFullProcessImageName(hProcess, flags, ExeName, size)) {
                throw wexception{std::format(L"QueryFullProcessImageNameW failed. Error: {}", getLastErrorMsg())};
            }
        }

        void WakeConditionVariable(PCONDITION_VARIABLE ConditionVariable) {
            mpWakeConditionVariable(ConditionVariable);
        }

        void CreateWellKnownSid(WELL_KNOWN_SID_TYPE WellKnownSidType, SID *DomainSid, SID *Sid, unsigned long *cbSid) {
            if (0 == mpCreateWellKnowsSid(WellKnownSidType, DomainSid, Sid, cbSid)) {
                throw wexception{std::format(L"CreateWellKnownSid failed. Error: {}", getLastErrorMsg())};
            }
        }

        [[nodiscard]] bool IsWellKnownSid(SID *Sid, WELL_KNOWN_SID_TYPE WellKnownSidType) {
            return TRUE == mpIsWellKnowsSid(Sid, WellKnownSidType);
        }

    private:
        DllImport mpKernel{"kernel32.dll"};
        DllImport mpAdvapi{"advapi32.dll"};
        AttachConsole_ptr mpAttachConsole{nullptr};
        SleepConditionVariableCS_ptr mpSleepConditionVariableCS{nullptr};
        QueryFullProcessImageName_ptr mpQueryFullProcessImageName{nullptr};
        WakeConditionVariable_ptr mpWakeConditionVariable{nullptr};
        CreateWellKnownSid_ptr mpCreateWellKnowsSid{nullptr};
        IsWellKnownSid_ptr mpIsWellKnowsSid{nullptr};
    };

    static  FARPROC getProcPtr(DllImport &dll, const std::string &proc_name) {
        FARPROC ptr{GetProcAddress(dll.dll(), proc_name.c_str())};
        if (!ptr) {
            throw std::runtime_error{std::format("Could not find {}.", proc_name)};
        }
        return ptr;
    }
} // namespace nssm
#endif //NSSM_IMPORTS_HPP