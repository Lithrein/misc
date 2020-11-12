#ifndef __BTREE_HPP__
#define __BTREE_HPP__

#define InsertPolicyType                                                      \
  template <class, template<class,class> class, template<class> class> class
#define StoragePolicyType                                                     \
  template <class> class
#define RotationPolicyType                                                    \
  template <class, class> class

template<typename T> class StorageArray;
template<typename T> typename StorageArray<T>::RefType GetTree(StorageArray<T>* bsa);

template<typename T> class StorageTree;

#include <policies/insert/naive.hpp>
#include <policies/insert/openmp.hpp>

// begin: policies/rotation/standard.hpp
template<typename T, typename U> class RotationStandard {};

template<typename T>
struct RotationStandard<T, StorageArray<T>>
{

  void right_rotate(int index, StorageArray<T> array)
  {
    std::cout << "right rotate on bsa" << std::endl;
  }

  void left_rotate(int index, StorageArray<T> array)
  {
    std::cout << "left rotate on bsa" << std::endl;
  }

};

template<typename T>
struct RotationStandard<T, StorageTree<T>>
{

  void right_rotate(int index, StorageTree<T> tree)
  {
    std::cout << "right rotate on bst" << std::endl;
  }

  void left_rotate(int index, StorageTree<T> tree)
  {
    std::cout << "left rotate on bst" << std::endl;
  }

};
// end: policies/rotation/standard.hpp

/* Intermediary class containing the private attributes, mandatory as a base
 * class to give to policies */
template<typename T>
class StorageArray
{

#ifdef WITH_INSERT_NAIVE_POLICY
  template<
    typename U,
    template <class, class> class RotationPolicy,
    template <class> class StoragePolicy
  > friend class InsertNaive;
#endif
#ifdef WITH_INSERT_OPENMP_POLICY
  template<
    typename U,
    template <class, class> class RotationPolicy,
    template <class> class StoragePolicy
  > friend class InsertOpenMP;
#endif

protected:
  std::vector<T> tree_;
public:
  typedef std::vector<T>& RefType;
  friend RefType GetTree(StorageArray<T>* bs) {
    return bs->tree_;
  }
};

template<typename T>
class StorageTree
{

#ifdef WITH_INSERT_NAIVE_POLICY
  template<template <class, class> class RotationPolicy>
    friend class InsertNaive;
#endif

protected:
  /* std::vector<T> tree_; */
public:
  /* friend std::vector<T>& GetTree(StorageArray<T>* bs) { */
  /*   return bs->tree_; */
  /* } */
};



/* binary search container */
template<
  typename T,
  InsertPolicyType   InsertPolicy   = InsertNaive,
  StoragePolicyType  StoragePolicy  = StorageArray,
  RotationPolicyType RotationPolicy = RotationStandard
>
class bsc
  : public StoragePolicy<T>
  , public InsertPolicy<T, RotationPolicy, StoragePolicy>
{
  typedef StoragePolicy<T>                               SP;
  typedef InsertPolicy<T, RotationPolicy, StoragePolicy> IP;

  public:
  void insert(T elem) {
    IP::insert(elem, this);
  }
};
#endif
