#include "archiver.h"

archiver::archiver::CompressedDataDefinition
archiver::archiver::compress(const std::filesystem::path &f_name)
{
    namespace fs = std::filesystem;

    // check whether file exists or not
    if (!fs::exists(f_name))
    {
        throw std::invalid_argument(
            "File " +
            f_name.generic_string() +
            " does not exist.");
    }
    // file size in bytes
    auto f_size{fs::file_size(f_name)};

    auto f_content = std::make_unique<char[]>(f_size);

    std::ifstream stream{f_name};
    if (!stream.is_open())
    {
        throw std::runtime_error(
            "Could not open stream to read "
            "file " +
            f_name.generic_string() +
            ".");
    }

    stream.read(f_content.get(), f_size);

    stream.close(); // close stream

    size_t max_dist_size = LZ4_compressBound(f_size); // the maximum possible theoretical size after compression

    auto compressed_data = std::make_unique<char[]>(max_dist_size);

    // assume we are good with memory

    // now we compress and return the final size
    int comp_size = LZ4_compress_default(f_content.get(), compressed_data.get(), f_size, max_dist_size);
    if (comp_size < 0)
    {
        throw std::runtime_error(
            "Could not compress file "
            "file " +
            f_name.generic_string() +
            ".");
    }

    archiver::archiver::CompressedDataDefinition out;
    out.CompressedSize = comp_size;
    out.OriginSize = f_size;
    out.data = std::move(compressed_data);
    out.crc32 = crc32b(f_content.get(), f_size);

    return out;
}

std::unique_ptr<char[]>
archiver::archiver::decompress(const char *src, size_t compressedSize, size_t originSize)
{
    auto buffer = std::make_unique<char[]>(originSize);

    if (LZ4_decompress_safe(src, buffer.get(), compressedSize, originSize) != originSize)
    {
        throw std::runtime_error("unable to decompress data. decompressed size doesnt equal origin size");
    }

    return buffer;
}

unsigned int
archiver::archiver::crc32b(const char *src, size_t size)
{
    int i, j;
    unsigned int crc, mask;
    char byte;

    i = 0;
    crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; i++)
    {
        byte = src[i]; // Get next byte.
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--)
        { // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}

unsigned int
archiver::archiver::crc32b(const std::filesystem::path &path)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream.is_open())
    {
        throw std::runtime_error("Fail. Unable to open stream");
    }

    int j;
    unsigned int crc, mask;
    char byte;

    crc = 0xFFFFFFFF;
    stream >> std::noskipws; //< this is to read whitespaces as well
    while (stream >> byte)
    {
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--)
        { // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    stream.close();
    return ~crc;
}

archiver::ArchiveBuilder::ArchiveBuilder()
{
    archive_file = std::ofstream("./patcher/archive.h");
    writer = std::make_unique<ArchiveWriter>(archive_file);
}

archiver::ArchiveBuilder::~ArchiveBuilder()
{
    writer.reset();
    archive_file.close();
}

void archiver::ArchiveBuilder::AddFile(const FileReplacmentConfig &conf)
{
    try
    {
        auto compressed = CompressFileStream(conf.OnCreatorPath);

        FileMemoryDefinition_t def;
        def.compressedSize = compressed.CompressedSize;
        def.offset = files_def.SizeOfData;
        def.originSize = compressed.OriginSize;
        def.crc32 = compressed.crc32;
        // set buffer with 0 and write data
        memset(def.relative_path, 0, sizeof(def.relative_path));
        memcpy(def.relative_path, conf.RelativeUserPath.c_str(), conf.RelativeUserPath.size());

        //writer->WriteToSection(compressed.data.get(), compressed.CompressedSize);

        files.push_back(std::move(def));

        files_def.SizeOfFMMArray++;
        files_def.SizeOfData += compressed.CompressedSize;
    }
    catch (std::exception err)
    {
        std::string msg = "Add file error. Error: ";
        msg += +err.what();
        LibLog::LogEngine::LogConsoleError(msg);
    }
}

void archiver::ArchiveBuilder::ArchiveWriter::AddSection(ArchiveBuilder::ArchiveWriter::Sections sec)
{
    if (new_section)
    {
        WriteServiceInfo(SectionEnd);
    }
    new_section = true;
    append_comma = false;

    switch (sec)
    {
    case ArchiveBuilder::ArchiveWriter::Sections::Data:
        WriteServiceInfo(DataSection);
        break;
    case ArchiveBuilder::ArchiveWriter::Sections::ArchiveControl:
        WriteServiceInfo(ArchiveControlSection);
        break;
    case ArchiveBuilder::ArchiveWriter::Sections::FileControl:
        WriteServiceInfo(FileControlSection);
        break;
    default:
        LOG_CONSOLE_DEBUG("unrecognised enum was passed to AddSection");
        break;
    }
    WritedByteInSection = 0;
}

void archiver::ArchiveBuilder::ArchiveWriter::WriteServiceInfo(const char *ptr)
{
    size_t size = strlen(ptr);
    archive_file.write(ptr, size);
}

void archiver::ArchiveBuilder::ArchiveWriter::WriteToSection(char *ptr, size_t size)
{
    if(size == 0ULL)
        return;

    if (append_comma)
    {
        archive_file << ", ";
    }
    append_comma = true;

    archive_file << "0x" << std::hex << (0xff & *ptr);
    ptr++;

    for (size_t i = 1; i < size; i++)
    {
        WriteByte(ptr);
        ptr++;
    }

    WritedByteInSection += size;
}

void archiver::ArchiveBuilder::ArchiveWriter::WriteByte(char *ptr)
{
    archive_file << ", 0x" << std::hex << (0xff & *ptr);
}

bool IsDestruct = false;
archiver::ArchiveBuilder::ArchiveWriter::~ArchiveWriter()
{
    EndSection();
}

void archiver::ArchiveBuilder::AddFiles(const std::vector<ArchiveBuilder::FileReplacmentConfig> &config)
{
    writer->AddSection(ArchiveBuilder::ArchiveWriter::Sections::Data);
    for (const auto &cnf : config)
    {
        AddFile(cnf);
    }

    writer->AddSection(ArchiveBuilder::ArchiveWriter::Sections::FileControl);
    writer->WriteToSection(reinterpret_cast<char *>(files.data()), sizeof(FileMemoryDefinition_t) * files.size());

    writer->AddSection(ArchiveBuilder::ArchiveWriter::Sections::ArchiveControl);
    writer->WriteToSection(reinterpret_cast<char *>(&files_def), sizeof(FileArchiveDefinition_t));
    writer->EndSection();

    archive_file.close();
}

void archiver::ArchiveBuilder::ArchiveWriter::EndSection() {
    if (new_section)
    {
        WriteServiceInfo(SectionEnd);
    }
}

size_t archiver::archiver::CompressChunk(LZ4F_compressionContext_t ctx, char *inBuffer,
                                                char *outBuffer, size_t inSize, size_t outCapacity) {
    size_t compressed = LZ4F_compressUpdate(ctx, outBuffer, outCapacity, inBuffer, inSize, NULL);
    if(LZ4F_isError(compressed)) {
        throw std::runtime_error("Unable to compress chunk. Err code:" + std::to_string(compressed));
    }
    return compressed;
}

archiver::archiver::CompressedDataDefinition archiver::ArchiveBuilder::CompressFileStream(const std::filesystem::path& path) {
    size_t result_size = 0;
    //create context
    LZ4F_compressionContext_t ctx;
    size_t const ctxCreation = LZ4F_createCompressionContext(&ctx, LZ4F_VERSION);
    //select chunk size and allocate space for it
    std::ifstream input(path, std::ios::binary);
    size_t file_size = std::filesystem::file_size(path);
    size_t chunk_size = CalculateFileChunk(file_size);
    auto inBuffer = std::make_unique<char[]>(chunk_size);
    //allocate space for outBuffer
    size_t dest_size = LZ4F_compressBound(chunk_size, NULL);
    auto outBuffer = std::make_unique<char[]>(dest_size);
    //begin compression
    const size_t header_size = LZ4F_compressBegin(ctx, outBuffer.get(), dest_size, NULL);
    if(LZ4F_isError(header_size)) {
        throw std::runtime_error("Unable to begin compression");
    }
    //write header 
    writer->WriteToSection(outBuffer.get(), header_size);
    result_size += header_size;

    LibLog::LogEngine::LogConsoleDebug("File ", path, " File size ", file_size, " Chunk size ", chunk_size, " dest size ", dest_size);

    size_t read = 0;
    do {
        //read and get read bytes
        input.read(inBuffer.get(), chunk_size);
        read = input.gcount();
        //compress and write
        size_t compressed = archiver::CompressChunk(ctx, inBuffer.get(), outBuffer.get(), read, dest_size);
        writer->WriteToSection(outBuffer.get(), compressed);
        result_size += compressed;

    } while (read != 0);

    //end compression
    const size_t flushed = LZ4F_compressEnd(ctx, outBuffer.get(), dest_size, NULL);
    if(LZ4F_isError(flushed)) {
        throw std::runtime_error("Unable to end compression");
    }
    writer->WriteToSection(outBuffer.get(), flushed);
    result_size += flushed;

    //release ctx
    LZ4F_freeCompressionContext(ctx); 

    input.close();

    archiver::archiver::CompressedDataDefinition out;
    out.CompressedSize = result_size;
    out.OriginSize = file_size;
    out.crc32 = archiver::crc32b(path);
    return out;
}

size_t archiver::ArchiveBuilder::CalculateFileChunk(size_t file_size) {
    return std::min(std::max(MIN_CHUNK_SIZE, file_size / 10ULL), MAX_CHUNK_SIZE);
}