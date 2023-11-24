//
// Created by Philip Botha on 2023/11/24.
//
#include "helper.h"

namespace nssm{

    [[nodiscard]] std::vector<TCHAR> getVec(const TCHAR * const apNullTermString){
        std::vector<TCHAR> vec{};
        vec.reserve(std::char_traits<TCHAR>::length(apNullTermString) + 1U);
        vec.assign(apNullTermString, apNullTermString + std::char_traits<TCHAR>::length(apNullTermString) + 1U);
        return vec;
    }
}