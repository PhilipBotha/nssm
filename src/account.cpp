//
// Created by Philip on 2024/02/23.
//

#include "account.hpp"
#include <format>
namespace nssm {

    [[nodiscard]] std::wstring virtual_account(Service_t &service) {
        return std::format(L"{}\\{}", L"", VIRTUAL_SERVICE_ACCOUNT_DOMAIN, service.name);
    }

    [[nodiscard]] bool is_virtual_account(Service_t &service) {
        return toLower(service.username) == toLower(virtual_account(service));
    }

    [[nodiscard]] std::wstring canonicalise_username(const std::wstring & username){
        ///\todo

        return username;
    }

} // namespace nssm