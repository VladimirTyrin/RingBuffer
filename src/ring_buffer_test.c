#include "ring_buffer_test.h"

#include "ring_buffer.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <assert.h>

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


/* Standard tests for ring_buffer */
static const
UnitTest
tests[] = {
  unit_test(init_destroy_test)
};


int
test_ring_buffer(void)
{
  return run_tests(tests, "run");
}