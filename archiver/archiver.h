#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <filesystem>
#include <exception>
#include <memory>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

#include "../external_libs/lib_lz4/lz4.h"
#include "archive_structs.h"
#include "../shared/liblog.h"
#include "../external_libs/lib_lz4/lz4frame.h"

namespace archiver
{
    class archiver
    {
    public:
        struct CompressedDataDefinition
        {
            size_t CompressedSize = 0;
            size_t OriginSize = 0;
            std::unique_ptr<char[]> data;
            unsigned int crc32;
        };
        /**
         * @brief compress file at specified path
         * @param f_name path to file
         */
        static CompressedDataDefinition compress(const std::filesystem::path &f_name);
        /**
         * @brief decompress data
         *
         * @param src ptr to data
         * @param compressedSize byte size of compressed block
         * @param originSize origin size of data
         * @return std::unique_ptr<char[]> unique_ptr of decompressed data
         */
        static std::unique_ptr<char[]> decompress(const char *src, size_t compressedSize, size_t originSize);

        /**
         * @brief calculate crc32
         * src: https://web.archive.org/web/20190108202303/http://www.hackersdelight.org/hdcodetxt/crc.c.txt
         * @param src ptr to data block
         * @param size size of block
         * @return unsigned int crc32
         */
        static unsigned int crc32b(const char *src, size_t size);

        /**
         * @brief calculate crc32 of a file
         * src: https://web.archive.org/web/20190108202303/http://www.hackersdelight.org/hdcodetxt/crc.c.txt
         * @param path path to the file
         */
        static unsigned int crc32b(const std::filesystem::path& path);

        /**
         * @brief Compress passed chunk in inBuffer and write to outBuffer
         * 
         * @param ctx valid LZ4F_compressionContext_t
         * @param inBuffer src buffer
         * @param outBuffer dest buffer
         * @param inSize bytes size of chunk in inBuffer
         * @param outCapacity bytes capacity of outBuffer
         * @return size_t writted size
         */
        static size_t CompressChunk(LZ4F_compressionContext_t ctx, char* inBuffer,
                                            char* outBuffer, size_t inSize, size_t outCapacity);
    };

    class ArchiveBuilder
    {
    public:
        ArchiveBuilder();
        ~ArchiveBuilder();

        struct FileReplacmentConfig
        {
            std::filesystem::path OnCreatorPath;
            std::string RelativeUserPath;
        };

        void AddFiles(const std::vector<FileReplacmentConfig>& config);

    private:
        void AddFile(const FileReplacmentConfig &conf);

        archiver::CompressedDataDefinition CompressFileStream(const std::filesystem::path& path);
        const size_t MAX_CHUNK_SIZE = 104857600ULL; //100 MB
        const size_t MIN_CHUNK_SIZE = 1024ULL; //1KB
        /**
         * @brief Calculate chunk size based on file size
         * set tenth part of file size or MIN_CHUNK_SIZE if it less than it
         * if part way to big sets MAX_CHUNK_SIZE
         * @param file_size 
         * @return size_t 
         */
        size_t CalculateFileChunk(size_t file_size);

        class ArchiveWriter
        {
        public:
            enum class Sections {
                Data, FileControl, ArchiveControl
            };

            ArchiveWriter(std::ofstream& _archive_file) : archive_file{_archive_file} {};
            ~ArchiveWriter();

            void AddSection(Sections sec);
            void EndSection();
            void WriteToSection(char* ptr, size_t size);
            

            
        private:
            const char *DataSection = "const char data[] = {";
            const char *FileControlSection = "const char control[] = {";
            const char *ArchiveControlSection = "const char archive[] = {";
            const char *SectionEnd = "};\n";

            bool new_section = false;
            bool append_comma = false;

            std::ofstream& archive_file;
            size_t WritedByteInSection = 0;

            void WriteServiceInfo(const char* ptr);
            void WriteByte(char* ptr);
        };

        std::ofstream archive_file;
        std::unique_ptr<ArchiveWriter> writer;
        std::vector<FileMemoryDefinition_t> files;
        FileArchiveDefinition_t files_def;
    };
} // archiver

#endif