#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <errno.h>
#include <stdio.h>


enum BUFFER_STATUS
{
  BS_OK = 0,          /* Operation successful */
  BS_ENOMEM = ENOMEM, /* Not enough memory */
  BS_EINVAL = EINVAL  /* Invalid arguments */
};


struct rb_object
{
  void *begin;
  void *end;
  size_t capacity;
  void *data;
};

typedef struct rb_object * ring_buffer;


ring_buffer
buffer_init(size_t capacity);


void
buffer_destroy(ring_buffer buffer);


size_t
buffer_data_size(const ring_buffer buffer);


size_t
buffer_space_available(const ring_buffer buffer);


enum BUFFER_STATUS
buffer_push(ring_buffer buffer, void *data, size_t data_size);


enum BUFFER_STATUS
buffer_read(ring_buffer buffer, void *dest, size_t data_size);


enum BUFFER_STATUS
buffer_pop(ring_buffer buffer, void *dest, size_t data_size);


void
buffer_clear(ring_buffer buffer);


void
buffer_print_info(const ring_buffer buffer, FILE *stream);


#endif // RING_BUFFER_H
