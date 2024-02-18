#ifndef CONSOLE_H
#define CONSOLE_H

//bool check_console();
//void alloc_console(nssm_service_t *);


namespace nssm{
    [[nodiscard]] bool check_console();
    void alloc_console(const nssm_service_t &service);
}

#endif
