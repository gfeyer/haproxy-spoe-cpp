#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>

namespace spoe::wire{

    enum class varint_error{
        buffer_too_small,
    };

    constexpr std::expected<void, varint_error>
    encode_varint(std::uint64_t value, std::span<std::byte>& out){

        if(out.size() < 1){
            return std::unexpected(varint_error::buffer_too_small);
        }

        if(value < 240){
            out[0] = static_cast<std::byte>(value);
            out = out.subspan(1);
            return {};
        }

        // write special 0xF byte signalling continuation and lower 4 bits 
        out[0] = static_cast<std::byte>(0xF0 | (value & 0x0F));
        out = out.subspan(1);
        value = (value - 240) >> 4;

        while(value >= 128){
            if(out.size() < 1){
                return std::unexpected(varint_error::buffer_too_small);
            }

            out[0] = static_cast<std::byte>(0x80 | (value & 0xFF));
            out = out.subspan(1);
            value = (value - 128) >> 7;
        }

        if(out.size() < 1){
            return std::unexpected(varint_error::buffer_too_small);
        }

        out[0] = static_cast<std::byte>(value);
        out = out.subspan(1);
        return {};
    }

}