#pragma once

#include <algorithm>
#include <cctype>
#include <locale>

namespace vrm
{

namespace details
{
template <typename CharType>
class CaseCharEqual
{
public:

  constexpr CaseCharEqual() = default;
  constexpr CaseCharEqual(bool ignoreCase) : m_ignoreCase(ignoreCase) {}

  constexpr bool operator()(CharType left, CharType right)
  {
    if (m_ignoreCase)
      return std::toupper(left, std::locale())
          == std::toupper(right, std::locale());
    else return left == right;
  }

private:

  bool m_ignoreCase = false;
};
} // namespace details

template <typename StringType>
inline constexpr StringType::size_type FindString(const StringType& searchIn,
                                                  const StringType& searchWhat,
                                                  bool ignoreCase = false)
{
  auto it = std::search(
    searchIn.begin(), searchIn.end(), searchWhat.begin(), searchWhat.end(),
    details::CaseCharEqual<typename StringType::value_type>(ignoreCase));

  if (it != searchIn.end()) return it - searchIn.begin();
  else return searchIn.npos;
}

} // namespace vrm
