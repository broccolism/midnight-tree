#ifndef __TREE_H__
#define __TREE_H__
#include <fstream>
using namespace std;
typedef int DataType;

typedef struct NodeStruct_ {
  DataType data;  // all data's gonna be KEY.
  struct NodeStruct_ *left;
  struct NodeStruct_ *right;
  int height;
} NodeStruct;

typedef NodeStruct *Node;

Node init_tree();
Node insert_data(Node, DataType);
Node search_data(Node, DataType);
Node delete_data(Node, DataType);

Node get_empty_node();
Node left_subtree(Node);
Node right_subtree(Node);
void connect_left_subtree(Node, Node);
void connect_right_subtree(Node, Node);

int height(Node);
int new_height(Node);
int get_balance_factor(Node);
Node ll_rotation(Node);
Node rr_rotation(Node);
Node lr_rotation(Node);
Node rl_rotation(Node);
Node rebalance(Node);

void remove_left_subtree(Node);
void remove_right_subtree(Node);
Node get_largest(Node);
Node get_smallest(Node);

void inorder_traversal(Node, void (*func)(Node));
void print_node(Node);
void print_to_file(Node, ofstream &file);
void free_node(Node);

#endif