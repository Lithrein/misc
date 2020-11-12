#ifdef WITH_INSERT_NAIVE_POLICY
#ifndef __INSERT_NAIVE_POLICY_HPP__

template<
  typename T,
  template <class, class> class RotationPolicy,
  template <class> class StoragePolicy
> class InsertNaive {};

template<
  typename T,
  template <class, class> class RotationPolicy
>
struct InsertNaive<T, RotationPolicy, StorageArray>
{
  void insert(T elem, StorageArray<T>* atree) {
    std::cout << "naive" << std::endl;
    GetTree(atree).push_back(elem);
    for (auto& i : GetTree(atree)) {
      std::cout << i << std::endl;
    }
  }
};
#endif
#endif
