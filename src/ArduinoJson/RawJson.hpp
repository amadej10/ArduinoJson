// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

namespace Internals {
// A special type of data that can be used to insert pregenerated JSON portions.
template <typename T>
class RawJsonString {
 public:
  explicit RawJsonString(T str) : _str(str) {}
  operator T() const {
    return _str;
  }

  const char* data() const {
    return _str.c_str();
  }

 private:
  T _str;
};

template <typename TChar>
class RawJsonString<TChar*> {
 public:
  explicit RawJsonString(TChar* p, size_t n) : _data(p), _size(n) {}
  operator TChar*() const {
    return _data;
  }

  TChar* data() const {
    return _data;
  }

  size_t size() const {
    return _size;
  }

 private:
  TChar* _data;
  size_t _size;
};

template <typename String>
struct StringTraits<RawJsonString<String>, void> {
  static bool is_null(RawJsonString<String> source) {
    return StringTraits<String>::is_null(static_cast<String>(source));
  }

  typedef RawJsonString<const char*> duplicate_t;

  template <typename Buffer>
  static duplicate_t duplicate(RawJsonString<String> source, Buffer* buffer) {
    return duplicate_t(
        StringTraits<String>::duplicate(source.data(), source.size(), buffer),
        source.size());
  }

  static const bool has_append = false;
  static const bool has_equals = false;
  static const bool should_duplicate = StringTraits<String>::should_duplicate;
};
}  // namespace Internals

template <typename T>
inline Internals::RawJsonString<T> RawJson(T str) {
  return Internals::RawJsonString<T>(str);
}

template <typename TChar>
inline Internals::RawJsonString<TChar*> RawJson(TChar* p) {
  return Internals::RawJsonString<TChar*>(p, strlen(p));
}

template <typename TChar>
inline Internals::RawJsonString<TChar*> RawJson(TChar* p, size_t n) {
  return Internals::RawJsonString<TChar*>(p, n);
}
}  // namespace ArduinoJson
