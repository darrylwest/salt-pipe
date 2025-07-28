#include <sodium.h>

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <saltpipe/saltpipe.hpp>
#include <stdexcept>

extern char** environ;

namespace saltpipe {

    Config load_config_from_env() {
        Config config;
        const char* primary_key_id_env = std::getenv("PRIMARY_KEY");
        if (!primary_key_id_env) {
            throw std::runtime_error("PRIMARY_KEY environment variable not set");
        }
        config.primary_key_id = primary_key_id_env;

        for (char** env = environ; *env != nullptr; ++env) {
            std::string env_var(*env);
            if (env_var.rfind("KEY_", 0) == 0) {
                size_t pos = env_var.find('=');
                if (pos != std::string::npos) {
                    std::string key_name = env_var.substr(4, pos - 4);
                    std::string key_value = env_var.substr(pos + 1);
                    config.keys[key_name] = hex_to_bytes(key_value);
                }
            }
        }

        if (config.keys.find(config.primary_key_id) == config.keys.end()) {
            throw std::runtime_error("Primary key specified in PRIMARY_KEY not found in the list of keys");
        }
        return config;
    }

    Bytes hex_to_bytes(const std::string& hex) {
        if (hex.size() != crypto_secretbox_KEYBYTES * 2)
            throw std::runtime_error("Key must be 64 hex characters (32 bytes)");

        Bytes bytes(crypto_secretbox_KEYBYTES);
        sodium_hex2bin(bytes.data(), bytes.size(), hex.c_str(), hex.size(), nullptr, nullptr, nullptr);
        return bytes;
    }

    Bytes read_stdin() {
        std::cin >> std::noskipws;
        return Bytes(std::istream_iterator<unsigned char>(std::cin), {});
    }

    void write_stdout(const Bytes& data) { std::cout.write(reinterpret_cast<const char*>(data.data()), data.size()); }

    Bytes encrypt(const Bytes& plaintext, const Config& config) {
        const auto& primary_key_iter = config.keys.find(config.primary_key_id);
        const Bytes& key = primary_key_iter->second;
        const std::string& kid = config.primary_key_id;

        Bytes nonce(crypto_secretbox_NONCEBYTES);
        randombytes_buf(nonce.data(), nonce.size());

        Bytes ciphertext(crypto_secretbox_MACBYTES + plaintext.size());
        crypto_secretbox_easy(ciphertext.data(), plaintext.data(), plaintext.size(), nonce.data(), key.data());

        Bytes result;
        unsigned char kid_len = static_cast<unsigned char>(kid.length());
        result.push_back(kid_len);
        result.insert(result.end(), kid.begin(), kid.end());
        result.insert(result.end(), nonce.begin(), nonce.end());
        result.insert(result.end(), ciphertext.begin(), ciphertext.end());

        return result;
    }

    Bytes decrypt(const Bytes& input, const Config& config) {
        if (input.empty()) {
            throw std::runtime_error("Input is empty");
        }

        unsigned char kid_len = input[0];
        if (input.size() < 1 + kid_len + crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
            throw std::runtime_error("Input too short for key rotation format");
        }

        std::string kid(input.begin() + 1, input.begin() + 1 + kid_len);
        auto key_iter = config.keys.find(kid);
        if (key_iter == config.keys.end()) {
            throw std::runtime_error("Key ID not found: " + kid);
        }
        const Bytes& key = key_iter->second;

        const size_t nonce_offset = 1 + kid_len;
        const size_t ciphertext_offset = nonce_offset + crypto_secretbox_NONCEBYTES;

        Bytes nonce(input.begin() + nonce_offset, input.begin() + ciphertext_offset);
        Bytes ciphertext(input.begin() + ciphertext_offset, input.end());
        Bytes decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);

        if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data(), ciphertext.size(), nonce.data(), key.data()) != 0) {
            throw std::runtime_error("Decryption failed");
        }
        return decrypted;
    }

}  // namespace saltpipe