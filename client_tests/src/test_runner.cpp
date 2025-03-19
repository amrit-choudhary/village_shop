#include <iostream>

#include "FileIO/INIParser_tests.h"
#include "datastructures/ring_buffer_tests.h"

int main(int argc, char **argv) {
    // INI
    TEST::Test_INIParse();

    // Ring Buffer
    TEST::Test_RingBuffer1();
    TEST::Test_RingBuffer2();
    TEST::Test_RingBuffer3();
    TEST::Test_RingBuffer4();
    TEST::Test_RingBuffer5();

    return 0;
}