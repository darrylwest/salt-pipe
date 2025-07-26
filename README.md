# Salt Pipe

```
 _______         __ __          ______ __
|     __|.---.-.|  |  |_ ______|   __ \__|.-----.-----.
|__     ||  _  ||  |   _|______|    __/  ||  _  |  -__|
|_______||___._||__|____|      |___|  |__||   __|_____|
                                          |__|
```

A small wrapper around sodium encrypt/decrypt to read from stdin -> encrypt -> write to stdout and the reverse stdin -> decrypt -> stdout.

**NOTE**: _this utility will probably work on any linux-ish system, but it has only been tested on Ubuntu24.10_

## Overview

* reads the encryption key from .env using dotenvx, key name is `SALT_PIPE_KEY`
* reads from `stdin` and either encrypts or decrypts then writes to `stdout`

### Example use:

#### Setup:

```
export SALT_PIPE_KEY=$(openssl rand -hex 32)
echo $SALT_PIPE_KEY
```

#### Encrypt a file:

`cat plain-file | salt-pipe --encrypt > file.enc`

#### Decrypt an encrypted file

`cat file.enc | salt-pipe --decrypt > plain-file

#### Recommendations

* create a .env file with SALT_PIPE_KEY=<your secret key>
* use dotenvx to encrypt
* use dotenvx

## Use Case: Droplet -> s3 and s3 -> Droplet

### Sending data to s3/spaces:

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌─────────────┐
│   Source    │───▶│   Encrypt    │───▶│   AWS CLI   │───▶│ DO Spaces   │
│    Data     │    │ AES-256-GCM  │    │     cp      │    │   Bucket    │
└─────────────┘    └──────────────┘    └─────────────┘    └─────────────┘

```

* **source data** might be a file or it could by the output of a web-socket data transfer
* encryption is done with sodium in an executable called salt-pipe
* aws cli is used to do the actual transfer from the droplet to s3
* the destination is an ecrypted file in the designated bucket and folder with an extension of `.enc` (encoded)

### Reading data fro s3/spaces

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌─────────────┐
│ DO Spaces   │───▶│   AWS CLI    │───▶│   Decrypt   │───▶│ Destination │
│   Bucket    │    │     cp       │    │ AES-256-GCM │    │    Data     │
└─────────────┘    └──────────────┘    └─────────────┘    └─────────────┘
```

* the data source is an ecrypted file in the designated bucket and folder with an extension of `.enc`
* aws cli is used to do the actual transfer from s3 to the droplet
* encryption is done with sodium in an executable called salt-pipe
* The destination data might be a file or it could by the input of a web-socket data transfer


###### dpw | 2025.07.26



