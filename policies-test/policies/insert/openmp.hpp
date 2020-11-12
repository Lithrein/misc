#ifdef WITH_INSERT_OPENMP_POLICY
#ifndef __INSERT_OPENMP_POLICY_HPP__

template<typename T, typename U> class InsertOpenMP {};

template<typename T>
struct InsertOpenMP<T, StorageArray<T>>
{
  void insert(T elem, StorageArray<T>* atree) {
    std::cout << "openmp" << std::endl;
    GetTree(atree).push_back(elem);
  }
};
#endif
#endif
