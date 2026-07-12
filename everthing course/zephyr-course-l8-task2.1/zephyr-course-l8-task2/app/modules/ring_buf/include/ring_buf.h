#ifndef RING_BUF_H
#define RING_BUF_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUF_MAX_CAPACITY 16U

/** Initialise with the given capacity [1, RING_BUF_MAX_CAPACITY]. */
int rb_init(uint32_t capacity);

/** Push a value; returns -ENOSPC if full. */
int rb_push(int value);

/** Pop the oldest value; returns -EINVAL if value is NULL, -ENODATA if empty. */
int rb_pop(int *value);

/** Peek at the oldest value without consuming it; same error codes as rb_pop. */
int rb_peek(int *value);

bool     rb_is_full(void);
bool     rb_is_empty(void);
uint32_t rb_count(void);

#ifdef __cplusplus
}
#endif

#endif /* RING_BUF_H */