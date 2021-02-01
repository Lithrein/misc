#ifdef WITH_INSERT_NAIVE_POLICY
#ifndef __INSERT_NAIVE_POLICY_HPP__

template<typename T, typename U> class InsertNaive  {};

template<typename T>
struct InsertNaive<T, _avl_array<T>> {
  void insert(T elem, _avl_array<T>* atree) {
    GetTree(atree).push_back(elem);
  }
};
#endif
#endif
