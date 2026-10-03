/**
 * Tests for ring buffer.
 */

#include "shared/src/datastructure/ring_buffer.h"
#include "test_framework/src/test_framework.h"

TEST(RingBuffer, InitialCountZero) {
    ME::RingBuffer<int> rb(10);

    EXPECT(rb.GetCount() == 0);
}

TEST(RingBuffer, CountAfterInserting) {
    ME::RingBuffer<int> rb(5);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT(rb.GetCount() == 4);
}

TEST(RingBuffer, CountCapsAtCapacity) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT(rb.GetCount() == 3);
}

TEST(RingBuffer, OldestOverwrittenWhenFull) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT(rb.Get(0) == 2);
    EXPECT(rb.Get(2) == 4);
}

TEST(RingBuffer, SubscriptMatchesGet) {
    ME::RingBuffer<int> rb(3);
    rb.Insert(1);
    rb.Insert(2);
    rb.Insert(3);
    rb.Insert(4);

    EXPECT(rb[0] == 2);
    EXPECT(rb[2] == 4);
}
