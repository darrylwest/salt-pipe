#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace saltpipe {
    // Version string
    inline constexpr std::string_view VERSION = "0.1.0-103";

    inline constexpr std::string_view HELP_TEXT = R"(
Usage

Generate a key: openssl rand -hex 32

Setup: export SALT_PIPE_KEY=<your 32 bit hex key>

To encrypt plain data:

    cat file | salt-pipe enc > file.enc

To decrypt encrypted data:

    cat file.enc | salt-pipe dec > file.txt

)";

    using Bytes = std::vector<unsigned char>;

    Bytes hex_to_bytes(const std::string& hex);
    Bytes read_stdin();
    void write_stdout(const Bytes& data);
    Bytes encrypt(const Bytes& plaintext, const Bytes& key);
    Bytes decrypt(const Bytes& input, const Bytes& key);

}  // namespace saltpipe