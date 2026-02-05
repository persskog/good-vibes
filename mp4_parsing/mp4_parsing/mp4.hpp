#pragma once
#include <cstdint>
#include <format>
#include <iterator>
#include <span>

struct mp4_box_type
{
    char value[4]{};

    std::span<uint8_t> as_byte_span() noexcept
    {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(value);
        return { ptr, std::size(value) };
    }

    std::string_view as_string() const noexcept
    {
        return { value, std::size(value) };
    }
};

struct mp4_box
{
    uint32_t     size;
    mp4_box_type type;
    uint64_t     largesize; // This is only valid if size == 1

    static constexpr auto bytes() noexcept { return sizeof(mp4_box); }
};

//constexpr uint32_t mp4_box_header_size = 16u;

template <>
struct std::formatter<mp4_box> : std::formatter<std::string_view>
{
    auto format(const mp4_box& box, auto& ctx) const
    {
        return std::formatter<std::string_view>::format(
            std::format("({}) size: {}, large size: {}",
                box.type.as_string(),
                box.size,
                box.largesize)
            , ctx);
    }
};