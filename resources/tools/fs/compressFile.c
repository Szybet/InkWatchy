#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // For uint32_t

#include "compressor.h"
#include "common.h"

#define WINDOW_BITS 10
#define LITERAL_BITS 8
#define WINDOW_SIZE (1 << WINDOW_BITS)
#define INPUT_BUFFER_SIZE 1024 * 16
#define OUTPUT_BUFFER_SIZE 1024 * 16 // A reasonable buffer size

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    // Seek to the end of the input file to get its size
    fseek(input_file, 0, SEEK_END);
    uint32_t original_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET); // Rewind to the beginning

    // Write placeholder for original_size and compressed_size
    // These will be updated later
    uint32_t placeholder = 0;
    fwrite(&placeholder, sizeof(uint32_t), 1, output_file); // Original size placeholder
    fwrite(&placeholder, sizeof(uint32_t), 1, output_file); // Compressed size placeholder

    TampCompressor compressor;
    TampConf conf = {
        .window = WINDOW_BITS,
        .literal = LITERAL_BITS,
        .use_custom_dictionary = 0,
    };

    unsigned char window[WINDOW_SIZE];
    tamp_res res = tamp_compressor_init(&compressor, &conf, window);
    if (res != TAMP_OK) {
        fprintf(stderr, "Error initializing compressor: %d\n", res);
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    unsigned char input_buffer[INPUT_BUFFER_SIZE];
    unsigned char output_buffer[OUTPUT_BUFFER_SIZE];
    size_t bytes_read;
    size_t total_compressed_bytes = 0;

    size_t output_written = 0;
    while ((bytes_read = fread(input_buffer, 1, INPUT_BUFFER_SIZE, input_file)) > 0) {
        size_t input_consumed = 0;
        while (input_consumed < bytes_read) {
            res = tamp_compressor_compress(&compressor, output_buffer, OUTPUT_BUFFER_SIZE,
                                           &output_written,
                                           input_buffer + input_consumed, bytes_read - input_consumed,
                                           &input_consumed);

            if (output_written > 0) {
                fwrite(output_buffer, 1, output_written, output_file);
                total_compressed_bytes += output_written;
            }

            if (res != TAMP_OK && res != TAMP_OUTPUT_FULL) {
                fprintf(stderr, "Error during compression: %d\n", res);
                fclose(input_file);
                fclose(output_file);
                return 1;
            }
        }
    }

    // Flush any remaining data
    output_written = 0;
    res = tamp_compressor_flush(&compressor, output_buffer, OUTPUT_BUFFER_SIZE, &output_written, true);
    if (output_written > 0) {
        fwrite(output_buffer, 1, output_written, output_file);
        total_compressed_bytes += output_written;
    }
    if (res != TAMP_OK) {
        fprintf(stderr, "Error flushing compressor: %d\n", res);
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Update original_size and compressed_size at the beginning of the file
    fseek(output_file, 0, SEEK_SET);
    fwrite(&original_size, sizeof(uint32_t), 1, output_file);
    fwrite(&total_compressed_bytes, sizeof(uint32_t), 1, output_file);

    fclose(input_file);
    fclose(output_file);

    fprintf(stdout, "Compressed: %s (Original: %u bytes, Compressed: %zu bytes, Saved: %.2f%%)\n",
            input_filename, original_size, total_compressed_bytes,
            original_size > 0 ? ((double)(original_size - total_compressed_bytes) / original_size) * 100.0 : 0.0);

    return 0;
}
