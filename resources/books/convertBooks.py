#!/bin/python3
import os
import shutil
from unidecode import unidecode

def remove_special_characters(text):
    return unidecode(text)

def split_and_store(file_path, chunk_size, output_file, output_path):
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

    chunks = [content_clean[i:i + chunk_size] for i in range(0, len(content_clean),chunk_size)]
    # Using String list causes esp_core_dump_flash: Core dump flash config is corrupted! CRC=0x7bd5c66f instead of 0x0
    # And uses a lot of memory... My god...
    cpp_list = "const char bookList[BOOK_PAGES][" + str(chunk_size + 1) + "] = {"
    cpp_list += ", ".join(f'"{chunk}"' for chunk in chunks)
    cpp_list += "};"
    cpp_list_count = f"#define BOOK_PAGES {len(chunks)}\n"
    with open(output_file, 'w') as file:
        file.write("#ifndef BOOK_H\n")
        file.write("#define BOOK_H\n\n")
        file.write(cpp_list_count + "\n")
        file.write(cpp_list + "\n\n")
        file.write("#endif // BOOK_H\n")

    if os.path.exists(output_path):
        os.remove(output_path + output_file)
    shutil.move(output_file, output_path)


file_path = 'book.txt'
output_file_path = 'book.h'
output_path = '../../src/defines/' 
split_and_store(file_path, 159, output_file_path, output_path)
print(f"Newlines removed in {file_path}. book saved to {output_path}{output_file_path}.")
