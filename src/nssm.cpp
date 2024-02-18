//
// Created by Philip on 2024/02/18.
//

#include "nssm.hpp"
#include "console.hpp"
#include "helper.hpp"
#include "imports.hpp"
#include "service.hpp"
#include "wexception.hpp"
#include <format>
#include <iostream>
namespace nssm {}

using namespace nssm;

int wmain(const int c, const wchar_t *const argv[]) {
    try {
        Utf8Console console{is_console()};


        const auto args{getArgs(c, argv)};
        DllImports imports{};

        const auto services{get_services()};


        std::wcout << std::format(L"Is console: {}.\n", is_console());
    } catch (const wexception &e) {
        std::wcout << e.wwhat();
    } catch (const std::exception &e) {
        std::wcout << e.what();
    } catch (...) {
        std::wcout << "Unknown exception.";
    }
    std::wcout << std::flush;
    return 0;
}