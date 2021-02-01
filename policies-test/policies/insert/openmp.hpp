#ifdef WITH_INSERT_OPENMP_POLICY
#ifndef __INSERT_OPENMP_POLICY_HPP__

template<typename T, typename U> class InsertOpenMP {};

template<typename T>
struct InsertOpenMP<T, _avl_array<T>> {

  void insert(T elem, _avl_array<T>* atree) {
    GetTree(atree).push_back(elem);
  }
};
#endif
#endif
