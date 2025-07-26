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

*   Reads the `SALT_PIPE_KEY` from an `.env` file, which is required for operation.
*   Reads from `stdin`, encrypts or decrypts, and writes the result to `stdout`.

### Example Use

#### Setup

Create a secret key and set the environment variable.

```sh
export SALT_PIPE_KEY=$(openssl rand -hex 32)
echo $SALT_PIPE_KEY
```

#### Encrypt a file

```sh
cat plain-file | salt-pipe enc > file.enc
```

#### Decrypt an encrypted file

```sh
cat file.enc | salt-pipe dec > plain-file
```

#### Recommendations

*   For persistent configuration, create a `.env` file with `SALT_PIPE_KEY=<your secret key>`.
*   Use a tool like `dotenvx` to manage your `.env` file, especially for security.

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

