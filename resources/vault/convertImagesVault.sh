#!/bin/bash

count=0
vault_list="const ImageDef vaultList[] = {"
vault_list_names="const String vaultListNames[] = {"
config_file="../../src/defines/confidential.h"
vault_password=$(grep '#define VAULT_PASSWORD' "$config_file" | awk '{print $3}' | tr -d '"')
echo "Using password: \"$vault_password\""


if [[ $vault_password =~ ^[0-9]+$ ]]; then
    echo "Password is valid"
else
    echo "Password is invalid or doesn't exist"
    #exit
fi

: '
# Not needed anymore
random_salt=$(openssl rand -hex 16)
echo "Random Salt: $random_salt"
random_salt_define="#define VAULT_SAULT \"$random_salt\""
search_line="#define VAULT_SAULT"
after_line="#define VAULT_PASSWORD"
if grep -q "$search_line" "$config_file"; then
    # Line exists, replace it with a new string
    sed -i "/$search_line/c $random_salt_define" "$config_file"
else
    sed -i "/$after_line/a\\$random_salt_define" "$config_file"
fi
'

rm vault.h 1>/dev/null 2>/dev/null
touch vault.h

echo -e "#ifndef VAULT_H" >> vault.h
echo -e "#define VAULT_H" >> vault.h
echo -e '' >> vault.h

echo '#define VAULT_IMAGE_LIST_SIZE 5000' >> vault.h
echo -e '' >> vault.h

for f in *
do
    if [[ $f == *".sh"* ]] || [[ $f == *".h"* ]] || [[ $f == "eink-2color.png" ]] || [[ $f == *".txt"* ]] || [[ $f == *".bin"* ]]; then
        continue
    fi

    if [ ! -f "$f" ]; then
        continue
    fi

    echo "Processing $f" # always double quote "$f" filename
    
    h=$(identify -ping -format '%w' $f)
    w=$(identify -ping -format '%h' $f)
    if [ "$w" -ne 200 ] || [ "$h" -ne 200 ]; then
    echo "Image dimensions are not 200x200. Exiting script."
    exit
    fi

    fnel=$(echo ${f%.*}VaultImg)

    # On arch linux install xxd-standalone
    # Working decryption
    # convert testImg.png -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | md5sum

    # convert testImg.png -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-256-ecb -k "keykeykeyykeykey" -base64 | openssl enc -aes-256-ecb -d -k "keykeykeyykeykey" -base64 | md5sum

    #convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-ecb -k "${vault_password}" -base64 2>/dev/null | xxd -i -n $fnel | sed 's/unsigned/const unsigned/g' >> vault.h
    
    #convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-ecb -k "$vault_password" -base64 > encrypted_data_base64.bin
    
    #cat encrypted_data_base64.bin | base64 -d > encrypted_data.bin
    #cat encrypted_data.bin | openssl enc -aes-128-ecb -d -k "$vault_password" | xxd > decrypted_image.bin
    
    #xxd -i -n $fnel encrypted_data.bin | sed 's/unsigned/const unsigned/g' >> vault.h

    iv=$(echo -n "$vault_password" | xxd -p -c 16)

    echo "iv is: $iv"

    # Encrypt the data using AES-128-CBC with the hashed vault password as the IV
    convert $f -dither FloydSteinberg -define dither:diffusion-amount=90% -remap ../images/eink-2color.png -depth 1 gray:- | openssl enc -aes-128-cbc -K "$(echo -n "$vault_password" | xxd -p -c 16)" -iv "$iv" -base64 > encrypted_data_base64.bin

    cat encrypted_data_base64.bin | base64 -d > encrypted_data.bin

    # Decrypt the data using AES-128-CBC with the same hashed vault password as the IV
    cat encrypted_data.bin | openssl enc -aes-128-cbc -d -K "$(echo -n "$vault_password" | xxd -p -c 16)" -iv "$iv" | xxd > decrypted_image.bin

    # Generate the header file with the encrypted data
    xxd -i -n $fnel encrypted_data_base64.bin | sed 's/unsigned/const unsigned/g' >> vault.h

    echo -e "const ImageDef ${fnel}Pack = {${fnel}, 200, 200};" >> vault.h

    vault_list="$vault_list${fnel}Pack, "
    vault_list_names="$vault_list_names\"${f%.*}\", "
    ((count++))
done

vault_list_repaired="${vault_list%??}"
vault_list_repaired="$vault_list_repaired};"

vault_list_names_repaired="${vault_list_names%??}"
vault_list_names_repaired="$vault_list_names_repaired};"

echo -e '' >> vault.h
echo $vault_list_names_repaired >> vault.h
echo -e $vault_list_repaired >> vault.h
echo -e '' >> vault.h
echo "#endif" >> vault.h

mv vault.h ../../src/defines/
