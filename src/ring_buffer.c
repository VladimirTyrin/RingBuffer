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
buffer_destroy(ring_buffer buffer)
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


enum BUFFER_STATUS
buffer_push(ring_buffer buffer, void *data, size_t data_size)
{
  if (data_size > buffer_space_available(buffer))
    return BS_ENOMEM;

  size_t end_free_segment;
  if (buffer->end >= buffer->begin)
    {
      end_free_segment = buffer->data + buffer->capacity - buffer->end
                         + (buffer->begin == buffer->data ? 0 : 1);
      if (end_free_segment >= data_size) /* Simple case */
        {
          memcpy(buffer->end, data, data_size);
          buffer->end += data_size;
        }
      else
        {
          memcpy(buffer->end, data, end_free_segment);
          memcpy(buffer->data, data + end_free_segment,
                 data_size - end_free_segment);
          buffer->end = buffer->data + data_size - end_free_segment;
        }
    }
  else
    {
      memcpy(buffer->end, data, data_size);
      buffer->end += data_size;
    }
  return BS_OK;
}


enum BUFFER_STATUS
buffer_read(ring_buffer buffer, void *dest, size_t data_size)
{
  if (data_size > buffer_data_size(buffer))
    return BS_EINVAL;

  size_t end_data_segment;
  if (buffer->end >= buffer->begin)
    {
      memcpy(dest, buffer->begin, data_size);
    }
  else
    {
      end_data_segment = buffer->data + buffer->capacity - buffer->begin
                         + (buffer->begin == buffer->data ? 0 : 1);
      if (end_data_segment >= data_size) /* Simple case */
        {
          memcpy(dest, buffer->begin, data_size);
        }
      else
        {
          memcpy(dest, buffer->begin, end_data_segment);
          memcpy(dest + end_data_segment, buffer->data,
                 data_size - end_data_segment);
        }
    }
  return BS_OK;
}


enum BUFFER_STATUS
buffer_pop(ring_buffer buffer, void *dest, size_t data_size)
{
  enum BUFFER_STATUS result = buffer_read(buffer, dest, data_size);
  if (result == BS_OK)
    {
      buffer->begin += data_size;
      if (buffer->begin > buffer->data + buffer->capacity)
        buffer->begin -= buffer->capacity;
    }
  return result;
}


void
buffer_clear(ring_buffer buffer)
{
  buffer->begin = buffer->data;
  buffer->end = buffer->data;
}


void
buffer_print_info(const ring_buffer buffer, FILE *stream)
{
  fprintf(stream, "-------- RING BUFFER ----------\n");
  fprintf(stream, "Capacity:   %zu\n", buffer->capacity);
  fprintf(stream, "Data size:  %zu\n", buffer_data_size(buffer));
  fprintf(stream, "Free space: %zu\n", buffer_space_available(buffer));
  fprintf(stream, "Begin:      %ld\n", buffer->begin - buffer->data);
  fprintf(stream, "End:        %ld\n", buffer->end - buffer->data);
}
