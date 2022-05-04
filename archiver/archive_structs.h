#ifndef __ARCHIVE_STRUCTS_H
#define __ARCHIVE_STRUCTS_H
typedef struct FileMemoryDefinition
{
    unsigned int crc32 = 0;    // crc32 of uncompressed file
    size_t offset = 0;         // offset in data struct
    size_t compressedSize = 0; // byte size of block
    size_t originSize = 0;     // origin size of uncompressed file
    char relative_path[256];   // relative path
} FileMemoryDefinition_t;

typedef struct FileArchiveDefinition
{
    size_t SizeOfFMMArray = 0; // size of FileMemoryDefinition array
    size_t SizeOfData = 0;     // size of data struct
} FileArchiveDefinition_t;
#endif