#include "littlefs.h"

#define STR_ERROR "Failed to setup fs"

/*
// Just use default value
bool fsIsConfig(String conf, String dir)
{
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return false;
    }
    File file = LittleFS.open(dir + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return false;
    }
    file.close();
    return true;
}
*/

String fsGetString(String conf, String defaultValue, String dir)
{
    bufSize blob = fsGetBlob(conf, dir);
    if (blob.buf == nullptr)
    {
        debugLog("Failed to get blob for conf: " + conf + ", setting default value.");
        fsSetString(conf, defaultValue, dir);
        return defaultValue;
    }

    // Ensure null termination for String conversion
    char *str_buf = (char *)malloc(blob.size + 1);
    if (!str_buf)
    {
        debugLog("Failed to allocate memory for string buffer.");
        free(blob.buf);
        return defaultValue;
    }
    memcpy(str_buf, blob.buf, blob.size);
    str_buf[blob.size] = '\0'; // Null-terminate the string

    String str = String(str_buf);
    free(blob.buf); // Free the buffer allocated by fsGetBlob
    free(str_buf);  // Free the temporary string buffer
    return str;
}

bool fsSetString(String conf, String value, String dir)
{
    // Convert String to uint8_t* and its size
    int size = value.length();
    uint8_t *buf = (uint8_t *)malloc(size);
    if (!buf)
    {
        debugLog("Failed to allocate memory for string conversion.");
        return false;
    }
    memcpy(buf, value.c_str(), size);

    bool success = fsSetBlob(conf, buf, size, dir);
    free(buf);
    return success;
}

static std::mutex tamp_mutex;
#define WINDOW_BITS 10
uint8_t window_buffer[1 << WINDOW_BITS];
static TampDecompressor tamp_decompressor;

static void initTampDecompressor()
{
    tamp_decompressor_init(&tamp_decompressor, NULL, window_buffer);
}

static TampCompressor tamp_compressor;
static void initTampCompressor()
{
    TampConf tamp_conf = {
        .window = WINDOW_BITS,
        .literal = 8,
        .use_custom_dictionary = false};
    tamp_compressor_init(&tamp_compressor, &tamp_conf, window_buffer);
}

bufSize fsGetBlob(String conf, String dir)
{
    std::lock_guard<std::mutex> lock(tamp_mutex);
    memset(window_buffer, 0, sizeof(window_buffer));
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return emptyBuff;
    }
    File file = LittleFS.open(dir + conf);
    if (file == false)
    {
        debugLog("There is no conf file: " + conf);
        return emptyBuff;
    }

    int fileSize = file.size();
    uint8_t *file_content_buffer = (uint8_t *)malloc(fileSize);
    if (!file_content_buffer)
    {
        debugLog("Failed to allocate memory for file content buffer");
        file.close();
        return emptyBuff;
    }

    if (file.read(file_content_buffer, fileSize) == 0)
    {
        debugLog("Failed to read the entire file: " + conf);
        file.close();
        free(file_content_buffer);
        return emptyBuff;
    }
    file.close();

    uint32_t original_size;
    uint32_t compressed_size;
    uint32_t offset = 0;

    if (fileSize < sizeof(uint32_t))
    {
        debugLog("File too small to contain original_size: " + conf);
        free(file_content_buffer);
        return emptyBuff;
    }
    memcpy(&original_size, file_content_buffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    // debugLog("fsGetBlob: Read original_size from header: " + String(original_size));

    if (fileSize < offset + sizeof(uint32_t))
    {
        debugLog("File too small to contain compressed_size: " + conf);
        free(file_content_buffer);
        return emptyBuff;
    }
    memcpy(&compressed_size, file_content_buffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    // debugLog("fsGetBlob: Read compressed_size from header: " + String(compressed_size));

    if (fileSize < offset + compressed_size)
    {
        debugLog("File content size mismatch with compressed_size: " + conf);
        free(file_content_buffer);
        return emptyBuff;
    }

    uint8_t *compressed_data = file_content_buffer + offset;

    uint8_t *decompressed_buffer = (uint8_t *)malloc(original_size);
    if (!decompressed_buffer)
    {
        debugLog("Failed to allocate memory for decompressed buffer");
        free(file_content_buffer);
        return emptyBuff;
    }

    initTampDecompressor();
    size_t output_written_size;
    size_t input_consumed_size;

#if DEBUG
    uint64_t start_time = millis();
#endif

    tamp_res res = tamp_decompressor_decompress(
        &tamp_decompressor,
        decompressed_buffer, original_size, &output_written_size,
        compressed_data, compressed_size, &input_consumed_size);

#if DEBUG
    uint64_t end_time = millis();
    uint64_t decompression_time_ms = end_time - start_time;
#endif

    free(file_content_buffer);

    if (res != TAMP_OK && res != TAMP_INPUT_EXHAUSTED && res != TAMP_OUTPUT_FULL)
    {
        debugLog("Tamp decompression failed with error: " + String(res));
        free(decompressed_buffer);
        return emptyBuff;
    }

    if (output_written_size != original_size)
    {
        debugLog("Decompressed size mismatch. Expected: " + String(original_size) + ", Got: " + String(output_written_size));
        free(decompressed_buffer);
        return emptyBuff;
    }
#if DEBUG
    debugLog("fsGetBlob: Decompression Time: " + String(decompression_time_ms) + "ms");
#endif

    bufSize retBuf = {
        decompressed_buffer, (int)output_written_size};
    return retBuf;
}

bool fsSetBlob(String conf, uint8_t *value, int size, String dir)
{
    std::lock_guard<std::mutex> lock(tamp_mutex);
    memset(window_buffer, 0, sizeof(window_buffer));
    if (fsSetup() == false)
    {
        debugLog("Failed to setup fs");
        return false;
    }

    uint32_t compressed_buffer_max_size = size * 2;
    uint8_t *compressed_buffer = (uint8_t *)malloc(compressed_buffer_max_size);
    if (!compressed_buffer)
    {
        debugLog("Failed to allocate memory for compressed buffer");
        return false;
    }

    initTampCompressor();

    size_t output_written_size;
    size_t input_consumed_size;

#if DEBUG
    uint64_t start_time = millis();
#endif

    tamp_res res = tamp_compressor_compress_and_flush(
        &tamp_compressor,
        compressed_buffer, compressed_buffer_max_size, &output_written_size,
        value, size, &input_consumed_size,
        true);

#if DEBUG
    uint64_t end_time = millis();
    uint64_t compression_time_ms = end_time - start_time;
#endif

    if (res != TAMP_OK)
    {
        debugLog("Tamp compression failed with error: " + String(res));
        free(compressed_buffer);
        return false;
    }

    uint32_t original_data_size = size;
#if DEBUG
    int compression_percentage = 0;
    if (original_data_size > 0)
    {
        compression_percentage = (100 * (original_data_size - output_written_size)) / original_data_size;
    }

    debugLog("fsSetBlob: Original size: " + String(original_data_size) + ", Compressed size: " + String(output_written_size) +
             ", Compression: " + String(compression_percentage) + "%, Time: " + String(compression_time_ms) + "ms");
#endif

    File file = LittleFS.open(dir + conf, FILE_WRITE, true);
    if (file == false)
    {
        debugLog("Failed to set conf: " + conf);
        free(compressed_buffer);
        return false;
    }
    if (file.write((uint8_t *)&original_data_size, sizeof(uint32_t)) == 0)
    {
        debugLog("Failed to write original size to file " + conf);
        file.close();
        free(compressed_buffer);
        return false;
    }
    uint32_t output_written_size_u32 = (uint32_t)output_written_size;
    if (file.write((uint8_t *)&output_written_size_u32, sizeof(uint32_t)) == 0)
    {
        debugLog("Failed to write compressed size to file " + conf);
        file.close();
        free(compressed_buffer);
        return false;
    }
    if (file.write(compressed_buffer, output_written_size) == 0)
    {
        debugLog("Failed to write compressed blob to file " + conf);
        file.close();
        free(compressed_buffer);
        return false;
    }
    file.close();
    free(compressed_buffer);
    return true;
}

/*
#if DEBUG
  debugLog("Starting Tamp compression/decompression test...");

  const char* test_data_str = "This is a test string for Tamp compression. It should be compressed and then decompressed correctly.";
  uint8_t* original_data = (uint8_t*)test_data_str;
  int original_size = strlen(test_data_str) + 1; // +1 for null terminator

  String test_blob_name = "/test_blob.tamp";
  String test_dir = "/conf/";

  // Set blob (compress and write)
  if (fsSetBlob(test_blob_name, original_data, original_size, test_dir)) {
    debugLog("fsSetBlob successful for " + test_blob_name);

    // Get blob (read and decompress)
    bufSize retrieved_blob = fsGetBlob(test_blob_name, test_dir);

    if (retrieved_blob.buf != nullptr && retrieved_blob.size > 0) {
      debugLog("fsGetBlob successful for " + test_blob_name + ", decompressed size: " + String(retrieved_blob.size));

      // Compare original and decompressed data
      if (retrieved_blob.size == original_size && memcmp(original_data, retrieved_blob.buf, original_size) == 0) {
        debugLog("Tamp test PASSED: Original and decompressed data match!");
      } else {
        debugLog("Tamp test FAILED: Original and decompressed data DO NOT match!");
        debugLog("Original: " + String((char*)original_data));
        debugLog("Decompressed: " + String((char*)retrieved_blob.buf));
      }
      free(retrieved_blob.buf); // Free the allocated buffer
    } else {
      debugLog("Tamp test FAILED: fsGetBlob returned empty buffer.");
    }
  } else {
    debugLog("Tamp test FAILED: fsSetBlob failed.");
  }
  debugLog("Tamp compression/decompression test finished.");

  debugLog("Starting Python-generated TAMP compression test...");

  const char* python_test_data_str = "This is a test string for Python-generated TAMP compression. It should be compressed by the Python script and then decompressed correctly by the C++ code.";
  uint8_t* python_original_data = (uint8_t*)python_test_data_str;
  int python_original_size = strlen(python_test_data_str) + 1; // +1 for null terminator

  String python_test_blob_name = "/test_python_compression.txt";
  String python_test_dir = "/"; // Assuming it's in the root of littlefs

  // Get blob (read and decompress)
  bufSize python_retrieved_blob = fsGetBlob(python_test_blob_name, python_test_dir);

  if (python_retrieved_blob.buf != nullptr && python_retrieved_blob.size > 0) {
    debugLog("fsGetBlob successful for Python-generated " + python_test_blob_name + ", decompressed size: " + String(python_retrieved_blob.size));

    // Compare original and decompressed data
    if (python_retrieved_blob.size == python_original_size && memcmp(python_original_data, python_retrieved_blob.buf, python_original_size) == 0) {
      debugLog("Python-generated TAMP test PASSED: Original and decompressed data match!");
    } else {
      debugLog("Python-generated TAMP test FAILED: Original and decompressed data DO NOT match!");
      debugLog("Original: " + String((char*)python_original_data));
      debugLog("Decompressed: " + String((char*)python_retrieved_blob.buf));
    }
    free(python_retrieved_blob.buf); // Free the allocated buffer
  } else {
    debugLog("Python-generated TAMP test FAILED: fsGetBlob returned empty buffer or failed.");
  }
  debugLog("Python-generated TAMP compression test finished.");

  delayTask(9999999);
#endif
*/
