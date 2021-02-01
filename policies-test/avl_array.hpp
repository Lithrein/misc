#ifndef __AVL_ARRAY_HPP__
#define __AVL_ARRAY_HPP__
#include <btree.hpp>

template<typename T> class _avl_array;
template<typename T> std::vector<T>& GetTree(_avl_array<T>* avl);

#include <policies/insert/naive.hpp>
#include <policies/insert/openmp.hpp>

/* Intermediary class containing the private attributes, mandatory as a base class to give to policies */
template<typename T>
class _avl_array : public btree<T> {

#ifdef WITH_INSERT_NAIVE_POLICY
  friend class InsertNaive<T, _avl_array<T>>;
#endif
#ifdef WITH_INSERT_OPENMP_POLICY
  friend class InsertOpenMP<T, _avl_array<T>>;
#endif

protected:
  std::vector<T> tree_;
public:
  friend std::vector<T>& GetTree(_avl_array<T>* avl) {
    return avl->tree_;
  }
};


template<
  typename T,
  template <class, class> class InsertPolicy = InsertNaive
>
class avl_array
  : public _avl_array<T>
  , public InsertPolicy<T, _avl_array<T>>
{
  typedef InsertPolicy<T, _avl_array<T>> IP;

  public:
  void insert(T elem) {
    IP::insert(elem, this);
  }
};
#endif
