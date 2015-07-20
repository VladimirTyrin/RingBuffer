#include "ring_buffer.h"

#include <stdlib.h>
#include <string.h>


ring_buffer
buffer_init(size_t capacity)
{
  /* doesn't make sense */
  if (capacity == 0)
    return (ring_buffer) 0;
  ring_buffer buffer = (ring_buffer) malloc(sizeof(struct rb_object));
  if (! buffer)
    return (ring_buffer) 0;

  buffer->capacity = capacity;

  /*
  https://en.wikipedia.org/wiki/Circular_buffer#Always_keep_one_slot_open
  policy is used
  */
  buffer->data = malloc(capacity + 1);

  if (! buffer->data)
    {
      free(buffer);
      return (ring_buffer) 0;
    }

  /* end == begin, so buffer is empty */
  buffer->begin = buffer->data;
  buffer->end = buffer->data;
  return buffer;
}


void
destroy_ring_buffer(ring_buffer buffer)
{
  if (! buffer)
    return;
  free(buffer->data);
  free(buffer);
  /* just in case */
  buffer = (ring_buffer) 0;
}


size_t
buffer_data_size(const ring_buffer buffer)
{
  if (buffer->end >= buffer->begin)
    return buffer->end - buffer->begin;
  return (buffer->end + buffer->capacity + 1) - buffer->begin;
}


size_t
buffer_space_available(const ring_buffer buffer)
{
  return buffer->capacity - buffer_data_size(buffer);
}