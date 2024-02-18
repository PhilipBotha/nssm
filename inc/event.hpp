//
// Created by Philip on 2024/02/18.
//

#ifndef NSSM_EVENT_HPP
#define NSSM_EVENT_HPP
#include <cstddef>
#include <cstdint>
namespace nssm {
    constexpr std::size_t NSSM_NUM_EVENT_STRINGS{16U};
    constexpr const wchar_t *const NSSM_SOURCE{L"NSSM"};

    enum class EventLogType : std::uint16_t{
        SUCCESS = 0x0000U,
        AUDIT_FAILURE = 0x0010U,
        AUDIT_SUCCESS = 0x0008U,
        ERROR_TYPE = 0x0001U,
        INFORMATION_TYPE = 0x0004U,
        WARNING_TYPE = 0x0002U
    };

    class EventSource {
    public:
        EventSource();
        EventSource(const EventSource &)            = delete;
        EventSource &operator=(const EventSource &) = delete;

        ~EventSource();
        // Move constructor
        EventSource(EventSource &&other) noexcept;

        // Move assignment operator
        EventSource &operator=(EventSource &&other) noexcept;

        void *handle();

    private:
        void *mpHandle{nullptr};
    };

    void log_event(EventLogType type, unsigned long id, ...);

} // namespace nssm
#endif //NSSM_EVENT_HPP
