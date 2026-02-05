#pragma once
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "mf")

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include "wil/resource.h"
#include "wil/cppwinrt.h"
#include <wil/result_macros.h>
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"

#include <cstdio>
#include <iterator>
#include <format>
#include <print>

namespace winrt {
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Storage;
    using namespace Windows::Storage::Streams;
}

//template <typename ... Args>
//static constexpr void println(std::string_view format, Args ... args)
//{
//    ::printf_s(
//        std::vformat(format, std::make_format_args(args...)).data()
//    );
//
//    
//}
//
//#define PRINT_LN(fmt, ...) \
//    println(fmt ## "\n", __VA_ARGS__)
