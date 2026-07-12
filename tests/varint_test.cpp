#include "spoe/varint.hpp"

#include <cassert>
#include <cstddef>
#include <span>

using spoe::wire::encode_varint;
using spoe::wire::varint_error;

void test_encode_ten() {
    std::byte buf[8]{};
    std::span<std::byte> out{buf};
    
    auto result = encode_varint(10, out);

    assert(result.has_value());
    assert(buf[0] == std::byte{0x0A});
    assert(out.size() == 7);
}

int main(){
    test_encode_ten();
    return 0;
}