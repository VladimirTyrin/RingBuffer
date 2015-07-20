#include "ring_buffer_test.h"

#include "ring_buffer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>

#include <cmockery/cmockery.h>

#define UNIT_TESTING 1
#if UNIT_TESTING
  #include <cmockery/cmockery_override.h>
#endif


static const size_t CAPACITY = 10;


static
void
init_destroy_test(void **state)
{
  (void) state;
  ring_buffer buffer = buffer_init(CAPACITY);

  assert_non_null(buffer);
  assert_non_null(buffer->data);
  assert_int_equal(buffer->capacity, CAPACITY);
  assert_int_equal(buffer_data_size(buffer), 0);
  assert_int_equal(buffer_space_available(buffer), CAPACITY);
  assert_ptr_equal(buffer->begin, buffer->data);
  assert_ptr_equal(buffer->end, buffer->data);

  buffer_destroy(buffer);
}


static
void
push_read_test(void **state)
{
  (void) state;
  ring_buffer buffer = buffer_init((2 * sizeof(int)));
  int a_src = 1;
  int b_src = 2;
  int a_fin = -1;
  int b_fin = -2;
  uint8_t byte_src = 5;
  uint8_t byte_fin = 10;

  buffer_push(buffer, &byte_src, 1);
  assert_int_equal(buffer_data_size(buffer), 1);
  buffer_push(buffer, &a_src, sizeof(int));
  buffer_pop(buffer, &byte_fin, 1);
  assert_int_equal(buffer_data_size(buffer), sizeof(int));
  buffer_push(buffer, &b_src, sizeof(int));

  buffer_pop(buffer, &a_fin, sizeof(int));
  buffer_pop(buffer, &b_fin, sizeof(int));

  assert_int_equal(byte_src, byte_fin);
  assert_int_equal(a_src, a_fin);
  assert_int_equal(b_src, b_fin);

  buffer_destroy(buffer);
}


static
void
clear_test(void **state)
{
  (void) state;
  ring_buffer buffer = buffer_init(CAPACITY);
  int int_var = 1;

  buffer_push(buffer, &int_var, sizeof(int));
  buffer_clear(buffer);

  assert_int_equal(buffer_space_available(buffer), CAPACITY);
  assert_ptr_equal(buffer->begin, buffer->data);
  assert_ptr_equal(buffer->end, buffer->data);

  buffer_destroy(buffer);
}


static
void
errors_test(void **state)
{
  (void) state;

  ring_buffer buffer = buffer_init(0);
  assert_int_equal(buffer, 0);

  buffer = buffer_init(CAPACITY);
  
  int buffer_state = buffer_push(buffer, buffer, CAPACITY + 1);
  assert_int_equal(buffer_state, BS_ENOMEM);

  buffer_state = buffer_read(buffer, buffer, CAPACITY + 1);
  assert_int_equal(buffer_state, BS_EINVAL);
}

/* Standard tests for ring_buffer */
static const
UnitTest
tests[] = {
  unit_test(init_destroy_test),
  unit_test(push_read_test),
  unit_test(clear_test),
  unit_test(errors_test)
};


int
test_ring_buffer(void)
{
  return run_tests(tests, "run");
}