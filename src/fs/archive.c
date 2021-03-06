#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <zlib.h>
#include <assert.h>
#include "archive.h"
#include "../logger.h"
#include "../utils/aes.h"

// Avoid corruption on win systems.
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

// 256k
#define ZLIB_CHUNK 256 * 1024

// Change in prod?
#define RANDOM_POS(file_len) ((file_len + (4 - 1)) / 4) * 3
static uint8_t ENCRYPTION_KEY[] = {
    0xD3, 0xA5, 0x2E, 0x61, 0x16, 0x29, 0x6E, 0xB4, 0x33, 0xF0, 0x9E, 0xBF, 0x6D, 0xCD, 0xD3, 0x92,
    0xFC, 0xC3, 0x53, 0x7E, 0x83, 0x70, 0xE1, 0x7C, 0xE2, 0x41, 0xAC, 0x39, 0x93, 0x43, 0x20, 0x6D 
};

void encryptFile(char *path) {
    FILE *file = NULL;
    uint8_t *in_bytes = NULL;
    uint8_t encryption_key[AES_KEYLEN];
    uint8_t encryption_iv[16];
    uint8_t random_bytes[0x64 + AES_KEYLEN];
    uint64_t file_length;
    char header[9];
    int i;

    // Use bytes from binary as encryption key
    file = fopen("vnengine", "rb");
    if (file == NULL) {
        log_Debug("%s: Failed to open file", __FUNCTION__);
        return;
    }

    // Get file length
    fseek(file, 0L, SEEK_END);
    file_length = ftell(file);
    
    // Seek to random part and read bytes
    fseek(file, RANDOM_POS(file_length), SEEK_SET);
    fread(&random_bytes, 1, 0x64 + AES_KEYLEN, file);
    fclose(file);
    log_Debug("Offset %d", RANDOM_POS(file_length));

    log_DebugHex("Original Encryption Key", ENCRYPTION_KEY, AES_KEYLEN);

    // Create key
    uint8_t c;
    for (i = 0; i < AES_KEYLEN; i++) {
        c = ENCRYPTION_KEY[i];
        if ((i % 2) == 0) {
            c = random_bytes[i / 2] ^ ENCRYPTION_KEY[i];
        }
        encryption_key[i] = c;
    }
    log_DebugHex("Encryption Key", encryption_key, AES_KEYLEN);

    // Create iv
    for (i = 0; i < 16; i++) {
        encryption_iv[i] = random_bytes[AES_KEYLEN + i];
    }
    log_DebugHex("Encryption IV", encryption_iv, 16);

    // Read file to encrypt
    file = fopen(path, "rb");
    if (file == NULL) {
        log_Error("Failed to open file");
        return;
    }

    // Get file length
    fseek(file, 0L, SEEK_END);
    file_length = ftell(file);
    
    // Go back
    fseek(file, 0, SEEK_SET);
    
    // Read header, subtract length not encrypting
    fread(&header, 1, 9, file);
    file_length -= 9;

    // Pad
    for (i = 0; i < 32; i++) {
        if ((file_length % 16) == 0) {
            break;
        }
        file_length += 1;
    }
    log_Debug("File length: %ld", file_length);

    // Read bytes
    in_bytes = malloc(file_length);
    fread(in_bytes, 1, file_length, file);
    fclose(file);

    // Encrypt
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, encryption_key, encryption_iv);
    AES_CBC_encrypt_buffer(&ctx, in_bytes, file_length);

    // Write to file
    file = fopen(path, "wb");

    // Write header
    fwrite(&header, 9, 1, file);

    // Write data
    fwrite(in_bytes, file_length, 1, file);

    fclose(file);
    free(in_bytes);
}

void readEncrypted(uint8_t *buffer, uint64_t length) {
    uint8_t encryption_key[AES_KEYLEN];
    uint8_t encryption_iv[16];
    uint8_t random_bytes[0x64 + AES_KEYLEN];
    uint64_t file_length = 0;
    FILE* file = NULL;
    int i;

    // Get random bytes
    file = fopen("vnengine", "rb");
    if (file == NULL) {
        log_Debug("%s: Failed to open file", __FUNCTION__);
        return;
    }

    // Get file length
    fseek(file, 0L, SEEK_END);
    file_length = ftell(file);
    
    // Seek to random part and read bytes
    fseek(file, RANDOM_POS(file_length), SEEK_SET);
    fread(&random_bytes, 1, 0x64 + AES_KEYLEN, file);
    fclose(file);
    log_Debug("Offset %d", RANDOM_POS(file_length));
    log_Debug("Decryption length: %ld", length);

    // Create key
    uint8_t c;
    for (i = 0; i < AES_KEYLEN; i++) {
        c = ENCRYPTION_KEY[i];
        if ((i % 2) == 0) {
            c = random_bytes[i / 2] ^ ENCRYPTION_KEY[i];
        }
        encryption_key[i] = c;
    }
    log_DebugHex("Decryption Key", encryption_key, AES_KEYLEN);

    // Create iv
    for (i = 0; i < 16; i++) {
        encryption_iv[i] = random_bytes[AES_KEYLEN + i];
    }
    log_DebugHex("Decryption IV", encryption_iv, 16);

    // Decrypt
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, encryption_key, encryption_iv);
    AES_CBC_decrypt_buffer(&ctx, buffer, length);
}

uint64_t _compressData(FILE *file, int level, int bufferSize, char **buffer) {
    unsigned char chunk_in[ZLIB_CHUNK];
    unsigned char chunk_out[ZLIB_CHUNK];
    int ret, flush;
    z_stream strm;
    uint64_t size = 0;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK) {
        log_Error("Failed to deflateInit");
        return size;
    }
    
    // Compress until EOF
    do {
        strm.avail_in = fread(chunk_in, 1, ZLIB_CHUNK, file);
        if (ferror(file)) {
            (void)deflateEnd(&strm);
            log_Error("%s: File stream error", __FUNCTION__);
            return size;
        }
        flush = feof(file) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = chunk_in;
        // run deflate() on input until output buffer not full, finish
        // compression if all of source has been read in
        do {
            strm.avail_out = ZLIB_CHUNK;
            strm.next_out = chunk_out;
            ret = deflate(&strm, flush);
            assert(ret != Z_STREAM_ERROR);
            if (bufferSize != 0) {
                log_Debug("%s: Writing %d bytes to buffer", __FUNCTION__, ZLIB_CHUNK - strm.avail_out);
                memcpy(*buffer + size, chunk_out, ZLIB_CHUNK - strm.avail_out);
            }
            size += ZLIB_CHUNK - strm.avail_out;    
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);
    (void)deflateEnd(&strm);
    log_Debug("Compressed size: %ld", size);
    return size;
}

uint64_t readCompressed(FILE *file, int bufferSize, uint8_t **buffer) {
    int ret;
    z_stream strm;
    unsigned char chunk_in[ZLIB_CHUNK];
    unsigned char chunk_out[ZLIB_CHUNK];
    uint64_t size = 0;
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        log_Error("Failed to inflateInit");
        return size;
    }

    // Decompress until eof
    do {
        strm.avail_in = fread(chunk_in, 1, ZLIB_CHUNK, file);
        if (ferror(file)) {
            log_Error("File stream error");
            (void)inflateEnd(&strm);
            return size;
        }
        if (strm.avail_in == 0) {
            break;
        }
        strm.next_in = chunk_in;
        // run inflate() on input until output buffer not full
        do {
            strm.avail_out = ZLIB_CHUNK;
            strm.next_out = chunk_out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR); 
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return size;
            }
            if (bufferSize != 0) {
                log_Debug("%s: Writing %d bytes to buffer", __FUNCTION__, ZLIB_CHUNK - strm.avail_out);
                memcpy(*buffer + size, chunk_out, ZLIB_CHUNK - strm.avail_out);
            }
            size += ZLIB_CHUNK - strm.avail_out;
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);
    (void)inflateEnd(&strm);
    log_Debug("Uncompressed size: %ld", size);
    return size;
}

void compressFile(char *path) {
    FILE *file = NULL;
    uint64_t compSize, outCheck;
    char *compressed = NULL;
    char header[9];

    // Open file
    file = fopen(path, "rb");
    if (file == NULL) {
        log_Error("%s: Failed to open file", __FUNCTION__);
        return;
    }

    // Copy header and calculate the compression of the contents
    fread(&header, 1, 9, file);
    compSize = _compressData(file, Z_BEST_COMPRESSION, 0, NULL);

    // Seek back to beginning, after header
    fseek(file, 9, SEEK_SET);

    // Create buffer based off compression size and recompress data again
    compressed = malloc(compSize);
    outCheck = _compressData(file, Z_BEST_COMPRESSION, compSize, &compressed);
    log_Debug("Out size %ld", outCheck, compSize);
    assert(compSize == outCheck);
    log_Debug("Compressed Header: 0x%x 0x%x", (unsigned)(unsigned char)compressed[0], (unsigned)(unsigned char)compressed[1]);

    // Close file
    fclose(file);

    // Open File
    file = fopen(path, "wb");

    // Write header
    fwrite(&header, 9, 1, file);

    // Write contents
    fwrite(compressed, compSize, 1, file);

    fclose(file);
}

Archive *archive_Read(char *archivePath) {
    FILE *file = NULL;
    Archive *archive = NULL;
    uint64_t file_size;
    ArchiveFlags flags;
    char header[5];
    uint8_t *raw_data = NULL;

    // Open file
    file = fopen(archivePath, "rb");
    if (file == NULL) {
        log_Error("Failed to open archive path for reading");
        return NULL;
    }

    // Read file size
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    log_Debug("Archive Size: %ld", file_size);
    if (file_size < 12) {
        log_Error("Archive too small");
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    // Check header
    fread(&header, 5, 1, file);
    log_DebugHex("File Header", header, 5);
    if ((strncmp(header, "baka\0", 5)) != 0) {
        log_Error("Invalid archive header");
        fclose(file);
        return NULL;
    }

    // Check flags
    fread(&flags, sizeof(flags), 1, file);
    switch (flags) { 
        case compressed:
            log_Debug("Reading compressed archive");
            file_size = readCompressed(file, 0, NULL);
            raw_data = malloc(file_size);
            fseek(file, 9, SEEK_SET);
            readCompressed(file, file_size, &raw_data);
            break;
        case encrypted:
            log_Debug("Reading encrypted archive");
            // First read into buffer
            file_size -= 9;
            raw_data = malloc(file_size);
            fseek(file, 9, SEEK_SET);
            fread(raw_data, file_size, 1, file);
            // Then pass in to decrypt buffer
            readEncrypted(raw_data, file_size);
            break;
        default:
            log_Debug("Reading archive");
            file_size -= 9;
            raw_data = malloc(file_size);
            fseek(file, 9, SEEK_SET);
            fread(raw_data, file_size, 1, file);
            break;
    }

    // Close file
    fclose(file);

    // Copy file count to archive struct
    archive = malloc(sizeof(Archive));
    memcpy(&archive->file_count, raw_data, sizeof(archive->file_count));
    log_Debug("File count: %ld", archive->file_count);

    // Create files
    archive->files = malloc(sizeof(ArchiveFile) * archive->file_count);

    uint64_t faddr;
    uint64_t i;
    for (i = 0; i < archive->file_count; i++) {
        // Create file
        archive->files[i] = malloc(sizeof(ArchiveFile));
        // Get file size
        memcpy(&archive->files[i]->size, raw_data + sizeof(uint64_t) + (sizeof(uint64_t) * archive->file_count) + (i * sizeof(uint64_t)), sizeof(uint64_t));
        // Get file address and read contents
        archive->files[i]->contents = malloc(archive->files[i]->size);
        memcpy(&faddr, raw_data + (sizeof(uint64_t) * (i + 1)), sizeof(faddr));
        memcpy(archive->files[i]->contents, raw_data + faddr - 9, archive->files[i]->size);
        // Write path
        memcpy(archive->files[i]->path, raw_data + sizeof(uint64_t) + ((sizeof(uint64_t) * archive->file_count) * 2) + (ARCHIVE_MAX_PATH_LENGTH * i), ARCHIVE_MAX_PATH_LENGTH);
        //log_DebugHex("File", archive->files[i], sizeof(ArchiveFile));
        //log_DebugHex("File Contents", archive->files[i]->contents, archive->files[i]->size);
    }

    free(raw_data);

    return archive;
}

void archive_Create(char *path, uint64_t fileCount, ArchiveFile **files, ArchiveFlags flags) {
    FILE *file = NULL;

    // Open file
    file = fopen(path, "wb");
    if (file) {
        // Write magic
        fwrite(ARCHIVE_MAGIC, sizeof(ARCHIVE_MAGIC), 1, file);

        // Write flags
        fwrite(&flags, sizeof(flags), 1, file);

        // Write file_count
        fwrite(&fileCount, sizeof(fileCount), 1, file);

        // Write ptrs
        int i;
        uint64_t fileTablePtr;
        uint64_t headerSize =  sizeof(ARCHIVE_MAGIC) + sizeof(flags) + sizeof(fileCount);
        uint64_t ptr = headerSize + ((fileCount * sizeof(uint64_t)) * 2) + (ARCHIVE_MAX_PATH_LENGTH * fileCount);
        for (i = 0; i < fileCount; i++) {
            // FILE TABLE
            // Write down the address of the file in the file table
            fwrite(&ptr, sizeof(uint64_t), 1, file);
            // Save position of current table addr
            fileTablePtr = ftell(file);

            // SIZE TABLE
            fseek(file, headerSize + (sizeof(uint64_t) * fileCount) + (i * sizeof(uint64_t)), SEEK_SET);
            fwrite(&files[i]->size, sizeof(uint64_t), 1, file);

            // PATH TABLE
            fseek(file, headerSize + ((sizeof(uint64_t) * fileCount) * 2) + (i * ARCHIVE_MAX_PATH_LENGTH), SEEK_SET);
            fwrite(&files[i]->path, sizeof(files[i]->path), 1, file);

            // FILE SPACE
            // Write file at addr
            fseek(file, ptr, SEEK_SET);
            fwrite(files[i]->contents, files[i]->size, 1, file);
            // Increase ptr new location by file size
            ptr += files[i] -> size;

            // Go back to ptr table
            fseek(file, fileTablePtr, SEEK_SET);
        }

        fclose(file);
    } else {
        log_Error("%s: Failed to open file", __FUNCTION__);
        fclose(file);
        return;
    }

    // Read flags
    // Check if compression needed else if compression needed
    switch (flags)
    {
        case compressed:
            compressFile(path);
            break;
        
        case encrypted:
            encryptFile(path);
            break;

        default:
            break;
    }
}

void archive_Free(Archive *archive) {
    if (archive == NULL) {
        return;
    }
    uint64_t i;
    for (i = 0; i < archive->file_count; i++) {
        free(archive->files[i]->contents);
        free(archive->files[i]);
    }
    free(archive->files);
    free(archive);
    log_Debug("Archive freed");
}
