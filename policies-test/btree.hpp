#ifndef __BTREE__
/* Binary search tree */
template<typename T>
struct btree {
  virtual void insert(T elem) = 0;
};
#endif
