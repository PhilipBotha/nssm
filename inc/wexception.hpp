//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_WEXCEPTION_HPP
#define NSSM_WEXCEPTION_HPP
#include <stdexcept>
#include <string>
namespace nssm {
    class wexception : public std::exception {
    public:
        wexception(std::wstring aMsg) : msg(std::move(aMsg)) {
        }

        [[nodiscard]] const char *what() const noexcept override {
            return reinterpret_cast<const char *>(msg.c_str());
        }

        [[nodiscard]] virtual const std::wstring &wwhat() const noexcept {
            return msg;
        }

    private:
        std::wstring msg;
    };
} // namespace nssm
#endif //NSSM_WEXCEPTION_HPP
