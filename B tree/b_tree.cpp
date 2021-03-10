#include "b_tree.h"

Node init_tree() {
  return get_empty_node();
}

DataType search_data(Node root, KeyType key) {
  Node traveler = root;

  int idx, child_idx;
  while (traveler != NULL) {
    idx = node_has_key(traveler, key);
    if (idx > -1) {
      break;
    }

    child_idx = where_to_go_down(traveler, key);
    traveler = traveler->children[child_idx];
  }

  if (traveler == NULL) {
    return -1;
  } else {
    return traveler->data[idx];
  }
}

int node_has_key(Node node, KeyType key) {
  int result = -1;

  if (node != NULL) {
    for (int i = 0; i < size(node); ++i) {
      if (node->keys[i] == key) {
        result = i;
        break;
      }
    }
  }

  return result;
}

void insert_data(Node root, KeyType key, DataType value) {
  // need to insert to the root.
  if (node_has_key(root, key) > -1) {
    return;
  }

  if (is_leaf(root)) {
    if (!is_full(root) && node_has_key(root, key) == -1) {
      root = insert_key_to_node(root, key, value);
    } else { // need to split.
      KeyType mid = root->keys[1];
      Node new_root = get_node_with_data(mid, root->data[1]);
      Node left = get_node_with_data(root->keys[0], root->data[0]);
      Node right = get_node_with_data(root->keys[2], root->data[2]);

      if (node_has_key(root, key) == -1) {
        if (key < mid) {
          insert_key_to_node(left, key, value);
        } else {
          insert_key_to_node(right, key, value);
        }
      }

      new_root->children[0] = left;
      new_root->children[1] = right;
      *root = *new_root;
    }
    return;
  }

  int mid;
  if (is_full(root)) {
    mid = root->keys[1];
    Node new_root = get_node_with_data(mid, root->data[1]);
    
    Node left = get_node_with_data(root->keys[0], root->data[0]);
    Node right = get_node_with_data(root->keys[2], root->data[2]);
    for (int i = 0; i < 2; ++i) {
      left->children[i] = root->children[i];
      right->children[i] = root->children[i + 2]; 
    }

    new_root->children[0] = left;
    new_root->children[1] = right;

    *root = *new_root;
  }

  Node parent = root;
  int idx = where_to_go_down(parent, key);
  int idx_in_parent;
  Node child = parent->children[idx];
  while (!is_leaf(parent)) {
    if (node_has_key(parent, key) > -1 || node_has_key(child, key) > -1) {
      return;
    }

    if (is_full(child)) {
      mid = child->keys[1];
      idx_in_parent = where_to_insert(parent, mid);
      split(parent, idx);
      if (key < mid) {
        parent = parent->children[idx_in_parent];
      } else {
        parent = parent->children[idx_in_parent + 1];
      }

      idx = where_to_go_down(parent, key);
      child = parent->children[idx];
    } else {
      idx = where_to_go_down(child, key);
      parent = child;
      if (idx == -1) {
        break;
      }
      child = parent->children[idx];
    }
  }

  insert_key_to_node(parent, key, value);
}

Node insert_key_to_node(Node node, KeyType key, DataType value) {
  if (node_has_key(node, key) > -1) {
    return node;
  }

  int idx = where_to_insert(node, key);
  if (idx == -1) {
    cout << "idx is -1 inserting " << value << endl;
    return NULL;
  } else if (idx == MAX_KEYS + 1) {
    cout << "idx is MAX_KEYS + 1 inserting " << value << endl;
    return NULL;
  } else if (is_full(node)) {
    cout << "node is full inserting " << value << endl;
    return NULL;
  }
 
  for (int i = size(node); i > idx; --i) {
    node->keys[i] = node->keys[i - 1];
    node->data[i] = node->data[i - 1];
  }

  node->keys[idx] = key;
  node->data[idx] = value;
  node->num_of_data++;
  return node;
}

Node split(Node parent, int idx) {
  if (is_full(parent)) {
    return NULL;
  }

  Node child = parent->children[idx];
  KeyType mid = child->keys[1];

  int idx_in_parent = where_to_insert(parent, mid);
  parent = insert_key_to_node(parent, mid, child->data[1]);

  Node left = get_node_with_data(child->keys[0], child->data[0]);
  Node right = get_node_with_data(child->keys[2], child->data[2]);
  for (int i = 0; i < 2; ++i) {
    left->children[i] = child->children[i];
    right->children[i] = child->children[i + 2]; 
  }

  for (int i = size(parent); i >= idx_in_parent + 1; --i) {
    parent->children[i] = parent->children[i - 1];
  }
  
  parent->children[idx_in_parent] = left;
  parent->children[idx_in_parent + 1] = right;

  return parent;
}

Node get_empty_node() {
  Node node = new NodeStruct;
  node->keys = new KeyType[MAX_KEYS];
  node->data = new DataType[MAX_KEYS];
  node->children = new NodeStruct*[MAX_CHILDREN];

  int i;
  for (i = 0; i < MAX_KEYS; ++i) {
    node->keys[i] = -1;
    node->data[i] = -1;
    node->children[i] = NULL;
  }
  node->children[i] = NULL;

  node->num_of_data = 0;
  return node;
}

Node get_node_with_data(KeyType key, DataType value) {
  Node node = new NodeStruct;
  node->keys = new DataType[MAX_KEYS];
  node->data = new DataType[MAX_KEYS];
  node->children = new NodeStruct*[MAX_CHILDREN];

  int i;
  for (i = 0; i < MAX_KEYS; ++i) {
    node->keys[i] = -1;
    node->data[i] = -1;
    node->children[i] = NULL;
  }
  node->children[i] = NULL;

  node->keys[0] = key;
  node->data[0] = value;
  node->num_of_data = 1;
  return node;
}

int where_to_insert(Node node, KeyType key) {
  int idx = -1;
  if (key < get_smallest_key(node)) {
    idx = 0;
  } else if (key > get_largest_key(node)) {
    idx = size(node);
  } else {
    for (int i = 0; i < size(node) - 1; ++i) {
      if (key > node->keys[i] && key < node->keys[i + 1]) {
        idx = i + 1;
        break;
      }
    }
  }

  return idx;
}

int where_to_go_down(Node parent, KeyType key) {
  int idx = -1;
  if (key < get_smallest_key(parent)) {
    idx = 0;
  } else if (key > get_largest_key(parent)) {
    idx = size(parent);
  }

  for (int i = 0; i < size(parent) - 1; ++i) {
    if (key > parent->keys[i] && key < parent->keys[i + 1]) {
      idx = i + 1;
      break;
    }
  }
  return idx;
}

void delete_by_key(Node node, KeyType key) {
  Node left = node->children[0];
  Node right = node->children[1];

  if (size(node) == 1 && size(left) == 1 && size(right) == 1) {
    Node new_node = get_node_with_data(left->keys[0], left->data[0]);
    new_node = insert_key_to_node(new_node, node->keys[0], node->data[0]);
    new_node = insert_key_to_node(new_node, right->keys[0], right->data[0]);

    for (int i = 0; i < 2; ++i) {
      new_node->children[i] = left->children[i];
      new_node->children[i + 2] = right->children[i];
    }
    
    *node = *new_node;
  }

  int idx = index_in_node(node, key);

  if (idx == -1) {
    if (is_leaf(node)) {
      return;
    } else { // need to go down one or more levels.
      idx = where_to_go_down(node, key);
      Node child = node->children[idx];
      
      int idx_in_child = index_in_node(child, key);
      if (size(child) == 1) {
        // steal or merge.
        if (idx != 0 && size(node->children[idx - 1]) > 1) {
          // steal from left sibling.
          steal_from_left(node, idx - 1);
          delete_by_key(node->children[idx], key);
        } else if (idx != size(node) && size(node->children[idx + 1]) > 1) {
          // steal from right sibling.
          steal_from_right(node, idx);
          delete_by_key(node->children[idx], key);
        } else {
          if (idx == size(node)) {
            merge(node, idx - 1);
            delete_by_key(node->children[idx - 1], key);
          } else {
            merge(node, idx);
            delete_by_key(node->children[idx], key);
          }
        }
      } else {
        delete_by_key(child, key);
      }
    }
  } else {
    // found the key in the given node.
    if (is_leaf(node)) {
      delete_in_leaf(node, idx);
    } else {
      delete_in_non_leaf(node, idx); 
    }
  }
}

void delete_in_leaf(Node node, int idx) {
  if (size(node) == 0) {
    return;
  }

  for (int i = idx; i < size(node) - 1; ++i) {
    node->keys[i] = node->keys[i + 1];
    node->data[i] = node->data[i + 1];
  }
  node->num_of_data = size(node) - 1;
}

void delete_in_non_leaf(Node node, int idx) {
  KeyType delete_me = node->keys[idx];

  Node left = node->children[idx];
  Node right = node->children[idx + 1];

  if (size(left) >= 2) { 
    // steal from left child
    KeyAndData keyAndData = get_largest_in_subtree(left, idx);

    KeyType max_key = keyAndData->key;
    node->keys[idx] = max_key;
    node->data[idx] = keyAndData->data;

    delete_by_key(left, max_key);
  } else if (size(right) >= 2) {
    // steal from right child
    KeyAndData keyAndData = get_smallest_in_subtree(right, idx);

    KeyType min_key = keyAndData->key;
    node->keys[idx] = min_key;
    node->data[idx] = keyAndData->data;

    delete_by_key(right, min_key);
  } else { 
    // merge
    merge(node, idx);
    delete_by_key(node, delete_me);
  }
}

void merge(Node node, int idx) {
  Node left = node->children[idx];
  Node right = node->children[idx + 1];

  Node new_node = get_node_with_data(left->keys[0], left->data[0]);
  new_node = insert_key_to_node(new_node, node->keys[idx], node->data[idx]);
  new_node = insert_key_to_node(new_node, right->keys[0], right->data[0]);

  for (int i = idx; i < size(node) - 1; ++i) {
    node->keys[i] = node->keys[i + 1];
    node->data[i] = node->data[i + 1];
  }

  for (int i = 0; i < 2; ++i) {
    new_node->children[i] = left->children[i];
    new_node->children[i + 2] = right->children[i];
  }

  for (int i = idx + 1; i < size(node); ++i) {
    node->children[i] = node->children[i + 1];
  }
  node->children[idx] = new_node;

  node->num_of_data = size(node) - 1;
}

void steal_from_left(Node parent, int idx_in_parent) {
  Node left = parent->children[idx_in_parent];
  Node right = parent->children[idx_in_parent + 1];

  for (int i = size(right); i > 0; --i) {
    right->keys[i] = right->keys[i - 1];
    right->data[i] = right->data[i - 1];
  }
  right->keys[0] = parent->keys[idx_in_parent];
  right->data[0] = parent->data[idx_in_parent];

  if (!is_leaf(right)) {
    for (int i = size(right) + 1; i > 0; --i) {
      right->children[i] = right->children[i - 1];
    }
    right->children[0] = left->children[size(left)];
  }

  parent->keys[idx_in_parent] = left->keys[size(left) - 1];
  parent->data[idx_in_parent] = left->data[size(left) - 1];

  right->num_of_data = size(right) + 1;
  left->num_of_data = size(left) - 1;
}

void steal_from_right(Node parent, int idx_in_parent) {
  Node left = parent->children[idx_in_parent];
  Node right = parent->children[idx_in_parent + 1];

  left->keys[size(left)] = parent->keys[idx_in_parent];
  left->data[size(left)] = parent->data[idx_in_parent];

  if (!is_leaf(left)) {
    left->children[size(left) + 1] = right->children[0];
    for (int i = 0; i < size(right); ++i) {
      right->children[i] = right->children[i + 1];
    }
  }

  parent->keys[idx_in_parent] = right->keys[0];
  parent->data[idx_in_parent] = right->data[0];
  for(int i = 0; i < size(right) - 1; ++i) {
    right->keys[i] = right->keys[i + 1];
    right->data[i] = right->data[i + 1];
  }

  left->num_of_data = size(left) + 1;
  right->num_of_data = size(right) - 1;
}


int size(Node node) {
  if (node == NULL) {
    return 0;
  }
  return node->num_of_data;
}

bool is_leaf(Node node) {
  bool result = true;
  for (int i = 0; i < size(node); ++i) {
    if (node->children[i] != NULL && size(node->children[i]) > 0) {
      result = false;
      break;
    }
  }

  return result;
}

bool is_full(Node node) {
  if (node == NULL) {
    return false;
  }
  return size(node) == MAX_KEYS;
}

KeyType get_largest_key(Node node) {
  return node->keys[size(node) - 1];
}

KeyType get_smallest_key(Node node) {
  return node->keys[0];
}

int index_in_node(Node node, KeyType key) {
  int idx = -1;
  
  if (node != NULL) {
    for (int i = 0; i < size(node); ++i) {
      if (node->keys[i] == key) {
        idx = i;
        break;
      }
    }
  }

  return idx;
}

KeyAndData get_largest_in_subtree(Node node, int idx) {
  KeyAndData return_me = new KeyAndDataStruct;

  Node traveler = node;

  if (!is_leaf(node)) {
    Node parent = node;
    traveler = node->children[size(parent)];
    while (!is_leaf(traveler)) {
      parent = traveler;
      traveler = parent->children[size(parent)];
    }

    Node left = parent->children[size(parent) - 1];
    int parent_size = size(parent);
  }

  int max_idx = size(traveler) - 1;
  KeyType max_key = traveler->keys[max_idx];
  DataType max_data = traveler->data[max_idx];

  return_me->key = max_key;
  return_me->data = max_data;
  
  return return_me;
}

KeyAndData get_smallest_in_subtree(Node node, int idx) {
  KeyAndData return_me = new KeyAndDataStruct;
  
  Node traveler = node;

  if (!is_leaf(node)) {
    Node parent = node;
    traveler = parent->children[0];
    while (!is_leaf(traveler)) {
      traveler = traveler->children[0];
    }

    Node right = parent->children[1];
    int parent_size = size(parent);
  }

  KeyType min_key = traveler->keys[0];
  DataType min_data = traveler->data[0];

  return_me->key = min_key;
  return_me->data = min_data;

  return return_me;
}

void levelorder_traversal(Node root) {
  if (root == NULL) {
    return;
  }

  queue<Node> Q;
  Q.push(root);
  Node node;
  while(!Q.empty()) {
    node = Q.front();
    Q.pop();
    if (node == NULL) {
      break;
    }

    print_node(node);

    for (int i = 0; i <= size(node); ++i) {
      Q.push(node->children[i]);
    }
  }
}

void print_node(Node node) {
  if (node == NULL) {
    return;
  }

  int s = size(node);
  for (int i = 0; i < s; ++i) {
    cout << node->keys[i] << "(" << node->data[i] << ")" << " ";
  }

  cout << " | size: " << s << endl;
}

void print_to_file(KeyType key, DataType value, ofstream& stream, bool exists) {
  if (!exists) {
    stream << key << "\tN/A" << endl;
  } else {
    stream << key << "\t" << value << endl;
  }
}