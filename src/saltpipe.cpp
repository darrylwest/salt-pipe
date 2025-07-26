#include <saltpipe/saltpipe.hpp>
#include <sodium.h>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace saltpipe {

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

Bytes encrypt(const Bytes& plaintext, const Bytes& key) {
    Bytes nonce(crypto_secretbox_NONCEBYTES);
    randombytes_buf(nonce.data(), nonce.size());

    Bytes ciphertext(crypto_secretbox_MACBYTES + plaintext.size());

    crypto_secretbox_easy(ciphertext.data(), plaintext.data(), plaintext.size(), nonce.data(), key.data());

    // prepend nonce
    Bytes result;
    result.reserve(nonce.size() + ciphertext.size());
    result.insert(result.end(), nonce.begin(), nonce.end());
    result.insert(result.end(), ciphertext.begin(), ciphertext.end());
    return result;
}

Bytes decrypt(const Bytes& input, const Bytes& key) {
    if (input.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES)
        throw std::runtime_error("Input too short");

    Bytes nonce(input.begin(), input.begin() + crypto_secretbox_NONCEBYTES);
    Bytes ciphertext(input.begin() + crypto_secretbox_NONCEBYTES, input.end());
    Bytes decrypted(ciphertext.size() - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(decrypted.data(), ciphertext.data(), ciphertext.size(), nonce.data(), key.data())
        != 0) {
        throw std::runtime_error("Decryption failed");
    }
    return decrypted;
}

}
