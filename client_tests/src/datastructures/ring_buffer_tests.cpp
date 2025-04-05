/**
 * Tests for ring buffer.
 */

#include "../test_runner.h"
#include "src/datastructure/ring_buffer.h"

TEST(RingBuffer, InitialCountZero) {
    ME::RingBuffer<int> rb(10);

    EXPECT_EQ(rb.GetCount() == 0);
}

TEST(RingBuffer, InitialCountCorrectAfterAdding) {
    ME::RingBuffer<int> rb(5);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT_EQ(rb.GetCount() == 4);
}

TEST(RingBuffer, InitialCountCorrectAfterAddingExcess) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT_EQ(rb.GetCount() == 3);
}

TEST(RingBuffer, GetAfterExcess) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT_EQ(rb.Get(0) == 2);
}

TEST(RingBuffer, GetUsingSubscript) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT_EQ(rb[0] == 2);
}
