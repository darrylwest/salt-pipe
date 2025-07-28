# Salt Pipe

```
 _______         __ __          ______ __
|     __|.---.-.|  |  |_ ______|   __ \__|.-----.-----.
|__     ||  _  ||  |   _|______|    __/  ||  _  |  -__|
|_______||___._||__|____|      |___|  |__||   __|_____|
                                          |__|
```

A small wrapper around `libsodium` for encrypting and decrypting data. It reads from `stdin`, performs the requested operation, and writes the result to `stdout`.

**NOTE**: This utility is expected to work on most Unix-like systems, but it has only been formally tested on Ubuntu 24.10.

## Overview

*   Reads key configuration from environment variables.
*   `PRIMARY_KEY`: The ID of the key to use for encryption (e.g., `v1`).
*   `KEY_V1`, `KEY_V2`, etc.: The actual encryption keys, where the suffix (`V1`, `V2`) matches the key IDs.
*   Reads from `stdin`, encrypts or decrypts, and writes the result to `stdout`.

### Example Use

#### Setup

Create secret keys and set the environment variables. The `PRIMARY_KEY` variable tells `salt-pipe` which key to use for encryption. The other keys are available for decryption.

```sh
export PRIMARY_KEY=v1
export KEY_V1=$(openssl rand -hex 32)
export KEY_V2=$(openssl rand -hex 32)
```

#### Encrypt a file

```sh
cat plain-file | salt-pipe enc > file.enc
```

#### Decrypt an encrypted file

`salt-pipe` will automatically use the correct key for decryption based on the key ID embedded in the encrypted file.

```sh
cat file.enc | salt-pipe dec > plain-file
```

#### Recommendations

*   For persistent configuration, create a `.env` file.
*   Use a tool like `dotenvx` to manage your `.env` file, especially for security. `dotenvx run -- your-command` will load the `.env` file and make the variables available to your command.

## Use Case: Droplet -> S3 and S3 -> Droplet

### Sending data to S3/Spaces:

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌─────────────┐
│   Source    │───▶│   Encrypt    │───▶│   AWS CLI   │───▶│ DO Spaces   │
│    Data     │    │ AES-256-GCM  │    │     cp      │    │   Bucket    │
└─────────────┘    └──────────────┘    └─────────────┘    └─────────────┘

```

*   **Source Data**: Can be a file or the output of another process, like a web-socket data transfer.
*   **Encryption**: Performed by `salt-pipe`, which uses the `libsodium` library.
*   **Transfer**: The `aws` CLI handles the transfer from the droplet to S3.
*   **Destination**: The result is an encrypted file (with a `.enc` extension) stored in the specified S3 bucket.

### Reading data from S3/Spaces

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌─────────────┐
│ DO Spaces   │───▶│   AWS CLI    │───▶│   Decrypt   │───▶│ Destination │
│   Bucket    │    │     cp       │    │ AES-256-GCM │    │    Data     │
└─────────────┘    └──────────────┘    └─────────────┘    └─────────────┘
```

*   **Data Source**: An encrypted file (with a `.enc` extension) in the specified S3 bucket.
*   **Transfer**: The `aws` CLI handles the transfer from S3 to the droplet.
*   **Decryption**: Performed by `salt-pipe`.
*   **Destination Data**: Can be saved to a file or piped as input to another process.


###### dpw | 2025.07.26

