// #############################################################################
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// #############################################################################

#pragma once
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/spinlock.h>

// This is a circular buffer template, so it works FIFO
// push() on an already full buffer, overwrites the oldest event.
// pop() on an empty buffer, returns 0x00

template <typename itemType, uint32_t toBeLength>
class circularBuffer {
  public:
    static constexpr uint32_t length{toBeLength};

    circularBuffer() { initialize(); };
    circularBuffer(const circularBuffer&)            = delete;
    circularBuffer& operator=(const circularBuffer&) = delete;
    circularBuffer(circularBuffer&&)                 = delete;
    circularBuffer& operator=(circularBuffer&&)      = delete;

    void initialize() {
        head  = 0;
        level = 0;
    };

    void push(itemType newItem) {
        k_spinlock_key_t key = k_spin_lock(&lock);
        buffer[(head + level) % length] = newItem;
        if (level < length) {
            level++;
        } else {
            head = (head + 1) % length;
        }
        k_spin_unlock(&lock, key);
    };

    itemType pop() {
        k_spinlock_key_t key = k_spin_lock(&lock);
        itemType result{};        // default should be all zeroes, which is returned when popping from empty buffer
        if (level > 0) {
            result = buffer[head];
            head   = (head + 1) % length;
            level--;
        }
        k_spin_unlock(&lock, key);
        return result;
    };

    bool isEmpty() const {
        return level == 0;
    };

    uint32_t getLevel() const {
        return level;
    };

#ifndef CONFIG_ZTEST

  private:
#endif
    uint32_t head{0};
    uint32_t level{0};
    itemType buffer[length]{};
    struct k_spinlock lock;
};
