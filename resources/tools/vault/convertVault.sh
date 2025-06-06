#!/bin/bash
source ../globalFunctions.sh

config_file="../../../src/defines/confidential.h"
vault_password=$(grep '#define VAULT_PASSWORD' "$config_file" | awk '{print $3}' | tr -d '"')

if [[ $vault_password == 0* ]]; then
    echo "Passwords starts with 0 - that's not allowed"
    exit
fi

if [[ $vault_password =~ ^[0-9]+$ ]]; then
    echo "Password is valid"
    if [ ${#vault_password} -lt 16 ]; then
        echo "Password is too small, adding 0 at the end of it"
    fi
    while [ ${#vault_password} -lt 16 ]; do
        vault_password="${vault_password}0"
    done
else
    echo "Password is invalid or doesn't exist"
    exit
fi

echo "Using password: \"$vault_password\""

random_salt=$(openssl rand -hex 16)
echo "Random Salt: $random_salt"

rm -rf out 1>/dev/null 2>/dev/null
mkdir -p out/conf
mkdir -p out/totp

./convertImagesVault.sh $random_salt $vault_password
echo -e ''
./convertTOTPVault.sh $random_salt $vault_password

echo -n "encryptionworked" | openssl enc -aes-128-ecb -nosalt -K "$(echo -n "$vault_password" | xxd -p -c 16)" -base64 > out/conf/check_enc
echo -n "encryptionworked" > out/conf/check_dec
echo -n $random_salt | openssl enc -aes-128-ecb -nosalt -K "$(echo -n "$vault_password" | xxd -p -c 16)" -base64 > out/conf/sault

rm -rf ../fs/littlefs/vault/ 1>/dev/null 2>/dev/null
mv out/ ../fs/littlefs/vault