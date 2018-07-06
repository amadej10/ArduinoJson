// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonFloat.hpp"
#include "JsonInteger.hpp"

namespace ArduinoJson {
namespace Internals {
// Forward declarations
struct JsonArrayData;
struct JsonObjectData;

// A union that defines the actual content of a JsonVariant.
// The enum JsonVariantType determines which member is in use.
union JsonVariantContent {
  JsonFloat asFloat;         // used for double and float
  JsonUInt asInteger;        // used for bool, char, short, int and longs
  JsonArrayData* asArray;    // asArray cannot be null
  JsonObjectData* asObject;  // asObject cannot be null
  struct {
    const char* data;  // asString can be null
    size_t size;
  } asString;
};
}  // namespace Internals
}  // namespace ArduinoJson
