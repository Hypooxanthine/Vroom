#pragma once

// From https://en.cppreference.com/w/cpp/utility/variant/visit2
template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};