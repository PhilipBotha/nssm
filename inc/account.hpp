//
// Created by Philip on 2024/02/23.
//

#ifndef NSSM_ACCOUNT_HPP
#define NSSM_ACCOUNT_HPP
#include "service.hpp"
#include <string>
namespace nssm {
    constexpr const wchar_t *const VIRTUAL_SERVICE_ACCOUNT_DOMAIN{L"NT Service"};

    [[nodiscard]] std::wstring virtual_account(Service_t &service);

    [[nodiscard]] bool is_virtual_account(Service_t &service);

    [[nodiscard]] std::wstring canonicalise_username(const std::wstring & username);
} // namespace nssm
#endif //NSSM_ACCOUNT_HPP
