#ifndef SERDA_RANGE_H
#define SERDA_RANGE_H

#include <utility>

// Range to allow for range-loops over the mapped elements.
template <typename Map>
struct ValueRange {
 public:
  typedef typename Map::mapped_type value_type;

  class iterator : public Map::iterator {
   public:
    iterator() : Map::iterator() {}
    iterator(const typename Map::iterator& e) : Map::iterator(e) {}

    value_type* operator->() { return &Map::iterator::operator->()->second; }
    value_type& operator*() { return Map::iterator::operator*().second; }
  };
  ValueRange(Map& map) : map(map) {}
  iterator begin() { return map.begin(); }
  iterator end() { return map.end(); }
  std::size_t size() const { return map.size(); }

 private:
  Map& map;
};

// Range to allow for range-loops over pairs of elements.
template <typename Container>
struct PairRange {
 public:
  typedef typename Container::value_type single_type;
  typedef std::pair<single_type&, single_type&> value_type;
  typedef typename Container::iterator container_iterator;

  class iterator {
   public:
    iterator(Container& container, container_iterator first,
             container_iterator second)
        : container(container), p(first, second) {}

    value_type operator*() { return value_type(*p.first, *p.second); }
    bool operator!=(const iterator& pi) const { return p != pi.p; }
    iterator& operator++();

   private:
    Container& container;
    std::pair<container_iterator, container_iterator> p;
  };

  PairRange(Container& container) : container(container) {}
  iterator begin() {
    if (container.size() < 2) return end();
    return iterator(container, container.begin(), ++container.begin());
  }
  iterator end() {
    return iterator(container, container.end(), container.end());
  }

 private:
  Container& container;
};

template <typename Container>
typename PairRange<Container>::iterator& PairRange<Container>::iterator::
operator++() {
  ++p.second;
  if (p.second == container.end()) {
    p.second = ++p.first;
    ++p.second;
    // We don't want 'second' to be at end() if 'first' is not.
    if (p.second == container.end()) ++p.first;
  }
  return *this;
}

// Range to allow for range-loops over pairs of mapped elements.
template <typename Map>
class ValuePairRange : public PairRange<ValueRange<Map>> {
 public:
  ValuePairRange(Map& map)
      : PairRange<ValueRange<Map>>(value_range),
        value_range(ValueRange<Map>(map)) {}

 private:
  ValueRange<Map> value_range;
};

#endif  // SERDA_RANGE_H
