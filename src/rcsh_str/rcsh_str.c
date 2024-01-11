#include <stdlib.h>
#include <string.h>

#include <rcsh_str.h>

rcsh_str_t *
rcsh_str_init (void)
{
  rcsh_str_t *self = calloc (1, sizeof (rcsh_str_t));

  return self;
}

char *
rcsh_str_deinit (rcsh_str_t **const self)
{
  if (self == NULL || *self == NULL)
    return NULL;

  char *ret_buf = (*self)->buffer;

  (*self)->buffer = NULL;
  (*self)->length = 0;
  (*self)->capacity = 0;
  free (*self);
  *self = NULL;

  return ret_buf;
}

void
__rcsh_str_resize_buffer (rcsh_str_t *const self, size_t n)
{
  size_t new_capacity = self->capacity == 0 ? 16 : self->capacity * 2;
  size_t new_length = self->length + n + 1;

  while (new_capacity <= new_length)
    new_capacity *= 2;

  char *new_buffer = calloc (new_capacity, sizeof (char));
  if (self->buffer != NULL)
    strncat (new_buffer, self->buffer, self->length);

  free (self->buffer);
  self->buffer = new_buffer;
  self->capacity = new_capacity;
}

void
rcsh_str_append_str (rcsh_str_t *const self, char *str)
{
  if (self == NULL || str == NULL)
    return;

  size_t len = strlen (str);
  rcsh_str_append_str_n (self, str, len);
}

void
rcsh_str_append_str_n (rcsh_str_t *const self, const char *str, size_t n)
{
  if (self == NULL || str == NULL)
    return;

  size_t new_length = self->length + n + 1;

  if (new_length >= self->capacity)
    {
      __rcsh_str_resize_buffer (self, n);
    }

  strncat (self->buffer, str, n);

  self->length += n;
}

void
rcsh_str_append_char (rcsh_str_t *const self, const char c)
{
  if (self == NULL)
    return;

  size_t new_length = self->length + 1 + 1;

  if (new_length >= self->capacity)
    {
      __rcsh_str_resize_buffer (self, 1);
    }

  char buf[2] = { c, '\0' };

  strncat (self->buffer, buf, 1);
  self->length += 1;
}
