#pragma once
#include "stream.h"
#include <mini/common.h>
#include <mini/string.h>
#include <mini/byte_buffer.h>

#include <cstdint>

namespace mini::io {

class stream_reader
{
  public:
    stream_reader(
      stream& stream
      )
      : _stream(stream)
    {

    }

    byte_buffer
    read_to_end(
      void
      )
    {
      byte_buffer result;

      static constexpr size_t buffer_size = 4096;
      byte_type buffer[buffer_size];

      size_t bytes_read;

      while ((bytes_read = _stream.read(buffer, buffer_size)) != 0)
      {
        result.add_many(byte_buffer_ref(buffer, buffer + bytes_read));
      }

      return result;
    }

    string
    read_string_to_end(
      void
      )
    {
      string result;

      static constexpr size_t buffer_size = 4096;
      char buffer[buffer_size + 1];

      size_t bytes_read;

      while ((bytes_read = _stream.read(buffer, buffer_size)) != 0)
      {
        buffer[bytes_read] = '\0';
        result.append(buffer);
      }

      return result;
    }


  private:
    stream& _stream;
};

}
