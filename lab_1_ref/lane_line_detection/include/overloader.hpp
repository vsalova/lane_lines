#ifndef OVERLOADER_HPP
#define OVERLOADER_HPP

template <class T> std::ostream& operator << (std::ostream& os, const std::vector<T*>& v) {

  os << "[";
  if (!v.size()) {
    os << "]";
    return os;
  }
  for (const T* val: v) {
    if (val == v.back()) {
      os << *val << "]";
      return os;
    }
    else {
      os << *val << ", ";
    }
  }
  return os;
}

template <class T> std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {

  os << "[";
  if (!v.size()) {
    os << "]";
    return os;
  }
  for (const T& val: v) {
    if (&val == &v.back()) {
      os << val << "]";
    }
    else {
      os << val << ", ";
    }
  }
  return os;
}

template <class T, class K>
std::ostream& operator << (std::ostream& os, const std::pair<T,K>& p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}


#endif /* OVERLOADER_HPP */
