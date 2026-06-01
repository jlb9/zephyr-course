#include "ring_buf.h"

#include <errno.h>
#include <stddef.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ring_buf);

static struct {
	int      data[RING_BUF_MAX_CAPACITY];
	uint32_t capacity;
	uint32_t head;  /* oldest element — read/pop position  */
	uint32_t tail;  /* next free slot  — write/push position */
	uint32_t count;
} rb;

int rb_init(uint32_t capacity)
{
	if (capacity == 0U || capacity > RING_BUF_MAX_CAPACITY) {
		return -EINVAL;
	}
	rb.capacity = capacity;
	rb.head     = 0U;
	rb.tail     = 0U;
	rb.count    = 0U;
	return 0;
}

int rb_push(int value)
{
	if (rb.count >= rb.capacity) {
		return -ENOSPC;
	}
	rb.data[rb.tail] = value;
	rb.tail = (rb.tail + 1U) % rb.capacity;
	rb.count++;
	return 0;
}

int rb_pop(int *value)
{
	if (value == NULL) {
		return -EINVAL;
	}
	if (rb.count == 0U) {
		return -ENODATA;
	}
	*value = rb.data[rb.head];
	rb.head = (rb.head + 1U) % rb.capacity;
	rb.count--;
	return 0;
}

int rb_peek(int *value)
{
	if (value == NULL) {
		return -EINVAL;
	}
	if (rb.count == 0U) {
		return -ENODATA;
	}
	*value = rb.data[rb.head];
	return 0;
}

bool rb_is_full(void)  { return rb.count >= rb.capacity; }
bool rb_is_empty(void) { return rb.count == 0U; }
uint32_t rb_count(void) { return rb.count; }
