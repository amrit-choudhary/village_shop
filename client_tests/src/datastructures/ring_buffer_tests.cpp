#include "ring_buffer_tests.h"

#include <iostream>

bool TEST::Test_RingBuffer1() {
    std::cout << "\n\nTEST: Starting: Ring Buffer Test = initial count zero." << '\n';

    VG::RingBuffer<int> rb(10);

    if (rb.GetCount() == 0) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}

bool TEST::Test_RingBuffer2() {
    std::cout << "\n\nTEST: Starting: Ring Buffer Test = correct count after adding." << '\n';

    VG::RingBuffer<int> rb(5);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    if (rb.GetCount() == 4) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}

bool TEST::Test_RingBuffer3() {
    std::cout << "\n\nTEST: Starting: Ring Buffer Test = count after adding excess items." << '\n';

    VG::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    if (rb.GetCount() == 3) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}

bool TEST::Test_RingBuffer4() {
    std::cout << "\n\nTEST: Starting: Ring Buffer Test = get after excess." << '\n';

    VG::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    if (rb.Get(0) == 2) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}

bool TEST::Test_RingBuffer5() {
    std::cout << "\n\nTEST: Starting: Ring Buffer Test = get using subscript." << '\n';

    VG::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    if (rb[0] == 2) {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}
