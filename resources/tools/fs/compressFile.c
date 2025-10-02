#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "compressor.h"
#include "common.h"

#define WINDOW_BITS 10
#define LITERAL_BITS 8
#define WINDOW_SIZE (1 << WINDOW_BITS)

// Global window buffer for Tamp compressor/decompressor
uint8_t window_buffer[WINDOW_SIZE];

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>", argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    // 1. Read input file
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file)
    {
        perror("Error opening input file");
        return 1;
    }

    fseek(input_file, 0, SEEK_END);
    long input_file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (input_file_size == -1)
    {
        perror("Error getting input file size");
        fclose(input_file);
        return 1;
    }

    uint8_t *input_buffer = (uint8_t *)malloc(input_file_size);
    if (!input_buffer)
    {
        perror("Error allocating memory for input buffer");
        fclose(input_file);
        return 1;
    }

    if (fread(input_buffer, 1, input_file_size, input_file) != input_file_size)
    {
        perror("Error reading input file");
        free(input_buffer);
        fclose(input_file);
        return 1;
    }
    fclose(input_file);

    // 2. Initialize Tamp Compressor
    TampCompressor tamp_compressor;
    TampConf tamp_conf = {
        .window = WINDOW_BITS,
        .literal = LITERAL_BITS,
        .use_custom_dictionary = false};
    tamp_compressor_init(&tamp_compressor, &tamp_conf, window_buffer);

    // 3. Compress data
    uint32_t original_data_size = (uint32_t)input_file_size;
    // Max compressed size can be slightly larger than original, use a safe upper bound
    uint32_t compressed_buffer_max_size = original_data_size + (original_data_size / 8) + 100; // A common heuristic for max compressed size

    uint8_t *compressed_buffer = (uint8_t *)malloc(compressed_buffer_max_size);
    if (!compressed_buffer)
    {
        perror("Error allocating memory for compressed buffer");
        free(input_buffer);
        return 1;
    }

    size_t output_written_size;
    size_t input_consumed_size;

    tamp_res res = tamp_compressor_compress_and_flush(
        &tamp_compressor,
        compressed_buffer, compressed_buffer_max_size, &output_written_size,
        input_buffer, original_data_size, &input_consumed_size,
        true); // Flush at the end

    if (res != TAMP_OK)
    {
        fprintf(stderr, "Tamp compression failed with error: %d\n", res);
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    if (input_consumed_size != original_data_size)
    {
        fprintf(stderr, "Error: Not all input data was consumed by the compressor. Consumed: %zu, Original: %u\n", input_consumed_size, original_data_size);
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    // 4. Write output file
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file)
    {
        perror("Error opening output file");
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    // Write original size
    if (fwrite(&original_data_size, sizeof(uint32_t), 1, output_file) != 1)
    {
        perror("Error writing original size to output file");
        fclose(output_file);
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    // Write compressed size
    uint32_t output_written_size_u32 = (uint32_t)output_written_size;
    if (fwrite(&output_written_size_u32, sizeof(uint32_t), 1, output_file) != 1)
    {
        perror("Error writing compressed size to output file");
        fclose(output_file);
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    // Write compressed data
    if (fwrite(compressed_buffer, 1, output_written_size, output_file) != output_written_size)
    {
        perror("Error writing compressed data to output file");
        fclose(output_file);
        free(input_buffer);
        free(compressed_buffer);
        return 1;
    }

    fclose(output_file);

    printf("File compressed successfully: %s -> %s (Original: %u bytes, Compressed: %u bytes)\n", input_filename, output_filename, original_data_size, output_written_size_u32);

    // Cleanup
    free(input_buffer);
    free(compressed_buffer);

    return 0;
}
