#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace saltpipe {
    // Version string
    inline constexpr std::string_view VERSION = "0.1.0-103";

    inline constexpr std::string_view HELP_TEXT = R"(
Usage

Generate a key: openssl rand -hex 32

Setup:
export PRIMARY_KEY=v1
export KEY_V1=<your 32 bit hex key>
export KEY_V2=<your 32 bit hex key>

To encrypt plain data:

    cat file | salt-pipe enc > file.enc

To decrypt encrypted data:

    cat file.enc | salt-pipe dec > file.txt

)";

    using Bytes = std::vector<unsigned char>;

    struct Config {
        std::string primary_key_id;
        std::map<std::string, Bytes> keys;
    };

    Config load_config_from_env();
    Bytes hex_to_bytes(const std::string& hex);
    Bytes read_stdin();
    void write_stdout(const Bytes& data);
    Bytes encrypt(const Bytes& plaintext, const Config& config);
    Bytes decrypt(const Bytes& input, const Config& config);

}  // namespace saltpipe
