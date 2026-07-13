#include "spoe/varint.hpp"

#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

using spoe::wire::encode_varint;
using spoe::wire::varint_error;

struct varint_case{
    uint64_t value;
    std::vector<std::byte> expected;
};

const std::vector<varint_case> cases = {
    {0,{std::byte{0x00}}},
    {10,{std::byte{0x0A}}},
    {239,{std::byte{0xEF}}},                                                          // max 1-byte
    {240,{std::byte{0xF0}, std::byte{0x00}}},                                         // min 2-byte
    {2287,{std::byte{0xFF}, std::byte{0x7F}}},                                        // max 2-byte
    {2288,{std::byte{0xF0}, std::byte{0x80}, std::byte{0x00}}},                       // min 3-byte
    {264431ULL,{std::byte{0xFF}, std::byte{0xFF}, std::byte{0x7F}}},                  // max 3-byte
    {264432ULL,{std::byte{0xF0}, std::byte{0x80}, std::byte{0x80}, std::byte{0x00}}}, // min 4-byte
    {33818863ULL,{std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0x7F}}},                  // max 4-byte
    {33818864ULL,{std::byte{0xF0}, std::byte{0x80}, std::byte{0x80}, std::byte{0x80}, std::byte{0x00}}}, // min 5-byte
    {4328786159ULL,{std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0x7F}}},                  // max 5-byte
    {4328786160ULL,{std::byte{0xF0}, std::byte{0x80}, std::byte{0x80}, std::byte{0x80}, std::byte{0x80}, std::byte{0x00}}}, // min 6-byte
    {4294967296ULL,{std::byte{0xF0}, std::byte{0xF1}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0x7E}}},                  // 2^32
    {9223372036854775808ULL,{std::byte{0xF0}, std::byte{0xF1}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE},
                              std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0x06}}},        // 2^63
    {18446744073709551615ULL,{std::byte{0xFF}, std::byte{0xF0}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE},
                               std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0xFE}, std::byte{0x0E}}},       // UINT64_MAX, max 10-byte
};

void test_encode_table() {
    for(const auto& c : cases){
        std::byte buf[16]{};
        std::span<std::byte> out(buf);

        auto result = encode_varint(c.value, out);
    
        assert(result.has_value());
        assert(std::equal(c.expected.begin(), c.expected.end(), buf));
        assert(out.size() == std::size(buf) - c.expected.size());
    }
}

int main(){
    test_encode_table();
    return 0;
}