
#include <saltpipe/saltpipe.hpp>
#include <sodium.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (sodium_init() < 0) {
        std::cerr << "libsodium init failed\n";
        return 1;
    }

    if (argc != 2 || (std::string(argv[1]) != "enc" && std::string(argv[1]) != "dec")) {
        std::cerr << "Usage: " << argv[0] << " enc|dec\n";
        return 1;
    }

    const char* env_key = std::getenv("SALT_PIPE_KEY");
    if (!env_key) {
        std::cerr << "Environment variable SALT_PIPE_KEY not set\n";
        return 1;
    }

    saltpipe::Bytes key = saltpipe::hex_to_bytes(env_key);
    saltpipe::Bytes input = saltpipe::read_stdin();

    try {
        saltpipe::Bytes output = (std::string(argv[1]) == "enc") ? saltpipe::encrypt(input, key)
                                                              : saltpipe::decrypt(input, key);
        saltpipe::write_stdout(output);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
