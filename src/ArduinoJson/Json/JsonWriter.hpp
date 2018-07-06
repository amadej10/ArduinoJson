// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdint.h>
#include <string.h>  // for strlen
#include "../Data/JsonInteger.hpp"
#include "../Numbers/FloatParts.hpp"
#include "../Polyfills/attributes.hpp"
#include "./EscapeSequence.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename Print>
class JsonWriter {
 public:
  explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

  // Returns the number of bytes sent to the Print implementation.
  size_t bytesWritten() const {
    return _length;
  }

  void beginArray() {
    writeRaw('[');
  }
  void endArray() {
    writeRaw(']');
  }

  void beginObject() {
    writeRaw('{');
  }
  void endObject() {
    writeRaw('}');
  }

  void writeColon() {
    writeRaw(':');
  }
  void writeComma() {
    writeRaw(',');
  }

  void writeBoolean(bool value) {
    if (value)
      writeRaw("true", 4);
    else
      writeRaw("false", 5);
  }

  void writeString(const char *value) {
    if (!value) {
      writeRaw("null", 4);
    } else {
      writeRaw('\"');
      while (*value) writeChar(*value++);
      writeRaw('\"');
    }
  }

  void writeChar(char c) {
    char specialChar = EscapeSequence::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }

  template <typename TFloat>
  void writeFloat(TFloat value) {
    if (isnan(value)) return writeRaw("NaN", 3);

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (isinf(value)) return writeRaw("Infinity", 8);

    FloatParts<TFloat> parts(value);

    writeInteger(parts.integral);
    if (parts.decimalPlaces) writeDecimals(parts.decimal, parts.decimalPlaces);

    if (parts.exponent < 0) {
      writeRaw("e-", 2);
      writeInteger(-parts.exponent);
    }

    if (parts.exponent > 0) {
      writeRaw('e');
      writeInteger(parts.exponent);
    }
  }

  template <typename UInt>
  void writeInteger(UInt value) {
    char buffer[22];
    char *end = buffer + sizeof(buffer) - 1;
    char *ptr = end;

    *ptr = 0;
    do {
      *--ptr = char(value % 10 + '0');
      value = UInt(value / 10);
    } while (value);

    writeRaw(ptr);
  }

  void writeDecimals(uint32_t value, int8_t width) {
    // buffer should be big enough for all digits, the dot and the null
    // terminator
    char buffer[16];
    char *ptr = buffer + sizeof(buffer) - 1;

    // write the string in reverse order
    *ptr = 0;
    while (width--) {
      *--ptr = char(value % 10 + '0');
      value /= 10;
    }
    *--ptr = '.';

    // and dump it in the right order
    writeRaw(ptr);
  }

  void writeRaw(const char *s) {
    writeRaw(s, strlen(s));
  }
  void writeRaw(const char *s, int n) {
    _length += _sink.write(reinterpret_cast<const uint8_t *>(s), n);
  }
  void writeRaw(char c) {
    _length += _sink.write(c);
  }

 protected:
  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned
};
}  // namespace Internals
}  // namespace ArduinoJson
