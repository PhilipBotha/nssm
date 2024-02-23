//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_NSSM_HPP
#define NSSM_NSSM_HPP

namespace nssm{
    enum class Startup {
        Automatic = 0,
        Delayed,
        Manual,
        Disabled
    };
}

extern "C" int wmain(int c, const wchar_t * const argv[]);
#endif //NSSM_NSSM_HPP
