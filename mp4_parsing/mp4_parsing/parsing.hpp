#pragma once
#include "winrt/Windows.Foundation.h"

inline auto Initialize()
{
    wil::SetResultLoggingCallback([](wil::FailureInfo const& failure) noexcept
        {
            wchar_t msg[2048] = {};
            if (SUCCEEDED(wil::GetFailureLogString(msg, std::size(msg), failure)))
            {
                std::fputws(msg, stderr);
            }
        });

    THROW_IF_FAILED(::MFStartup(MF_VERSION, MFSTARTUP_LITE));
    return wil::scope_exit([] { ::MFShutdown(); });
}

// Asynchronous function to run MP4 parsing
winrt::IAsyncAction RunAsync();
