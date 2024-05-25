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

for f in ../../personal/vault/*
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]] || [[ $f == *".bin"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    file_name="${f##*/}"
    echo "Processing $file_name"
    f_path="out/$file_name"
    cp $f $f_path

    h=$(identify -ping -format '%w' $f_path)
    w=$(identify -ping -format '%h' $f_path)
    if [ "$w" -ne 200 ] || [ "$h" -ne 200 ]; then
        echo "Image dimensions are not 200x200. Resizing."
        ../other/in/convert $f_path -resize 200x200! $f_path
        h=200
        w=200
    fi

    fc=$(echo -n ${file_name%.*})

    ../other/in/convert $f_path -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/img/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-cbc -K "$(echo -n "$vault_password" | xxd -p -c 16)" -iv "$random_salt" -base64 > out/$fc

    rm $f_path

    #cat encrypted_data_base64.bin | base64 -d > encrypted_data.bin
    #cat encrypted_data.bin | openssl enc -aes-128-cbc -d -K "$(echo -n "$vault_password" | xxd -p -c 16)" -iv "$random_salt" | xxd > decrypted_image.bin
    #cat encrypted_data_base64.bin | base64 -d > vault/$fc
done

echo -n "encryptionworked" | openssl enc -aes-128-ecb -nosalt -K "$(echo -n "$vault_password" | xxd -p -c 16)" -base64 > out/conf/check_enc
echo -n "encryptionworked" > out/conf/check_dec
echo -n $random_salt | openssl enc -aes-128-ecb -nosalt -K "$(echo -n "$vault_password" | xxd -p -c 16)" -base64 > out/conf/sault

rm -rf ../fs/littlefs/vault/ 1>/dev/null 2>/dev/null
mv out/ ../fs/littlefs/vault

# Some testing commands
# On arch linux install xxd-standalone
# Working decryption
# convert testImg.png -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | md5sum

# convert testImg.png -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-256-ecb -k "keykeykeyykeykey" -base64 | openssl enc -aes-256-ecb -d -k "keykeykeyykeykey" -base64 | md5sum

#convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-ecb -k "${vault_password}" -base64 2>/dev/null | xxd -i -n $fnel | sed 's/unsigned/const unsigned/g' >> vault.h

#convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-ecb -k "$vault_password" -base64 > encrypted_data_base64.bin

#cat encrypted_data_base64.bin | base64 -d > encrypted_data.bin
#cat encrypted_data.bin | openssl enc -aes-128-ecb -d -k "$vault_password" | xxd > decrypted_image.bin

#xxd -i -n $fnel encrypted_data.bin | sed 's/unsigned/const unsigned/g' >> vault.h
