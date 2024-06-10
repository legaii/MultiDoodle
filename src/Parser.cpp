#include "Parser.hpp"


sj::VectorToString::ReturnT sj::VectorToString::function(const std::vector<char>& v) {
  return std::string(v.cbegin(), v.cend());
}

sj::StringToVector::ReturnT sj::StringToVector::function(const std::string& s) {
  return std::vector<char>(s.cbegin(), s.cend());
}

sj::Load<int>::ReturnT sj::Load<int>::function(const std::string& s) {
  static const int len = sizeof(int) * 2;
  if ((int)s.size() < len) {
    return {};
  }
  int res = 0;
  for (int i = 0; i < len; ++i) {
    res |= (s[i] - 'a') << (i << 2);
  }
  return {{res, len}};
}

sj::Load<bool>::ReturnT sj::Load<bool>::function(const std::string& s) {
  return FMap<Load<int>::ReturnT, Id<int, bool>>::function(Load<int>::function(s));
}

sj::Load<char>::ReturnT sj::Load<char>::function(const std::string& s) {
  return FMap<Load<int>::ReturnT, Id<int, char>>::function(Load<int>::function(s));
}

sj::Load<std::string>::ReturnT sj::Load<std::string>::function(const std::string& s) {
  return FMap<Load<std::vector<char>>::ReturnT, VectorToString>::function(
      Load<std::vector<char>>::function(s));
}

sj::Load<std::tuple<>>::ReturnT sj::Load<std::tuple<>>::function(const std::string&) {
  return {{{}, 0}};
}

sj::Dump<int>::ReturnT sj::Dump<int>::function(const int& t) {
  static const int len = sizeof(int) * 2;
  std::string res(len, 0);
  for (int i = 0; i < len; ++i) {
    res[i] = ((t >> (i << 2)) & 15) + 'a';
  }
  return res;
}

sj::Dump<bool>::ReturnT sj::Dump<bool>::function(const bool& t) {
  return Dump<int>::function(Id<bool, int>::function(t));
}

sj::Dump<char>::ReturnT sj::Dump<char>::function(const char& t) {
  return Dump<int>::function(Id<char, int>::function(t));
}

sj::Dump<std::string>::ReturnT sj::Dump<std::string>::function(const std::string& t) {
  return Dump<std::vector<char>>::function(StringToVector::function(t));
}

sj::Dump<std::tuple<>>::ReturnT sj::Dump<std::tuple<>>::function(const std::tuple<>&) {
  return "";
}
