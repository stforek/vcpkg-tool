#pragma once

#include <vcpkg/base/chrono.h>
#include <vcpkg/base/lineinfo.h>
#include <vcpkg/base/messages.h>
#include <vcpkg/base/strings.h>

#include <atomic>

namespace vcpkg::Debug
{
    extern std::atomic<bool> g_debugging;

    template<class... Args>
    void print(const Args&... args)
    {
        if (g_debugging) msg::write_unlocalized_text_to_stdout(Color::none, Strings::concat("[DEBUG] ", args...));
    }

    template<class F, class R = std::result_of_t<F && ()>, class = std::enable_if_t<!std::is_void<R>::value>>
    R time(LineInfo line, F&& f)
    {
        if (g_debugging)
        {
            auto timer = ElapsedTimer::create_started();
            auto&& result = f();
            print(line, " took ", timer, '\n');
            return static_cast<R&&>(result);
        }
        else
            return f();
    }

    template<class F, class R = std::result_of_t<F && ()>, class = std::enable_if_t<std::is_void<R>::value>>
    void time(LineInfo line, F&& f)
    {
        if (g_debugging)
        {
            auto timer = ElapsedTimer::create_started();
            f();
            print(line, " took ", timer, '\n');
        }
        else
            f();
    }
}
