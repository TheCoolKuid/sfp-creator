//#include <fstream>
//const char data0[10];



//const char data[] = {'1','2','3','4','5','6','7','8','9','0'}; // binary block with all files that were read and zipped

//const SomeStruct[] = {}; // 1. relative path; 2. shift within data to get to the file; 3. bytesize;
///const size_t data_size = 0; // byte size of the archive-file
//const size_t file_count = 0; // number of archived files

// берешь с диска файл
// в data записать сжатый файл


// char* d;
// std::ofstream f{"test_file.c"};

// f >> d;


// std::hexidecimal

#include <iostream>
#include <fstream>
#include "../lz4/lz4/lib/lz4.h" 
#include "../archiver/archiver.h"

int main()
{
    std::cout << "Hello, World!\n" ;

    const char* data = "Uebat' nahaui Tumura!!Uebat' nahui Tumrura!!Uebeat' nahui Tumura!!Uebat' enahui rTumura!!Uebrat' nahwui Tumura!!Uebat' nahui Tumura!!Uebat' enahui Tumura!!Uebat' rnahui Tumura!!Uebat' nahui Tumura!!Uebat' nahui Teumura!!";
    const int data_size = (int)(strlen(data)+1);

    const int max_dist_size = LZ4_compressBound(data_size);

    char* c_data = new char[max_dist_size]; // allocated memory for compressed file

    // assume we are good with memory

    // now we compress and return the final size
    const int compressed_data_size = LZ4_compress_default(data, c_data, data_size, max_dist_size);

    std::cout << '0x' << std::hex << (int)'a' << std::endl;



    return 0;
}


