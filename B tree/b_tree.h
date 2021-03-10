#ifndef __TREE_H__
#define __TREE_H__

#include <fstream>
#include <iostream>
#include <queue>
#include <cstring>
#include <string>
using namespace std;

#define MAX_KEYS 3
#define MAX_CHILDREN 4
#define MIN_KEYS 1

typedef int KeyType;
typedef int DataType;

typedef struct NodeStruct_ {
  KeyType *keys;
  DataType *data;
  NodeStruct_ **children;
  int num_of_data;
} NodeStruct;
typedef NodeStruct *Node;

typedef struct KeyAndData_ {
  KeyType key;
  DataType data;
} KeyAndDataStruct;
typedef KeyAndDataStruct *KeyAndData;

Node init_tree();

DataType search_data(Node, KeyType);
int node_has_key(Node, KeyType);

void insert_data(Node, KeyType, DataType);
Node insert_key_to_node(Node, KeyType, DataType);
Node split(Node, int);

Node get_empty_node();
Node get_node_with_data(KeyType, DataType);
int where_to_insert(Node, KeyType);
int where_to_go_down(Node, KeyType);

void delete_by_key(Node, KeyType);
void delete_in_leaf(Node, int);
void delete_in_non_leaf(Node, int);
void merge(Node, int);
void steal_from_left(Node, int);
void steal_from_right(Node, int);

int size(Node);
bool is_leaf(Node);
bool is_full(Node);
KeyType get_largest_key(Node);
KeyType get_smallest_key(Node);
int index_in_node(Node, KeyType);
KeyAndData get_largest_in_subtree(Node, int);
KeyAndData get_smallest_in_subtree(Node, int);

void levelorder_traversal(Node);
void print_node(Node);
void print_to_file(KeyType, DataType, ofstream& file, bool);

#endif