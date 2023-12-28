#!/bin/python3
import os
import shutil
from unidecode import unidecode

def remove_special_characters(text):
    return unidecode(text)

def remove_newlines(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
    content_without_newlines = content.replace('\n', ' ')
    content_without_newlines = content_without_newlines.replace("     ", ' ')
    content_without_newlines = content_without_newlines.replace("    ", ' ')
    content_without_newlines = content_without_newlines.replace("   ", ' ')
    content_without_newlines = content_without_newlines.replace("  ", ' ')
    with open(file_path, 'w') as file:
        file.write(content_without_newlines)


def split_and_store(file_path, chunk_size, output_file, output_path):
    with open(file_path, 'r') as file:
        content = file.read()
    content_without_special_chars = remove_special_characters(content)
    chunks = [content_without_special_chars[i:i + chunk_size] for i in range(0, len(content_without_special_chars),chunk_size)]    
    cpp_list = "const String bookList[BOOK_PAGES] = {"
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
remove_newlines(file_path)
split_and_store(file_path, 50, output_file_path, output_path)
print(f"Newlines removed in {file_path}. book saved to {output_path}{output_file_path}.")
