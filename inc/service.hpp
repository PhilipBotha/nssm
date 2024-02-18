//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_SERVICE_HPP
#define NSSM_SERVICE_HPP
#include "helper.hpp"
#include "wexception.hpp"
#include <cstdint>
#include <filesystem>
#include <span>
#include <windows.h>

namespace nssm {

    constexpr std::size_t STDIN_SHARING{FILE_SHARE_WRITE};
    constexpr std::size_t STDIN_DISPOSITION{OPEN_EXISTING};
    constexpr std::size_t STDIN_FLAGS{FILE_ATTRIBUTE_NORMAL};
    constexpr std::size_t STDOUT_SHARING{FILE_SHARE_READ | FILE_SHARE_WRITE};
    constexpr std::size_t STDOUT_DISPOSITION{OPEN_ALWAYS};
    constexpr std::size_t STDOUT_FLAGS{FILE_ATTRIBUTE_NORMAL};
    constexpr std::size_t STDERR_SHARING{FILE_SHARE_READ | FILE_SHARE_WRITE};
    constexpr std::size_t STDERR_DISPOSITION{OPEN_ALWAYS};
    constexpr std::size_t STDERR_FLAGS{FILE_ATTRIBUTE_NORMAL};
    constexpr std::size_t RESET_THROTTLE_RESTART{1500UL};
    constexpr std::size_t KILL_CONSOLE_GRACE_PERIOD{1500UL};
    constexpr std::size_t KILL_WINDOW_GRACE_PERIOD{1500UL};
    constexpr std::size_t KILL_THREADS_GRACE_PERIOD{1500UL};

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

    class Service_t {
    public:
        bool native;
        std::wstring name{};
        std::wstring displayname{};
        std::wstring description{};
        unsigned long startup;
        std::wstring username{};
        std::wstring password;
        unsigned long type{SERVICE_WIN32_OWN_PROCESS};
        std::wstring image{};
        std::wstring exe{};
        std::wstring flags{};
        std::wstring dir{};
        std::wstring env{};
        std::int64_t affinity;
        std::wstring dependencies{};
        std::wstring env_extra{};
        unsigned long priority{NORMAL_PRIORITY_CLASS};
        unsigned long no_console;
        std::filesystem::path stdin_path{};
        unsigned long stdin_sharing{STDIN_SHARING};
        unsigned long stdin_disposition{STDIN_DISPOSITION};
        unsigned long stdin_flags{STDIN_FLAGS};
        std::filesystem::path stdout_path{};
        unsigned long stdout_sharing{STDOUT_SHARING};
        unsigned long stdout_disposition{STDOUT_DISPOSITION};
        unsigned long stdout_flags{STDOUT_FLAGS};
        bool use_stdout_pipe;
        HANDLE stdout_si;
        HANDLE stdout_pipe;
        HANDLE stdout_thread;
        unsigned long stdout_tid;
        std::filesystem::path stderr_path{};
        unsigned long stderr_sharing{STDERR_SHARING};
        unsigned long stderr_disposition{STDERR_DISPOSITION};
        unsigned long stderr_flags{STDERR_FLAGS};
        bool use_stderr_pipe;
        HANDLE stderr_si;
        HANDLE stderr_pipe;
        HANDLE stderr_thread;
        unsigned long stderr_tid;
        bool hook_share_output_handles;
        bool rotate_files;
        bool timestamp_log;
        bool stdout_copy_and_truncate;
        bool stderr_copy_and_truncate;
        unsigned long rotate_stdout_online;
        unsigned long rotate_stderr_online;
        unsigned long rotate_seconds;
        std::uint64_t rotate_bytes;
        unsigned long rotate_delay;
        unsigned long default_exit_action;
        unsigned long restart_delay;
        unsigned long throttle_delay{RESET_THROTTLE_RESTART};
        unsigned long stop_method{~0UL};
        unsigned long kill_console_delay{KILL_CONSOLE_GRACE_PERIOD};
        unsigned long kill_window_delay{KILL_WINDOW_GRACE_PERIOD};
        unsigned long kill_threads_delay{KILL_THREADS_GRACE_PERIOD};
        bool kill_process_tree{true};
        SC_HANDLE handle;
        SERVICE_STATUS status;
        SERVICE_STATUS_HANDLE status_handle;
        HANDLE process_handle;
        unsigned long pid;
        HANDLE wait_handle;
        unsigned long exitcode;
        bool stopping;
        bool allow_restart;
        unsigned long throttle;
        CRITICAL_SECTION throttle_section;
        bool throttle_section_initialised;
        CRITICAL_SECTION hook_section;
        bool hook_section_initialised;
        CONDITION_VARIABLE throttle_condition;
        HANDLE throttle_timer;
        LARGE_INTEGER throttle_duetime;
        FILETIME nssm_creation_time;
        FILETIME creation_time;
        FILETIME exit_time;
        std::wstring initial_env{};
        unsigned long last_control;
        unsigned long start_requested_count;
        unsigned long start_count;
        unsigned long exit_count;
    };

    void pre_install_service(const std::wstring &service_name, const std::filesystem::path &service_path,
                             const std::span<const std::wstring> &args);

    void install_service(Service_t &service);
} // namespace nssm

#endif //NSSM_SERVICE_HPP