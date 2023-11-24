//
// Created by Philip Botha on 2023/11/24.
//

#ifndef NSSM_HELPER_H
#define NSSM_HELPER_H

#include <cstddef>
#include <array>
#include <type_traits>
#include <Windows.h>

namespace nssm {

    template<typename T>
    concept Char_c = std::is_same_v<std::remove_cvref_t<T>, char> || std::is_same_v<std::remove_cvref_t<T>, wchar_t>;

    template<Char_c T, std::size_t N, std::size_t ... Is>
    [[nodiscard]] constexpr std::array<T, N - 1>
    to_array(const T (&a)[N], std::index_sequence<Is...>) noexcept{
        return {{a[Is]...}};
    }
    /// Template function to initialse an array with a litteral string.
    /// \tparam T
    /// \tparam N The size of the array.
    /// \param a The string litteral
    /// \return
    template<Char_c T, std::size_t N>
    [[nodiscard]] constexpr std::array<T, N - 1> to_array(const T (&a)[N]) noexcept{
        return to_array(a, std::make_index_sequence<N - 1>());
    }

    template<std::size_t N>
    [[nodiscard]] constexpr LSA_UNICODE_STRING getLsa(std::array<wchar_t, N> &arArray) noexcept{
        return LSA_UNICODE_STRING {
            .Length = static_cast<USHORT>(arArray.size() * sizeof(wchar_t)),
            .MaximumLength = static_cast<USHORT>(arArray.size() * sizeof(wchar_t)),
            .Buffer = arArray.data()
        };
    }
}

#endif //NSSM_HELPER_H
