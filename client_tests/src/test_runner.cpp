#include <iostream>

#include "datastructures/ring_buffer_tests.h"
#include "file_io/ini_parser_tests.h"
#include "math/vector_tests.h"

int main(int argc, char **argv) {
    // INI
    TEST::Test_INIParse();

    // Ring Buffer
    TEST::Test_RingBuffer1();
    TEST::Test_RingBuffer2();
    TEST::Test_RingBuffer3();
    TEST::Test_RingBuffer4();
    TEST::Test_RingBuffer5();

    // Test
    TEST::Test_Vec31();
    TEST::Test_Vec32();
    TEST::Test_Vec33();
    TEST::Test_Vec34();
    return 0;
}