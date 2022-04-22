#pragma once

#include <string_view>
#include <chrono>
#include <functional>
#include <exception>
#include "util/log.h"

class CTime {
public:
    CTime() noexcept = default;

    [[maybe_unused]] inline void Set(const std::chrono::nanoseconds& time) noexcept { m_Time = time; }
    [[maybe_unused]] inline void Set(const std::chrono::microseconds& time) noexcept { m_Time = std::chrono::duration_cast<std::chrono::microseconds>(time); }
    [[maybe_unused]] inline void Set(const std::chrono::milliseconds& time) noexcept { m_Time = std::chrono::duration_cast<std::chrono::milliseconds>(time); }
    [[maybe_unused]] inline void Set(const std::chrono::seconds& time) noexcept { m_Time = std::chrono::duration_cast<std::chrono::seconds>(time); }
    [[maybe_unused]] inline void Set(const std::chrono::minutes& time) noexcept { m_Time = std::chrono::duration_cast<std::chrono::minutes>(time); }

public:
    [[maybe_unused]] [[nodiscard]] inline auto AsNano() const noexcept { return m_Time.count(); }
    [[maybe_unused]] [[nodiscard]] inline auto AsMicro() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(m_Time).count(); }
    [[maybe_unused]] [[nodiscard]] inline auto AsMilli() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(m_Time).count(); }
    [[maybe_unused]] [[nodiscard]] inline auto AsSecond() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(m_Time).count(); }
    [[maybe_unused]] [[nodiscard]] inline auto AsMinutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(m_Time).count(); }

private:
    std::chrono::nanoseconds m_Time;
};

class CTimer {
public:
    CTimer() noexcept = default;

    inline CTimer(const CTimer& other) {
        *this = other;
    }

    inline explicit CTimer(std::string_view name, const bool& autoExit = true) noexcept
        : m_Name(name), m_bAutoExit(autoExit), m_bStopped(false), m_bPaused(false) {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    inline ~CTimer() noexcept {
        if (m_bStopped || !m_bAutoExit)
            return;

        m_bStopped = true;

        const auto pauseTime = m_PauseStart - m_PauseEnd;
        const auto duration = std::chrono::high_resolution_clock::now() - (m_Start - pauseTime);

        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration).count();
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        std::string sResult = std::string("[MONITOR] ") + m_Name.data() + std::string(": ") + std::string("took ");

        if (minutes > 0) sResult += std::to_string(minutes) + "m";
        else if (seconds > 0) sResult += std::to_string(seconds) + "s";
        else if (milliseconds > 0) sResult += std::to_string(milliseconds) + "ms";
        else if (microseconds > 0) sResult += std::to_string(microseconds) + "us";
        else sResult += std::to_string(nanoseconds) + "ns";

        LogVerbose(sResult.c_str());
    }

public:
    /* General Functions */
    [[maybe_unused]] inline CTime Stop() noexcept {
        if (m_bStopped)
            return {};

        m_bStopped = true;
        return GetDuration();
    }

    [[maybe_unused]] inline void Pause() noexcept {
        if (m_bPaused)
            return;

        m_PauseStart = std::chrono::high_resolution_clock::now();
        m_bPaused = true;
    }

    [[maybe_unused]] inline void Resume() noexcept {
        if (!m_bPaused)
            return;

        m_PauseEnd = std::chrono::high_resolution_clock::now();
        m_bPaused = false;
    }

    [[maybe_unused]] inline void Restart() noexcept {
        m_Start = std::chrono::high_resolution_clock::now();
        m_PauseStart = {};
        m_PauseEnd = {};
        m_bStopped = false;
        m_bPaused = false;
    }

public:
    /* Accessors */
    [[maybe_unused]] [[nodiscard]] inline CTime GetDuration() noexcept {
        const auto pauseTime = m_PauseStart - m_PauseEnd;
        const auto duration = std::chrono::high_resolution_clock::now() - m_Start - pauseTime;

        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

        if (minutes.count() > 0) m_Time.Set(minutes);
        else if (seconds.count() > 0) m_Time.Set(seconds);
        else if (milliseconds.count() > 0) m_Time.Set(milliseconds);
        else if (microseconds.count() > 0) m_Time.Set(microseconds);
        else m_Time.Set(nanoseconds);

        return m_Time;
    }

    [[maybe_unused]] [[nodiscard]] inline std::string_view GetName() const noexcept { return m_Name; }
    [[maybe_unused]] [[nodiscard]] inline bool IsStopped() const noexcept { return m_bStopped; }
    [[maybe_unused]] [[nodiscard]] inline bool IsPaused() const noexcept { return m_bPaused; }

public:
    /* Mutators */
    [[maybe_unused]] inline void SetName(std::string_view name) noexcept { m_Name = name; }
    [[maybe_unused]] inline void SetAutoExit(const bool& autoExit) noexcept { m_bAutoExit = autoExit; }

public:
    /* Operators */
    [[maybe_unused]] [[nodiscard]] inline CTime operator()() { return GetDuration(); }

    [[nodiscard]] bool operator==(CTimer& other) { return GetDuration().AsMicro() == other().AsMicro(); }
    [[nodiscard]] bool operator!=(CTimer& other) { return GetDuration().AsMicro() != other().AsMicro(); }

    [[nodiscard]] std::strong_ordering operator<=>(CTimer& other) { return GetDuration().AsMicro() <=> other().AsMicro(); }

private:
    CTime m_Time {};

    std::chrono::high_resolution_clock::time_point m_Start = {};
    std::chrono::high_resolution_clock::time_point m_PauseStart = {};
    std::chrono::high_resolution_clock::time_point m_PauseEnd = {};

    std::string_view m_Name = {};

    bool m_bStopped = false;
    bool m_bPaused = false;
    bool m_bAutoExit = false;
};

#define MONITOR() CTimer(__FUNCTION__)