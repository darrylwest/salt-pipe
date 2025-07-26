#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace saltpipe {
    // Version string
    inline constexpr std::string_view VERSION = "0.1.0-101";

    using Bytes = std::vector<unsigned char>;

    Bytes hex_to_bytes(const std::string& hex);
    Bytes read_stdin();
    void write_stdout(const Bytes& data);
    Bytes encrypt(const Bytes& plaintext, const Bytes& key);
    Bytes decrypt(const Bytes& input, const Bytes& key);

}  // namespace saltpipe