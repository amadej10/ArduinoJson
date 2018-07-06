// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {

// Converts a compact JSON string into an indented one.
template <typename Print>
class Prettyfier {
 public:
  explicit Prettyfier(IndentedPrint<Print>& p) : _sink(p) {
    _previousChar = 0;
    _inString = false;
  }

  size_t write(uint8_t c) {
    size_t n = _inString ? handleStringChar(c) : handleMarkupChar(char(c));
    _previousChar = char(c);
    return n;
  }

  size_t write(const uint8_t* s, size_t n) {
    // TODO: optimize
    size_t bytesWritten = 0;
    while (n > 0) {
      bytesWritten += write(*s++);
      n--;
    }
    return bytesWritten;
  }

 private:
  Prettyfier& operator=(const Prettyfier&);  // cannot be assigned

  bool inEmptyBlock() {
    return _previousChar == '{' || _previousChar == '[';
  }

  size_t handleStringChar(uint8_t c) {
    bool isQuote = c == '"' && _previousChar != '\\';

    if (isQuote) _inString = false;

    return _sink.write(c);
  }

  size_t handleMarkupChar(char c) {
    switch (c) {
      case '{':
      case '[':
        return writeBlockOpen(c);

      case '}':
      case ']':
        return writeBlockClose(c);

      case ':':
        return writeColon();

      case ',':
        return writeComma();

      case '"':
        return writeQuoteOpen();

      default:
        return writeNormalChar(c);
    }
  }

  size_t writeBlockClose(char c) {
    size_t n = 0;
    n += unindentIfNeeded();
    n += writeRaw(c);
    return n;
  }

  size_t writeBlockOpen(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += writeRaw(c);
    return n;
  }

  size_t writeColon() {
    size_t n = 0;
    n += writeRaw(": ", 2);
    return n;
  }

  size_t writeComma() {
    size_t n = 0;
    n += writeRaw(",\r\n", 3);
    return n;
  }

  size_t writeQuoteOpen() {
    _inString = true;
    size_t n = 0;
    n += indentIfNeeded();
    n += writeRaw('"');
    return n;
  }

  size_t writeNormalChar(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += writeRaw(c);
    return n;
  }

  size_t indentIfNeeded() {
    if (!inEmptyBlock()) return 0;

    _sink.indent();
    return writeRaw("\r\n", 2);
  }

  size_t unindentIfNeeded() {
    if (inEmptyBlock()) return 0;

    _sink.unindent();
    return writeRaw("\r\n", 2);
  }

  size_t writeRaw(char c) {
  	return _sink.write(static_cast<uint8_t>(c));
  }

  size_t writeRaw(const char* s, size_t len) {
  	return _sink.write(reinterpret_cast<const uint8_t*>(s), len);
  } 

  char _previousChar;
  IndentedPrint<Print>& _sink;
  bool _inString;
};
}  // namespace Internals
}  // namespace ArduinoJson
