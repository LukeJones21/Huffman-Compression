#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>

#include "bstream.h"

TEST(BStream, input) {
std::string filename{"test_bstream_input"};

const unsigned char val[] = {
        0x58, 0x90, 0xab, 0x08,
        0x00, 0x4e, 0xdb, 0x40,
};
// Equivalent in binary is:
// 0101100010010000101010110000100000000000010011101101101101000000
// ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

// Write this to a file
std::ofstream ofs(filename, std::ios::out |
                            std::ios::trunc |
                            std::ios::binary);
ofs.write(reinterpret_cast<const char *>(val), sizeof(val));
ofs.close();

// Read it back in binary format
std::ifstream ifs(filename, std::ios::in |
                            std::ios::binary);
BinaryInputStream bis(ifs);

// Make sure that we reading the binary in the correct order
EXPECT_EQ(bis.GetBit(), 0);
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
EXPECT_EQ(bis.GetBit(), 1);
EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

ifs.close();

std::remove(filename.c_str());
}

TEST(BStream, CharInputAndOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);
    bos.PutChar('A');
    bos.PutChar('D');
    bos.PutChar('L');
    bos.PutChar('d');
    bos.PutChar('i');
    ofs.close();

    std::ifstream ifs(filename, std::ios::in |
                                std::ios::binary);
    BinaryInputStream bis(ifs);
    EXPECT_EQ(bis.GetChar(), 'A');
    EXPECT_EQ(bis.GetChar(), 'D');
    EXPECT_EQ(bis.GetChar(), 'L');
    EXPECT_EQ(bis.GetChar(), 'd');
    EXPECT_EQ(bis.GetChar(), 'i');
    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, IntInputAndOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);
    bos.PutInt(4);
    bos.PutInt(932);
    bos.PutInt(837);
    bos.PutInt(108);
    bos.PutInt(98);
    ofs.close();

    std::ifstream ifs(filename, std::ios::in |
                                std::ios::binary);
    BinaryInputStream bis(ifs);
    EXPECT_EQ(bis.GetInt(), 4);
    EXPECT_EQ(bis.GetInt(), 932);
    EXPECT_EQ(bis.GetInt(), 837);
    EXPECT_EQ(bis.GetInt(), 108);
    EXPECT_EQ(bis.GetInt(), 98);
    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, BitInputAndOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);
    // Has to be 8 bits because of the buffer
    bos.PutBit(1);
    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutBit(1);
    bos.PutBit(0);
    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutBit(0);
    ofs.close();

    std::ifstream ifs(filename, std::ios::in |
                                std::ios::binary);
    BinaryInputStream bis(ifs);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetBit(), 0);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetBit(), 0);
    EXPECT_EQ(bis.GetBit(), 0);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetBit(), 0);
    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, CharOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);

    // Put 8 chars into file
    bos.PutChar('A');
    bos.PutChar('C');
    bos.PutChar('D');
    bos.PutChar('f');
    bos.PutChar('m');
    bos.PutChar('s');
    bos.PutChar('b');
    bos.PutChar(' ');

    ofs.close();

    // Put chars from file into array
    unsigned char val[8];
    std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
    ifs.read(reinterpret_cast<char *>(val), sizeof(val));
    ifs.close();

    EXPECT_EQ(val[0], 'A');
    EXPECT_EQ(val[1], 'C');
    EXPECT_EQ(val[2], 'D');
    EXPECT_EQ(val[3], 'f');
    EXPECT_EQ(val[4], 'm');
    EXPECT_EQ(val[5], 's');
    EXPECT_EQ(val[6], 'b');
    EXPECT_EQ(val[7], ' ');
}

TEST(BStream, IntOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);

    // Put 4 ints into file
    bos.PutInt(48);        // 00000000000000000000000000110000
    bos.PutInt(99999);     // 00000000000000011000011010011111
    bos.PutInt(0);         // 00000000000000000000000000000000
    bos.PutInt(3);         // 00000000000000000000000000000011

    ofs.close();

    // Put ints from file into array
    unsigned char val[16];
    std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);
    ifs.read(reinterpret_cast<char *>(val), sizeof(val));
    ifs.close();

    EXPECT_EQ(val[0], '\0'); // 00000000 = '\0
    EXPECT_EQ(val[1], '\0'); // 00000000 = '\0
    EXPECT_EQ(val[2], '\0'); // 00000000 = '\0'
    EXPECT_EQ(val[3], '0');  // 00110000 = '0'
    EXPECT_EQ(val[4], '\0'); // 00000000 = '\0
    EXPECT_EQ(val[5], 0x01); // 00000001 =  01 (hex)
    EXPECT_EQ(val[6], 0x86); // 10000110 = 0x86
    EXPECT_EQ(val[7], 0x9F); // 10011111 = 0x9F
    EXPECT_EQ(val[8], '\0');
    EXPECT_EQ(val[9], '\0');
    EXPECT_EQ(val[10], '\0');
    EXPECT_EQ(val[11], '\0');
    EXPECT_EQ(val[12], '\0');
    EXPECT_EQ(val[13], '\0');
    EXPECT_EQ(val[14], '\0');
    EXPECT_EQ(val[15], 0x03);
}

TEST(BStream, CharBitIntInputOutput) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);

    bos.PutBit(0);
    bos.PutChar('J');
    bos.PutInt(94);
    bos.PutBit(0);
    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutBit(1);
    bos.PutBit(0);

    ofs.close();

    std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);

    BinaryInputStream bis(ifs);
    EXPECT_EQ(bis.GetInt(), 620756992); 
    EXPECT_EQ(bis.GetChar(), 0x2F); 
    EXPECT_EQ(bis.GetChar(), 0x16); 
    ifs.close();

}

TEST(BStream, ExeptionThrow) {
    std::string filename("test_bstream_output");

    std::ofstream ofs(filename, std::ios::out |
                                std::ios::trunc |
                                std::ios::binary);
    BinaryOutputStream bos(ofs);

    bos.PutBit(1);
    bos.PutBit(0);
    bos.PutBit(0);

    ofs.close();

    std::ifstream ifs(filename, std::ios::in |
                    std::ios::binary);

    BinaryInputStream bis(ifs);

    EXPECT_THROW(bis.GetBit(), std::exception);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
