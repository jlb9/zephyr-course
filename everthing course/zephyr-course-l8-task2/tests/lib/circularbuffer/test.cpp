#include <zephyr/ztest.h>
#include <circularbuffer.hpp>

ZTEST_SUITE(circularbuffer, NULL, NULL, NULL, NULL, NULL);

ZTEST(circularbuffer, test_initialize) {
    constexpr uint32_t testBufferLength{4};
    circularBuffer<uint8_t, testBufferLength> theBuffer;
    zassert_equal(0, theBuffer.head);
    zassert_equal(0, theBuffer.level);
    zassert_equal(testBufferLength, theBuffer.length);
    zassert_true(theBuffer.isEmpty());
}

ZTEST(circularbuffer, test_push_pop) {
    constexpr uint32_t testBufferLength{4};
    circularBuffer<uint8_t, testBufferLength> theBuffer;
    theBuffer.push(0x11);
    theBuffer.push(0x22);
    theBuffer.push(0x33);
    theBuffer.push(0x44);
    zassert_equal(testBufferLength, theBuffer.level);
    zassert_equal(0, theBuffer.head);
    zassert_false(theBuffer.isEmpty());

    zassert_equal(0x11, theBuffer.pop());
    zassert_equal(0x22, theBuffer.pop());
    zassert_equal(0x33, theBuffer.pop());
    zassert_equal(0x44, theBuffer.pop());
    zassert_equal(0, theBuffer.level);
    zassert_equal(0, theBuffer.head);
    zassert_true(theBuffer.isEmpty());
}

ZTEST(circularbuffer, test_push_pop_wrap) {
    constexpr uint32_t testBufferLength{4};
    circularBuffer<uint8_t, testBufferLength> theBuffer;
    theBuffer.push(0x11);
    theBuffer.push(0x22);
    (void)theBuffer.pop();
    (void)theBuffer.pop();
    zassert_equal(0, theBuffer.level);
    zassert_equal(2, theBuffer.head);

    theBuffer.push(0x33);
    theBuffer.push(0x44);
    theBuffer.push(0x55);
    theBuffer.push(0x66);

    zassert_equal(testBufferLength, theBuffer.level);
    zassert_equal(2, theBuffer.head);
    zassert_equal(0x33, theBuffer.pop());
    zassert_equal(0x44, theBuffer.pop());
    zassert_equal(0x55, theBuffer.pop());
    zassert_equal(0x66, theBuffer.pop());
    zassert_equal(0, theBuffer.level);
    zassert_equal(2, theBuffer.head);
}

ZTEST(circularbuffer, test_push_overflow) {
    constexpr uint32_t testBufferLength{4};
    circularBuffer<uint8_t, testBufferLength> theBuffer;
    theBuffer.push(0x11);
    theBuffer.push(0x22);
    theBuffer.push(0x33);
    theBuffer.push(0x44);
    theBuffer.push(0x55);
    theBuffer.push(0x66);

    zassert_equal(testBufferLength, theBuffer.level);
    zassert_equal(2, theBuffer.head);
    zassert_equal(0x33, theBuffer.pop());
    zassert_equal(0x44, theBuffer.pop());
    zassert_equal(0x55, theBuffer.pop());
    zassert_equal(0x66, theBuffer.pop());
    zassert_equal(0, theBuffer.level);
    zassert_equal(2, theBuffer.head);
}

ZTEST(circularbuffer, test_pop_underflow) {
    constexpr uint32_t testBufferLength{4};
    circularBuffer<uint8_t, testBufferLength> theBuffer;
    zassert_equal(0x00, theBuffer.pop());
    zassert_equal(0, theBuffer.level);
    zassert_equal(0, theBuffer.head);
}
