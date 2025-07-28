#pragma once

#include <map>
#include <string>
#include <vector>

namespace saltpipe {
    using Bytes = std::vector<unsigned char>;

    struct Config {
        std::string primary_key_id;
        std::map<std::string, Bytes> keys;
    };

    Config parse_config(const std::string& dotenv_path);

}  // namespace saltpipe
