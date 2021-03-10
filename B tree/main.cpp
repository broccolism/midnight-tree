#include "b_tree.h"

void print_menu();
string get_file_name();
void do_insertion(string, Node);
void do_deletion(string, Node);
void do_search(string, string, Node);
void do_compare(string, string);

int main() {
    Node tree;

    int input_command;
    string insert_file_name, delete_file_name, compare_file_name;
    while (true) {
        print_menu();
        cin >> input_command;

        if (input_command == 3) {
            cout << "bye!" << endl;
            break;
        }

        switch(input_command) {
            case 1:
                tree = get_empty_node();
                insert_file_name = get_file_name();
                do_insertion(insert_file_name, tree);
                do_search(insert_file_name, insert_file_name, tree);
                do_compare(insert_file_name, insert_file_name);
                cout << "ALL DONE with 1. Insertion!" << endl;
                break;
            case 2:
                delete_file_name = get_file_name();
                do_deletion(delete_file_name, tree);
                do_search(delete_file_name, insert_file_name, tree);
                cout << endl;
                cout << "file name to compare? (delete_result/delete_result2) " << endl;
                cin >> compare_file_name;
                do_compare(delete_file_name, compare_file_name);
                cout << "ALL DONE with 2. Deletion!" << endl;
                break;
            default:
                cout << "Oops, you gave me something wrong.." << endl;
        }
    }

    return 0;
}

void print_menu() {
    cout << "=================" << endl;
    cout << "1. Insertion" << endl;
    cout << "2. Deletion" << endl;
    cout << "3. Quit" << endl;
    cout << "=================" << endl;
    cout << "What you wanna do? (number only) ";
}

string get_file_name() {
    string input;
 
    cout << "What's the name of file? (name only) ";
    cin >> input;

    return input;
}

void do_insertion(string insert_file_name, Node tree) {
    string insert_file = "./" + insert_file_name + ".csv";

    cout << "inserting data from " << insert_file << "..." << endl;

    ifstream insert_us(insert_file);

    KeyType key;
    DataType value;
    while (insert_us.peek() != EOF) {
        insert_us >> key >> value;
        insert_data(tree, key, value);
    }
    insert_us.close();
    cout << "done insertion." << endl;
}

void do_deletion(string delete_file_name, Node tree) {
    string delete_file = "./" + delete_file_name + ".csv";

    cout << "deleting data from " << delete_file << "..." << endl;

    ifstream delete_us(delete_file);

    KeyType key;
    DataType value;
    while (delete_us.peek() != EOF) {
        delete_us >> key >> value;
        delete_by_key(tree, key);
    }

    delete_us.close();
    cout << "done deletion." << endl;
}

void do_search(string output_name, string search_file_name, Node tree) {
    string output_file_name = "result_" + output_name + ".csv";

    cout << "searching & printing data to ./" << output_file_name << "..." << endl;

    string search_file = "./" + search_file_name + ".csv";

    ifstream search_us(search_file);
    ofstream search_result;
    search_result.open(output_file_name);

    DataType result;
    bool exists;
    KeyType key;
    DataType value;
    while (search_us.peek() != EOF) {
        search_us >> key >> value;
        result = search_data(tree, key);

        exists = result == -1 ? false : true;
        print_to_file(key, result, search_result, exists);
    }

    search_result.close();
    cout << "done search." << endl;
}

void do_compare(string my_file_name, string name) {
    cout << endl;
    cout << "start to find difference between result_" << my_file_name << ".csv and " << name << ".csv" << endl;

    ifstream file1("result_" + my_file_name + ".csv");
    ifstream file2(name + ".csv");

    int key1, key2;
    string val1, val2;
    while (file1.peek() != EOF) {
        file1 >> key1 >> val1;
        file2 >> key2 >> val2;

        if (key1 != key2) {
            cout << "found difference: " << key1 << endl;
        } else if (val1.compare(val2) != 0) {
            cout << "found difference: " << key1 << " " << val1 << endl;
        }
    }

    cout << "==> done compare. found no difference." << endl;
    cout << endl;
}