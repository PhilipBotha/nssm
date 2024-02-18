//
// Created by Philip Botha on 2023/11/24.
//
#include "helper.hpp"
#include <format>
#include <stdexcept>

namespace nssm {

    [[nodiscard]] std::vector<TCHAR> getVec(const TCHAR *const apNullTermString) {
        std::vector<TCHAR> vec{};
        vec.reserve(std::char_traits<TCHAR>::length(apNullTermString) + 1U);
        vec.assign(apNullTermString, apNullTermString + std::char_traits<TCHAR>::length(apNullTermString) + 1U);
        return vec;
    }

    [[nodiscard]] std::wstring getLastErrorMsg() {
        const DWORD error_code{GetLastError()};
        LPWSTR buffer{nullptr};
        if (0 ==
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           nullptr,
                           error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&buffer), 0, nullptr)) {
            throw std::runtime_error{std::format("Failed to format error message. Error code: {}", GetLastError())};
        }

        std::wstring error_message{buffer};
        LocalFree(buffer);
        return error_message;
    }
} // namespace nssm