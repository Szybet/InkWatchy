#!/bin/python3
import os
import shutil
from unidecode import unidecode

def remove_special_characters(text):
    return unidecode(text)

def split_and_store(file_path, chunk_size, output_path):
    with open(file_path, 'r') as file:
        content = file.read()

    content_clean = remove_special_characters(content)

    content_clean = content_clean.replace('\n', ' ')
    content_clean = content_clean.replace('"', ' ')
    content_clean = content_clean.replace('„', '<')
    content_clean = content_clean.replace('”', '>')

    content_clean = content_clean.replace("     ", ' ')
    content_clean = content_clean.replace("    ", ' ')
    content_clean = content_clean.replace("   ", ' ')
    content_clean = content_clean.replace("  ", ' ')

    chunks = [content_clean[i:i + chunk_size] for i in range(0, len(content_clean), chunk_size)]

    output_dir = output_path + 'book/'
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)

    for i, chunk in enumerate(chunks):
        with open(f'{output_dir}{i}', 'w') as file:
            file.write(chunk)

file_path = 'book.txt'
output_path = '../fs/littlefs/' 
if os.path.exists(file_path):
    split_and_store(file_path, 159, output_path)
    print(f"Newlines removed in {file_path}. Book saved to {output_path}book/")
else:
    print(file_path + " doesn't exist - skipping book generation")