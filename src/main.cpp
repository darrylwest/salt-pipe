
#include <sodium.h>

#include <cstdlib>
#include <iostream>
#include <saltpipe/saltpipe.hpp>
#include <stdexcept>
#include <string>

void show_version() {
    std::cout << "Version: " << saltpipe::VERSION << '\n';
}

void show_help() {
    std::cout << saltpipe::HELP_TEXT << '\n';
    show_version();
}

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string(argv[1]) == "--help")) {
        show_help();
        return 0;
    }

    if (sodium_init() < 0) {
        std::cerr << "libsodium init failed\n";
        return 1;
    }

    if (argc != 2 || (std::string(argv[1]) != "enc" && std::string(argv[1]) != "dec")) {
        std::cerr << "ERROR! " << argv[0] << "need either enc or dec\n";
        show_help();
        return 1;
    }

    try {
        saltpipe::Config config = saltpipe::load_config_from_env();
        saltpipe::Bytes input = saltpipe::read_stdin();
        saltpipe::Bytes output
            = (std::string(argv[1]) == "enc") ? saltpipe::encrypt(input, config) : saltpipe::decrypt(input, config);
        saltpipe::write_stdout(output);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
