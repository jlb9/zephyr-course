#include <errno.h>
#include <zephyr/ztest.h>

#include "ring_buf.h"

static void before(void *f) {
    ARG_UNUSED(f);
    rb_init(4);
}

/* ============================================================
 * Suite: ring_buf_init
 * ============================================================ */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_init, test_fresh_state) {
    zassert_true(rb_is_empty(), "Fresh buffer must be empty");
    zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state) {
    rb_push(99);
    zassert_equal(rb_count(), 1, "count must be 1 after push");

    rb_init(4);

    zassert_true(rb_is_empty(), "Buffer must be empty after reinit");
    zassert_equal(rb_count(), 0, "count must be 0 after reinit");
}

/* ============================================================
 * Suite: ring_buf_push_pop
 * ============================================================ */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop) {
    int v = 0;

    zassert_ok(rb_push(42), "push must succeed");
    zassert_ok(rb_pop(&v), "pop must succeed");
    zassert_equal(v, 42, "popped value must be 42");
    zassert_true(rb_is_empty(), "buffer must be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order) {
    int v = 0;

    zassert_ok(rb_push(1));
    zassert_ok(rb_push(2));
    zassert_ok(rb_push(3));

    zassert_ok(rb_pop(&v));
    zassert_equal(v, 1, "first pop must return 1");

    zassert_ok(rb_pop(&v));
    zassert_equal(v, 2, "second pop must return 2");

    zassert_ok(rb_pop(&v));
    zassert_equal(v, 3, "third pop must return 3");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc) {
    zassert_ok(rb_push(1));
    zassert_ok(rb_push(2));
    zassert_ok(rb_push(3));
    zassert_ok(rb_push(4));

    zassert_equal(rb_push(5), -ENOSPC,
                  "push onto full buffer must return -ENOSPC");
}

/* ============================================================
 * Suite: ring_buf_boundaries
 * ============================================================ */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume) {
    int v = 0;

    zassert_ok(rb_push(7));

    zassert_ok(rb_peek(&v));
    zassert_equal(v, 7, "first peek must return 7");

    v = 0;
    zassert_ok(rb_peek(&v));
    zassert_equal(v, 7, "second peek must still return 7");

    zassert_equal(rb_count(), 1, "count must still be 1 after two peeks");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval) {
    zassert_equal(rb_pop(NULL), -EINVAL, "pop(NULL) must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill) {
    zassert_ok(rb_push(10));
    zassert_ok(rb_push(20));
    zassert_ok(rb_push(30));
    zassert_ok(rb_push(40));

    zassert_true(rb_is_full(), "buffer must be full after 4 pushes");
    zassert_equal(rb_count(), 4, "count must be 4");
}