//
// Created by Philip on 2024/02/18.
//

#include "event.hpp"
#include "wexception.hpp"
#include <cstddef>
#include <vector>
#include <windows.h>
namespace {}

namespace nssm {
    void log_event(EventLogType type, unsigned long id, ...) {
        va_list arg;

        const wchar_t *s;
        std::vector<const wchar_t *> strings{};
        strings.reserve(NSSM_NUM_EVENT_STRINGS);
        // Open event log
        EventSource eventSource{};

        // Log it
        auto count{0ULL};
        va_start(arg, id);
        while ((s = va_arg(arg, const wchar_t *)) && (count < (NSSM_NUM_EVENT_STRINGS - 1U))) {
            strings.push_back(s);
            ++count;
        }
        va_end(arg);
        ReportEventW(eventSource.handle(), static_cast<unsigned short>(type), 0, id, nullptr,
                     static_cast<WORD>(strings.size()), 0, &strings[0], nullptr);
    }

    EventSource::EventSource() {
        mpHandle = RegisterEventSourceW(nullptr, NSSM_SOURCE);
        if (!mpHandle) {
            throw wexception{L"Could not register event source."};
        }
    }
    EventSource::~EventSource() {
        if (mpHandle) {
            DeregisterEventSource(mpHandle);
            mpHandle = nullptr;
        }
    }
    EventSource &EventSource::operator=(EventSource &&other) noexcept {
        if (this != &other) {
            mpHandle       = other.mpHandle;
            other.mpHandle = nullptr;
        }
        return *this;
    }
    EventSource::EventSource(EventSource &&other) noexcept : mpHandle{nullptr} {
        mpHandle       = other.mpHandle;
        other.mpHandle = nullptr;
    }
    void *EventSource::handle() {
        return mpHandle;
    }
} // namespace nssm