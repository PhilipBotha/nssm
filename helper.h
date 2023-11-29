//
// Created by Philip Botha on 2023/11/24.
//

#ifndef NSSM_HELPER_H
#define NSSM_HELPER_H

#include <cstddef>
#include <array>
#include <type_traits>
#include <Windows.h>
#include <vector>
#include <string>
#include <ntsecapi.h>

namespace nssm {

    template<typename T>
    concept Char_c = std::is_same_v<std::remove_cvref_t<T>, char> || std::is_same_v<std::remove_cvref_t<T>, wchar_t>;

    template<Char_c T, std::size_t N, std::size_t ... Is>
    [[nodiscard]] constexpr std::array<T, N - 1>
    to_array(const T (&a)[N], std::index_sequence<Is...>) noexcept{
        return {{a[Is]...}};
    }
    /// Template function to initialize an array with a literal string. The resultant array does not have the null
    /// terminating character.
    /// \tparam T Either a char or a wchar_t.
    /// \tparam N The length of the string including the null terminator.
    /// \param a The string literal
    /// \return An array containing the string literal without the null terminator.
    /// \note Do not use the data() member function to pass the string in the array to a function expecting a
    /// null terminated string. I.e. functions expecting char *.
    template<Char_c T, std::size_t N>
    [[nodiscard]] constexpr std::array<T, N - 1> to_array(const T (&a)[N]) noexcept{
        return to_array(a, std::make_index_sequence<N - 1>());
    }

    template<Char_c T, std::size_t N, std::size_t ... Is>
    [[nodiscard]] constexpr std::array<T, N>
    to_array_nullterm(const T (&a)[N], std::index_sequence<Is...>) noexcept{
        return {{a[Is]...}};
    }

    /// Template function to initialize an array with a literal string. The resultant array does have the null. The
    /// resultant array can be, using the data(), passed to functions expecting null terminated strings.
    /// terminating character.
    /// \tparam T Either a char or a wchar_t.
    /// \tparam N The length of the string including the null terminator.
    /// \param a The string literal
    /// \return An array containing the string literal including the null terminator.
    template<Char_c T, std::size_t N>
    [[nodiscard]] constexpr std::array<T, N> to_array_nullterm(const T (&a)[N]) noexcept{
        return to_array_nullterm(a, std::make_index_sequence<N>());
    }

    template<std::size_t N>
    [[nodiscard]] constexpr LSA_UNICODE_STRING getLsa(std::array<wchar_t, N> &arArray) noexcept{
        return LSA_UNICODE_STRING {
            .Length = static_cast<USHORT>(arArray.size() * sizeof(wchar_t)),
            .MaximumLength = static_cast<USHORT>(arArray.size() * sizeof(wchar_t)),
            .Buffer = arArray.data()
        };
    }

    /// Return vector of the string including the null terminator.
    /// \param apNullTermString
    /// \return Null terminated vector containing the string.
    [[nodiscard]] std::vector<TCHAR> getVec(const TCHAR * const apNullTermString);
}

#endif //NSSM_HELPER_H
