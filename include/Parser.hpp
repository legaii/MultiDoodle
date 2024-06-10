#ifndef PARSER_HPP
#define PARSER_HPP

#include <limits>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <iostream>

namespace sj {

template <class T, class F>
struct FMap {
    typedef typename F::ReturnT ReturnT;
    static ReturnT function(const T& t);
};

template <class T, class F>
struct FMap<std::optional<T>, F> {
    typedef std::optional<typename FMap<T, F>::ReturnT> ReturnT;
    static ReturnT function(const std::optional<T>& t);
};

template <class From, class To>
struct Id {
    typedef To ReturnT;
    static ReturnT function(const From& x);
};

struct VectorToString {
    typedef std::string ReturnT;
    static ReturnT function(const std::vector<char>& v);
};

struct StringToVector {
    typedef std::vector<char> ReturnT;
    static ReturnT function(const std::string& s);
};

template <class T1, class T2>
struct TupleToPair {
    typedef std::pair<T1, T2> ReturnT;
    static ReturnT function(const std::tuple<T1, T2>& t);
};

template <class T>
struct LoadResult {
    T result;
    int offset;
};

template <class T, class F>
struct FMap<LoadResult<T>, F> {
    typedef LoadResult<typename FMap<T, F>::ReturnT> ReturnT;
    static ReturnT function(const LoadResult<T>& t);
};

template <class T>
struct Load {};

template <>
struct Load<int> {
    typedef std::optional<LoadResult<int>> ReturnT;
    static ReturnT function(const std::string& s);
};

template <>
struct Load<bool> {
    typedef FMap<Load<int>::ReturnT, Id<int, bool>>::ReturnT ReturnT;
    static ReturnT function(const std::string& s);
};

template <>
struct Load<char> {
    typedef FMap<Load<int>::ReturnT, Id<int, char>>::ReturnT ReturnT;
    static ReturnT function(const std::string& s);
};

template <class T>
struct Load<std::vector<T>> {
    typedef std::optional<LoadResult<std::vector<T>>> ReturnT;
    static ReturnT function(const std::string& s);
};

template <>
struct Load<std::string> {
    typedef FMap<Load<std::vector<char>>::ReturnT, VectorToString>::ReturnT ReturnT;
    static ReturnT function(const std::string& s);
};

template <>
struct Load<std::tuple<>> {
    typedef std::optional<LoadResult<std::tuple<>>> ReturnT;
    static ReturnT function(const std::string& s);
};

template <class T, class... Ts>
struct Load<std::tuple<T, Ts...>> {
    typedef std::optional<LoadResult<std::tuple<T, Ts...>>> ReturnT;
    static ReturnT function(const std::string& s);
};

template <class T1, class T2>
struct Load<std::pair<T1, T2>> {
    typedef typename FMap<typename Load<std::tuple<T1, T2>>::ReturnT,
                          TupleToPair<T1, T2>>::ReturnT ReturnT;
    static ReturnT function(const std::string& s);
};

template <class T>
struct Dump {};

template <>
struct Dump<int> {
    typedef std::string ReturnT;
    static ReturnT function(const int& t);
};

template <>
struct Dump<bool> {
    typedef std::string ReturnT;
    static ReturnT function(const bool& t);
};

template <>
struct Dump<char> {
    typedef std::string ReturnT;
    static ReturnT function(const char& t);
};

template <class T>
struct Dump<std::vector<T>> {
    typedef std::string ReturnT;
    static ReturnT function(const std::vector<T>& t);
};

template <>
struct Dump<std::string> {
    typedef std::string ReturnT;
    static ReturnT function(const std::string& t);
};

template <>
struct Dump<std::tuple<>> {
    typedef std::string ReturnT;
    static ReturnT function(const std::tuple<>& t);
};

template <class T, class... Ts>
struct Dump<std::tuple<T, Ts...>> {
    typedef std::string ReturnT;
    static ReturnT function(const std::tuple<T, Ts...>& t);
};

template <class T1, class T2>
struct Dump<std::pair<T1, T2>> {
    typedef std::string ReturnT;
    static ReturnT function(const std::pair<T1, T2>& t);
};

}  // namespace sj


template <class T, class F>
typename sj::FMap<T, F>::ReturnT sj::FMap<T, F>::function(const T& t) {
  return F::function(t);
}

template <class T, class F>
typename sj::FMap<std::optional<T>, F>::ReturnT sj::FMap<std::optional<T>, F>::function(
    const std::optional<T>& t) {
  if (!t) {
    return {};
  }
  return FMap<T, F>::function(t.value());
}

template <class From, class To>
typename sj::Id<From, To>::ReturnT sj::Id<From, To>::function(const From& x) {
  return static_cast<To>(x);
}

template <class T1, class T2>
typename sj::TupleToPair<T1, T2>::ReturnT sj::TupleToPair<T1, T2>::function(
    const std::tuple<T1, T2>& t) {
  return {std::get<0>(t), std::get<1>(t)};
}

template <class T, class F>
typename sj::FMap<sj::LoadResult<T>, F>::ReturnT sj::FMap<sj::LoadResult<T>, F>::function(
    const LoadResult<T>& t) {
  return {FMap<T, F>::function(t.result), t.offset};
}

template <class T>
typename sj::Load<std::vector<T>>::ReturnT sj::Load<std::vector<T>>::function(
    const std::string& s) {
  ReturnT ans;
  ans.emplace();
  ans.value().offset = 0;
  Load<int>::ReturnT maybe_n = Load<int>::function(s.substr(ans.value().offset));
  if (!maybe_n || (ans.value().offset += maybe_n.value().offset) > (int)s.size()) {
    return {};
  }
  int n = maybe_n.value().result;
  if (n < 0) {
    return {};
  }
  for (int i = 0; i < n; ++i) {
    typename Load<T>::ReturnT maybe_t = Load<T>::function(s.substr(ans.value().offset));
    if (!maybe_t || (ans.value().offset += maybe_t.value().offset) > (int)s.size()) {
      return {};
    }
    ans.value().result.push_back(maybe_t.value().result);
  }
  return ans;
}

template <class T, class... Ts>
typename sj::Load<std::tuple<T, Ts...>>::ReturnT sj::Load<std::tuple<T, Ts...>>::function(
    const std::string& s) {
  ReturnT ans;
  ans.emplace();
  ans.value().offset = 0;
  typename Load<T>::ReturnT maybe_t = Load<T>::function(s.substr(ans.value().offset));
  if (!maybe_t || (ans.value().offset += maybe_t.value().offset) > (int)s.size()) {
    return {};
  }
  typename Load<std::tuple<Ts...>>::ReturnT maybe_ts =
      Load<std::tuple<Ts...>>::function(s.substr(ans.value().offset));
  if (!maybe_ts || (ans.value().offset += maybe_ts.value().offset) > (int)s.size()) {
    return {};
  }
  ans.value().result =
      std::tuple_cat(std::make_tuple(maybe_t.value().result), maybe_ts.value().result);
  return ans;
}

template <class T1, class T2>
typename sj::Load<std::pair<T1, T2>>::ReturnT sj::Load<std::pair<T1, T2>>::function(
    const std::string& s) {
  return FMap<typename Load<std::tuple<T1, T2>>::ReturnT, TupleToPair<T1, T2>>::function(
      Load<std::tuple<T1, T2>>::function(s));
}

template <class T>
typename sj::Dump<std::vector<T>>::ReturnT sj::Dump<std::vector<T>>::function(
    const std::vector<T>& t) {
  ReturnT ans;
  int n = (int)t.size();
  ans += Dump<int>::function(n);
  for (const T& ti : t) {
    ans += Dump<T>::function(ti);
  }
  return ans;
}

template <class T, class... Ts>
typename sj::Dump<std::tuple<T, Ts...>>::ReturnT sj::Dump<std::tuple<T, Ts...>>::function(
    const std::tuple<T, Ts...>& t) {
  return std::apply(
      [](T t, Ts... ts) -> sj::Dump<std::tuple<T, Ts...>>::ReturnT {
        return Dump<T>::function(t) +
               Dump<std::tuple<Ts...>>::function(std::make_tuple(ts...));
      },
      t);
}

template <class T1, class T2>
typename sj::Dump<std::pair<T1, T2>>::ReturnT sj::Dump<std::pair<T1, T2>>::function(
    const std::pair<T1, T2>& t) {
  return Dump<std::tuple<T1, T2>>::function(
      Id<std::pair<T1, T2>, std::tuple<T1, T2>>::function(t));
}

#endif
