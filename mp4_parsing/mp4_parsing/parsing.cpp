#include "pch.h"
#include "parsing.hpp"
#include <string>
#include "mp4.hpp"
#include <print>


using namespace winrt;

/*
    Internal
*/

static const auto MetaVideo  = LR"(D:\muxing\repair_meta.mp4)";
static const auto BrokenFile = LR"(D:\muxing\repair_broken.mp4)";
static const auto NewFile    = LR"(D:\muxing\fixed_broken.mp4)";

static auto CreateRandomAccessStream(IMFByteStream* stream)
{
    winrt::IRandomAccessStream ras{ nullptr };
    check_hresult(::MFCreateStreamOnMFByteStreamEx(
        stream,
        winrt::guid_of<winrt::IRandomAccessStream>(),
        winrt::put_abi(ras)));
    return ras;
}

static HRESULT CreateAndOpenStreams(std::wstring_view brokenFilePath,
                                    std::wstring_view filePath,
                                    IMFByteStream**   brokenStream,
                                    IMFByteStream**   fileStream)
{
    HRESULT hr = ::MFCreateFile(
        MF_ACCESSMODE_READ,
        MF_OPENMODE_FAIL_IF_NOT_EXIST,
        MF_FILEFLAGS_NONE,
        brokenFilePath.data(),
        brokenStream);

    if (SUCCEEDED(hr))
    {
        hr = ::MFCreateFile(
            MF_ACCESSMODE_READWRITE,
            MF_OPENMODE_DELETE_IF_EXIST,
            MF_FILEFLAGS_NONE,
            filePath.data(),
            fileStream);
    }

    return hr;
}

winrt::IAsyncAction ProcessAsync(winrt::IRandomAccessStream stream)
{
    std::vector<mp4_box> boxes;
    boxes.reserve(10);

    winrt::Buffer buffer{ 1024u };
    auto read = co_await resume_agile(stream.ReadAsync(buffer, mp4_box::bytes(), winrt::InputStreamOptions::None));
    uint64_t next_box_position{};

    do
    {
        auto reader = winrt::DataReader::FromBuffer(read);
        if (reader.UnconsumedBufferLength() < mp4_box::bytes())
        {
            break;
        }
        auto& box = boxes.emplace_back();
        box.size = reader.ReadUInt32();
        reader.ReadBytes(box.type.as_byte_span());
        auto largesize = reader.ReadUInt64();
        bool has_undtermined_size = (box.size == 0);
        if (box.size == 1)
        {
            box.largesize = largesize;
            next_box_position += box.largesize;
        }
        else
        {
            if (box.size)
            {
                next_box_position += box.size;
            }
        }

        if (!has_undtermined_size)
        {
            stream.Seek(next_box_position);
            read = co_await resume_agile(stream.ReadAsync(read, mp4_box::bytes(), winrt::InputStreamOptions::None));
        }
        else
        {
            break;
        }

    } while (true);

    for (const auto& box : boxes)
    {
        std::println("{}", box);
    }
}


/*
    Public
*/



winrt::IAsyncAction RunAsync()
{
    co_await winrt::resume_background();

    com_ptr<IMFByteStream> brokenStream;
    com_ptr<IMFByteStream> fileStream;
    THROW_IF_FAILED(CreateAndOpenStreams(BrokenFile, NewFile, brokenStream.put(), fileStream.put()));

    auto ras = CreateRandomAccessStream(brokenStream.get());
    co_await ProcessAsync(ras);
}
