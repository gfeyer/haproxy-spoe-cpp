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

        // TODO: update for values >= 240
        return std::unexpected(varint_error::buffer_too_small);

    }

}