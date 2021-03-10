#include "avl_tree.h"

#include <algorithm>
#include <iostream>
using namespace std;

Node init_tree() {
  return get_empty_node();
}

Node insert_data(Node root, DataType new_data) {
  if (root == NULL) {
    Node new_node = get_empty_node();
    new_node->data = new_data;
    return new_node;
  }

  if (new_data < root->data) {
    root->left = insert_data(left_subtree(root), new_data);
  } else if (new_data > root->data) {
    root->right = insert_data(right_subtree(root), new_data);
  } else {
    cout << new_data << " is DUPLICATED. not inserted... ";
    return root;
  }

  root->height = new_height(root);

  return rebalance(root);
}

Node search_data(Node root, DataType target) {
  Node traveler = root;
  DataType data;
  while (traveler != NULL) {
    data = traveler->data;
    if (data == target) {
      return traveler;
    }

    traveler = data < target ? right_subtree(traveler) : left_subtree(traveler);
  }

  return NULL;
}

Node delete_data(Node root, DataType target) {
  if (root == NULL) {
    return root;
  }

  if (target < root->data) {
    root->left = delete_data(left_subtree(root), target);
  } else if (target > root->data) {
    root->right = delete_data(right_subtree(root), target);
  } else {
    Node left = left_subtree(root);
    Node right = right_subtree(root);
    if (left == NULL && right == NULL) {
      free_node(root);
      return NULL;
    } else if (left == NULL) {
      root = right;
    } else if (right == NULL) {
      root = left;
    } else {
      DataType largest = get_largest(left)->data;
      root->data = largest;
      root->left = delete_data(left, largest);
    }
  }

  if (root != NULL) {
    root->height = new_height(root);
    return rebalance(root);
  } else {
    return NULL;
  }
}

Node get_empty_node() {
  Node empty_node = new NodeStruct;
  empty_node->data = -1;
  empty_node->left = NULL;
  empty_node->right = NULL;
  empty_node->height = 0;
  return empty_node;
}

Node left_subtree(Node parent) {
  return parent->left;
}

Node right_subtree(Node parent) {
  return parent->right;
}

void connect_left_subtree(Node parent, Node new_left) {
  parent->left = new_left;
}

void connect_right_subtree(Node parent, Node new_right) {
  parent->right = new_right;
}

int height(Node node) {
  if (node == NULL) {
    return -1;
  }
  return node->height;
}

int new_height(Node node) {
  return max(height(left_subtree(node)), height(right_subtree(node))) + 1;
}

int get_balance_factor(Node node) {
  int left = height(left_subtree(node));
  int right = height(right_subtree(node));
  return left - right;
}

Node ll_rotation(Node node) {
  Node left = left_subtree(node);
  Node left_right = right_subtree(left);
  connect_left_subtree(node, left_right);
  connect_right_subtree(left, node);

  node->height = new_height(node);
  left->height = new_height(left);
  if (left_right != NULL) {
    left_right->height = new_height(left_right);
  }
  return left;
}

Node rr_rotation(Node node) {
  Node right = right_subtree(node);
  Node right_left = left_subtree(right);
  connect_right_subtree(node, right_left);
  connect_left_subtree(right, node);

  node->height = new_height(node);
  right->height = new_height(right);
  if (right_left != NULL) {
    right_left->height = new_height(right_left);
  }
  return right;
}

Node lr_rotation(Node node) {
  node->left = rr_rotation(left_subtree(node));
  return ll_rotation(node);
}

Node rl_rotation(Node node) {
  node->right = ll_rotation(right_subtree(node));
  return rr_rotation(node);
}

Node rebalance(Node node) {
  int balance_factor = get_balance_factor(node);

  if (balance_factor > 1) {                            // left subtee has more.
    if (get_balance_factor(left_subtree(node)) > 0) {  // LL case
      return ll_rotation(node);
    } else {  // LR case
      return lr_rotation(node);
    }
  } else if (balance_factor < -1) {
    if (get_balance_factor(right_subtree(node)) < 0) {  // RR case
      return rr_rotation(node);
    } else {  // RL case
      return rl_rotation(node);
    }
  } else {
    return node;
  }
}

void remove_left_subtree(Node parent) {
  Node left = parent->left;
  if (left != NULL) {
    free_node(left);
    parent->left = NULL;
  }
}

void remove_right_subtree(Node parent) {
  Node right = parent->right;
  if (right != NULL) {
    free_node(right);
    parent->right = NULL;
  }
}

Node get_largest(Node root) {
  Node traveler = root;
  while (traveler->right != NULL) {
    traveler = traveler->right;
  }
  return traveler;
}

Node get_smallest(Node root) {
  Node traveler = root;
  while (traveler->left != NULL) {
    traveler = traveler->left;
  }
  return traveler;
}

void inorder_traversal(Node root, void (*func)(Node)) {
  if (root == NULL) {
    return;
  }

  inorder_traversal(root->left, func);
  func(root);
  inorder_traversal(root->right, func);
}

void print_node(Node target) {
  cout << target->data << '[' << target->height << ']' << endl;
}

void print_to_file(Node root, ofstream& stream) {
  if (root == NULL) {
    return;
  }

  print_to_file(root->left, stream);
  stream << root->data << endl;
  print_to_file(root->right, stream);
}

void free_node(Node free_me) {
  if (free_me != NULL) {
    free_me->left = NULL;
    free_me->right = NULL;
  }
  delete (free_me);
}
