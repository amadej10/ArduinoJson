// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Strings/StringTraits.hpp"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a String
template <typename TString>
class DynamicStringBuilder {
 public:
  DynamicStringBuilder(TString &str) : _str(str) {}

  size_t write(uint8_t c) {
    StringTraits<TString>::append(_str, char(c));
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    StringTraits<TString>::append(_str, reinterpret_cast<const char *>(s), n);
    return n;
  }

 private:
  DynamicStringBuilder &operator=(const DynamicStringBuilder &);

  TString &_str;
};
}  // namespace Internals
}  // namespace ArduinoJson
