#include <iostream>
#include <string>

#include "avl_tree.h"
using namespace std;

DataType convert_to_datatype(string);

int main() {
  Node tree;
  string line;
  string insert_input = "./insert.csv";
  string delete_input = "./delete.csv";
  string search_input = "./search(500000).txt";
  string insert_output = "result_insert.csv";
  string delete_output = "result_delete.csv";

  ifstream insert_us(insert_input);
  ifstream delete_us(delete_input);
  ofstream insert_result;
  ofstream delete_result;

  cout << "inserting keys... " << endl;
  DataType key;
  while (insert_us.peek() != EOF) {
    getline(insert_us, line);
    key = convert_to_datatype(line);

    tree = insert_data(tree, key);
  }

  cout << "doing inorder traversal... " << endl;
  insert_result.open(insert_output);
  print_to_file(tree, insert_result);

  cout << "deleting keys... " << endl;
  while (!delete_us.eof()) {
    getline(delete_us, line);

    key = convert_to_datatype(line);
    tree = delete_data(tree, key);
  }

  cout << "doing inorder traversal... " << endl;
  delete_result.open(delete_output);
  print_to_file(tree, delete_result);

  cout << "closing files... " << endl;
  insert_us.close();
  delete_us.close();
  insert_result.close();
  delete_result.close();

  cout << "all done!" << endl;
  cout << "bye!" << endl;

  return 0;
}

DataType convert_to_datatype(string line) {
  return atoi(line.c_str());
}