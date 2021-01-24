//
// Created by stew3254 on 1/22/21.
//

#include <stdio.h>
#include "tree.h"
#include "list.h"

int main() {
  tree *t = tree_new();
  list *l = list_new();
  for (int i = 0; i < 5; ++i) {
    list_push_back(l, i);
//    list_push_front(l, i);
  }
  list_print(l, "%d\n");
  printf("%s\n", "test");
}
