#pragma once

#include "Vroom/Core/Assert.h"
#include "Vroom/Api.h"

#include <nlohmann/json.hpp>
#include <optional>
#include <string_view>
#include <glm/glm.hpp>

#define CHECK(x, ...) VRM_CHECK_RET_FALSE_MSG(x, __VA_ARGS__)

using json = nlohmann::json;

namespace vrm
{

#define ELSE else{} else

#define CHECK_ATTR(x, attr)                                           \
  CHECK(x.contains(#attr), #x " must contain a \"" #attr "\" attribute. " #x " is {}", x.dump(2)); \
  const auto &attr = x.at(#attr)

#define IF_HAS_ATTR(x, attr) \
  std::optional<json> attr##_opt;\
  if (x.contains(#attr)) attr##_opt = x.at(#attr);\
  if (attr##_opt.has_value())\
    if (const auto& attr = attr##_opt.value(); true)

#define TYPE_CHECK_FUNC(type) is_##type

#define IF_TYPE(x, type) \
  std::optional<type> x##Val_opt;\
  if (IsType<type>(x)) x##Val_opt = x.get<type>();\
  if (x##Val_opt.has_value())\
    if (const auto& x##Val = x##Val_opt.value(); true)

#define IF_STRING(x) \
  IF_TYPE(x, std::string)

#define IF_ARRAY(x) \
  IF_TYPE(x, json::array_t)

#define IF_OBJECT(x) \
  IF_TYPE(x, json::object_t)

#define IF_FLOAT(x) \
  IF_TYPE(x, float)

#define CHECK_TYPE(x, type) \
  CHECK(IsType<type>(x), "Json element must be of type " #type ":\n{}", x.dump(2));

#define CHECK_STRING(x) \
  CHECK_TYPE(x, std::string)\
  auto x##Val = x.get<std::string>()

#define CHECK_ARRAY(x) \
  CHECK_TYPE(x, json::array_t)

#define CHECK_OBJECT(x) \
  CHECK_TYPE(x, json::object_t)

#define CHECK_FLOAT(x) \
  CHECK_TYPE(x, float)\
  auto x##Val = x.get<float>()

#define CHECK_ATTR_TYPE(x, attr, type) \
  CHECK_ATTR(x, attr);                 \
  CHECK_TYPE(attr, type);\
  auto attr##Val = attr.get<type>()

#define CHECK_ATTR_STRING(x, attr)  \
  CHECK_ATTR_TYPE(x, attr, std::string)

#define CHECK_ATTR_ARRAY(x, attr) \
  CHECK_ATTR_TYPE(x, attr, json::array_t)

#define CHECK_ATTR_OBJECT(x, attr) \
  CHECK_ATTR_TYPE(x, attr, json::object_t)

#define CHECK_ATTR_FLOAT(x, attr) \
  CHECK_ATTR_TYPE(x, attr, float)

#define CHECK_ATTR_BOOL(x, attr) \
  CHECK_ATTR_TYPE(x, attr, bool)

#define IF_HAS_ATTR_TYPE(x, attr, type)\
  IF_HAS_ATTR(x, attr){}\
  std::optional<type> attr##Val##_opt;\
  if (attr##_opt.has_value())\
  {\
    CHECK(IsType<type>(*attr##_opt), "Json element must be of type " #type)\
    attr##Val##_opt = attr##_opt->get<type>();\
  }\
  if (attr##Val##_opt.has_value())\
    if (const auto& attr##Val = attr##Val##_opt.value(); true)

#define IF_HAS_ATTR_STRING(x, attr)  \
  IF_HAS_ATTR_TYPE(x, attr, std::string)

#define IF_HAS_ATTR_ARRAY(x, attr) \
  IF_HAS_ATTR_TYPE(x, attr, json::array_t)

#define IF_HAS_ATTR_OBJECT(x, attr) \
  IF_HAS_ATTR_TYPE(x, attr, json::object_t)

#define IF_HAS_ATTR_FLOAT(x, attr) \
  IF_HAS_ATTR_TYPE(x, attr, float)

#define IF_HAS_ATTR_BOOL(x, att) \
  IF_HAS_ATTR_TYPE(x, attr, bool)


template <typename T>
inline consteval std::string_view ToString() { return "Unsupported"; }

template<typename T>
inline bool IsType(const json& value) { return false; }

template<>
inline consteval std::string_view ToString<float>() { return "float"; }

template <>
inline bool IsType<float>(const json& value) { return value.is_number_float(); }

template<>
inline consteval std::string_view ToString<bool>() { return "bool"; }

template <>
inline bool IsType<bool>(const json& value) { return value.is_boolean(); }

template<>
inline consteval std::string_view ToString<int>() { return "int"; }

template <>
inline bool IsType<int>(const json& value) { return value.is_number_integer(); }

template<>
inline consteval std::string_view ToString<unsigned int>() { return "unsigned int"; }

template <>
inline bool IsType<unsigned int>(const json& value) { return value.is_number_unsigned(); }

template<>
inline consteval std::string_view ToString<std::string>() { return "string"; }

template <>
inline bool IsType<std::string>(const json& value) { return value.is_string(); }

template <>
inline bool IsType<json::array_t>(const json& value) { return value.is_array(); }

template <>
inline bool IsType<json::object_t>(const json& value) { return value.is_object(); }

template <typename T>
inline bool GetParamValue(const json& paramValue, T& outValue)
{
  CHECK(IsType<T>(paramValue), "Unexpected type {}", paramValue.type_name());
  outValue = paramValue.get<T>();
  return true;
}

template <glm::length_t L, typename T>
inline bool GetParamValue(const json& paramValue, glm::vec<L, T>& outValue)
{
  glm::length_t i = 0;
  for (const auto& value_i : paramValue)
  {
    CHECK(
      i < L,
      "Expected an array of {} elements", L
    );
    CHECK(
      GetParamValue<T>(value_i, outValue[i]),
      "Expected type {}", ToString<T>()
    );
    ++i;
  }

  return true;
}

} // namespace vrm

namespace nlohmann
{
  
  template <glm::length_t L, typename T>
  inline void to_json(json& j, const glm::vec<L, T> p)
  {
    j = json::array();
    
    for (glm::length_t i = 0; i < L; ++i)
    {
      j.emplace_back(p[i]);
    }
  }
    
  template <glm::length_t L, typename T>
  inline void from_json(const json j, glm::vec<L, T>& p)
  {
    if (!j.is_array())
    {
      throw std::runtime_error("json must be an array");
    }

    for (glm::length_t i = 0; i < L; ++i)
    {
      p[i] = j.at(i).get<float>();
    }
  }

}