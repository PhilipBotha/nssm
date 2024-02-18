//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_CONSOLE_HPP
#define NSSM_CONSOLE_HPP

namespace nssm {
    [[nodiscard]] bool is_console();

    class Utf8Console {
    public:
        Utf8Console() = delete;
        explicit Utf8Console(bool isConsole);

        // Deleting the copy constructor
        Utf8Console(const Utf8Console&) = delete;

        // Deleting the copy assignment operator
        Utf8Console& operator=(const Utf8Console&) = delete;

        // Enabling the move constructor
        Utf8Console(Utf8Console&&) noexcept;

        // Enabling the move assignment operator
        Utf8Console& operator=(Utf8Console&&) noexcept;

        ~Utf8Console();

    private:
        unsigned int cp{0U};
    };
} // namespace nssm

#endif //NSSM_CONSOLE_HPP