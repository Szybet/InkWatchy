#!/bin/bash
source ../globalFunctions.sh

random_salt=$1
vault_password=$2



for f in ../../personal/vault/totp/*
do
    if [[ $f != *".txt"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    file_name="${f##*/}"
    echo "Processing $file_name"
    f_path="out/$file_name"
    cp $f $f_path

    # h=$(identify -ping -format '%w' $f_path)
    # w=$(identify -ping -format '%h' $f_path)
    # if [ "$w" -ne 200 ] || [ "$h" -ne 200 ]; then
    #     echo "Image dimensions are not 200x200. Resizing."
    #     ../other/in/magick $f_path -resize 200x200! $f_path
    #     h=200
    #     w=200
    # fi

    fc=$(echo -n ${file_name%.*})

    # ../other/in/magick $f_path -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/img/eink-2color.png -depth 1 gray:-
    cat $f_path | openssl enc -aes-128-ecb -nosalt -K "$(echo -n "$vault_password" | xxd -p -c 16)" -base64 > out/totp/$fc
    

    rm $f_path

    #cat encrypted_data_base64.bin | base64 -d > encrypted_data.bin
    #cat encrypted_data.bin | openssl enc -aes-128-cbc -d -K "$(echo -n "$vault_password" | xxd -p -c 16)" -iv "$random_salt" | xxd > decrypted_image.bin
    #cat encrypted_data_base64.bin | base64 -d > vault/$fc
done

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

# Latest linux command testing
# Pure image hash:
# ../other/in/magick ../../personal/vault/dog.jpg -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/img/eink-2color.png -depth 1 gray:- | md5sum

# Encrypted, decrypted hash
# ../other/in/magick ../../personal/vault/dog.jpg -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/img/eink-2color.png -depth 1 gray:- | openssl enc -aes-256-ecb -k "keykeykeyykeykey" -base64 | openssl enc -aes-256-ecb -d -k "keykeykeyykeykey" -base64 | md5sum