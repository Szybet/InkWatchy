import struct
import sys
import os
import subprocess

def compress_file(original_file_path):
    if not os.path.exists(original_file_path):
        print(f"Error: File not found at {original_file_path}", file=sys.stderr)
        sys.exit(1)

    original_size = os.path.getsize(original_file_path)
    compressed_temp_file = f"{original_file_path}.tamp_compressed"

    try:
        # Run tamp compress with explicit --input and --output flags
        subprocess_cmd = [
            "tamp",
            "compress",
            "--input", original_file_path,
            "--output", compressed_temp_file,
            "--window", "10",
            "--literal", "8"
        ]
        subprocess.run(
            subprocess_cmd,
            check=True,
            stderr=subprocess.PIPE
        )
    except subprocess.CalledProcessError as e:
        print(f"Error compressing file with tamp: {e.stderr.decode()}\nOriginal file: {original_file_path}", file=sys.stderr)
        if os.path.exists(compressed_temp_file):
            os.remove(compressed_temp_file)
        sys.exit(1)
    except FileNotFoundError:
        print("Error: 'tamp' command not found. Please ensure TAMP is installed and in your PATH.", file=sys.stderr)
        sys.exit(1)

    compressed_size = os.path.getsize(compressed_temp_file)

    # Calculate percentage saved
    percentage_saved = 0.0
    if original_size > 0:
        percentage_saved = ((original_size - compressed_size) / original_size) * 100

    print(f"Compressed: {original_file_path} (Original: {original_size} bytes, Compressed: {compressed_size} bytes, Saved: {percentage_saved:.2f}%) ")

    # Read the compressed data from the temporary file
    with open(compressed_temp_file, 'rb') as f_in:
        compressed_data = f_in.read()

    # Remove the temporary compressed file
    os.remove(compressed_temp_file)

    # Pack sizes as 4-byte little-endian unsigned integers (assuming size_t is 4 bytes on ESP32)
    original_size_bytes = struct.pack('<I', original_size)
    compressed_size_bytes = struct.pack('<I', compressed_size)

    # Write the sizes and then the compressed data back to the original file path
    with open(original_file_path, 'wb') as f_out:
        f_out.write(original_size_bytes)
        f_out.write(compressed_size_bytes)
        f_out.write(compressed_data)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 compressFile.py <file_path>", file=sys.stderr)
        sys.exit(1)
    compress_file(sys.argv[1])
