#include <vector>
#include <iostream>

#include "config.hpp"
#include "avl_array.hpp"

int
main () {
  avl_array<int, InsertOpenMP> tree;
  tree.insert(1);
  tree.insert(2);
  return 0;
}
