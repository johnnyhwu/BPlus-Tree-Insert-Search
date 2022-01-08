#include <iostream>

using namespace std;

class Node
{
    public:
        int* keys;
        int num_keys;
        Node** child_nodes;
        Node* parent_node;
        bool is_leaf;

        Node(int max_key_num, int max_child_num) {
            keys = new int[max_key_num];
            num_keys = 0;
            child_nodes = new Node*[max_child_num];
        }
};

class BPTree {
    private:
        int max_key_num;
        int max_child_num;
        Node* root;
        bool is_debug;

    public:
        BPTree(int order, bool debug) {
            is_debug = debug;

            if(is_debug) {
                cout<<"<INFO>: BPTree constructor"<<endl;
            }
                

            max_key_num = order-1;
            max_child_num = order;
            root = NULL;

            if(is_debug) {
                cout<<"<INFO>: max_key_num: "<<max_key_num<<endl;
                cout<<"<INFO>: max_child_num: "<<max_child_num<<endl;
            }
        }

        void insertValue(int value) {

            if(is_debug) {
                cout<<"<INFO>: insert value = "<<value<<endl;
            }
            

            // root node is not created
            if(root == NULL) {

                if(is_debug) {
                    cout<<"<INFO>: root node is not created "<<value<<endl;
                }
                
                root = new Node(max_key_num, max_child_num);
                root->keys[0] = value;
                root->num_keys = 1;
                root->is_leaf = true;
            } 
            
            // root node already created
            else {

                if(is_debug) {
                    cout<<"<INFO>: root node is created "<<value<<endl;
                }
                

                // find correct leaf node
                Node* current_node = root;
                Node* current_node_parent;
                
                while(current_node->is_leaf == false) {
                    for(int i=0; i<current_node->num_keys; i++) {
                        if(value < current_node->keys[i]) {
                            current_node_parent = current_node;
                            current_node = current_node->child_nodes[i];
                            break;
                        }

                        if(i == current_node->num_keys-1) {
                            current_node_parent = current_node;
                            current_node = current_node->child_nodes[i+1];
                            break;
                        }
                    }
                }

                if(is_debug) {
                    cout<<"<INFO>: find correct leaf node !"<<endl;
                    cout<<"<INFO> all keys in this node: ";
                    for(int i=0; i<current_node->num_keys; i++) {
                        cout<<current_node->keys[i]<<" ";
                    }
                    cout<<endl;
                }
                

                // find correct position in current node
                // available space in leaf node
                if(current_node->num_keys < max_key_num) {

                    if(is_debug) {
                        cout<<"<INFO>: current leaf node has available space"<<endl;
                    }
                    
                    // find position for new value
                    int idx = 0;
                    while(idx < current_node->num_keys) {
                        if(value < current_node->keys[idx]) {
                            break;
                        }

                        idx++;
                    }

                    // insert it
                    for(int j=current_node->num_keys; j>idx; j--) {
                        current_node->keys[j] = current_node->keys[j-1];
                    }
                    current_node->keys[idx] = value;
                    current_node->num_keys++;

                    // update child nodes pointer
                    current_node->child_nodes[current_node->num_keys] = current_node->child_nodes[current_node->num_keys-1];
                    current_node->child_nodes[current_node->num_keys-1] = NULL;
                } 
                
                // leaf node overflow
                else {
                    if(is_debug) {
                        cout<<"<INFO>: current leaf node overflow"<<endl;
                    }
                    

                    // duplicate all keys in current node
                    int duplicate_keys[max_key_num + 1];
                    for(int i=0; i<max_key_num; i++) {
                        duplicate_keys[i] = current_node->keys[i];
                    }

                    // find position of new value in duplicate keys array
                    int idx = 0;
                    while(idx < max_key_num) {
                        if(value < duplicate_keys[idx]) {
                            break;
                        }
                        idx++;
                    }

                    // insert new value into duplicate keys array
                    for(int j=(max_key_num+1)-1; j>idx; j--) {
                        duplicate_keys[j] = duplicate_keys[j-1];
                    }
                    duplicate_keys[idx] = value;

                    // store keys in duplicate keys array into first leaf node and second leaf node
                    Node* first_leaf_node = current_node;
                    first_leaf_node->num_keys = int((max_key_num+1)/2);

                    Node* second_leaf_node = new Node(max_key_num, max_child_num);
                    second_leaf_node->num_keys = (max_key_num+1) - int((max_key_num+1)/2);
                    second_leaf_node->is_leaf = true;

                    first_leaf_node->child_nodes[first_leaf_node->num_keys] = second_leaf_node;
                    second_leaf_node->child_nodes[second_leaf_node->num_keys] = first_leaf_node->child_nodes[max_key_num];
                    first_leaf_node->child_nodes[max_key_num] = NULL;

                    for(int i=0; i<first_leaf_node->num_keys; i++) {
                        first_leaf_node->keys[i] = duplicate_keys[i];
                    }

                    for(int i=0; i<second_leaf_node->num_keys; i++) {
                        second_leaf_node->keys[i] = duplicate_keys[i+first_leaf_node->num_keys];
                    }

                    if (is_debug)
                    {
                        cout<<"<INFO>: first_leaf_node: ";
                        for(int i=0; i<first_leaf_node->num_keys; i++) {
                            cout<<first_leaf_node->keys[i]<<" ";
                        }
                        cout<<endl;

                        cout<<"<INFO>: second_leaf_node: ";
                        for(int i=0; i<second_leaf_node->num_keys; i++) {
                            cout<<second_leaf_node->keys[i]<<" ";
                        }
                        cout<<endl;

                        cout<<"<INFO>: first_leaf_node & second_leaf_node: ";
                        for(int i=0; i<first_leaf_node->num_keys; i++) {
                            cout<<first_leaf_node->keys[i]<<" ";
                        }
                        Node* temp = first_leaf_node->child_nodes[first_leaf_node->num_keys];
                        for(int i=0; i<temp->num_keys; i++) {
                            cout<<temp->keys[i]<<" ";
                        }
                        cout<<endl;
                    }
                    
                    


                    // build parent for second leaf node
                    if(first_leaf_node == root) {
                        Node* new_root = new Node(max_key_num, max_child_num);
                        new_root->keys[0] = second_leaf_node->keys[0];
                        new_root->num_keys = 1;
                        new_root->is_leaf = false;
                        new_root->child_nodes[0] = first_leaf_node;
                        new_root->child_nodes[1] = second_leaf_node;
                        root = new_root;

                        first_leaf_node->parent_node = new_root;
                        second_leaf_node->parent_node = new_root;
                    } else {
                        insertInternalValue(second_leaf_node->keys[0], current_node_parent, second_leaf_node);
                    }
                }
            }
        }

        void insertInternalValue(int value, Node* current_node, Node* current_node_child) {

            if(is_debug) {
                cout<<"<INFO>: insert internal value "<<value<<endl;
                cout<<"<INFO>: current internal node: ";
                for(int i=0; i<current_node->num_keys; i++) {
                    cout<<current_node->keys[i]<<" ";
                }
                cout<<endl;
            }
            

            // available space for new key
            if(current_node->num_keys < max_key_num) {
                if(is_debug) {
                    cout<<"<INFO>: internal node has available space"<<endl;
                }
                

                // find position for this new key in current node
                int idx = 0;
                while(idx < current_node->num_keys)
                {
                    if(value < current_node->keys[idx]) {
                        break;
                    }
                    idx++;
                }

                // insert this new key
                for (int j = current_node->num_keys; j > idx; j--) {
                    current_node->keys[j] = current_node->keys[j - 1];
                }
    
                for (int j = current_node->num_keys+1; j > idx+1; j--) {
                    current_node->child_nodes[j] = current_node->child_nodes[j - 1];
                }
        
                current_node->keys[idx] = value;
                current_node->num_keys++;
                current_node->child_nodes[idx + 1] = current_node_child;
                current_node_child->parent_node = current_node;
            } else {
                if(is_debug) {
                    cout<<"<INFO>: internal node overflow"<<endl;
                }
                

                // duplicate keys and child_nodes
                int duplicate_keys[max_key_num + 1];
                Node* duplicate_child_nodes[max_child_num + 1];
                
                for(int i=0; i<max_key_num; i++) {
                    duplicate_keys[i] = current_node->keys[i];
                }

                for(int i=0; i<max_child_num; i++) {
                    duplicate_child_nodes[i] = current_node->child_nodes[i];
                }

                // find position of new key in dupliate keys array
                int idx = 0;
                while(idx < max_key_num) {
                    if(value < duplicate_keys[idx]) {
                        break;
                    }
                    idx++;
                }

                // insert this new key into duplicate key array
                for (int j = max_key_num; j > idx; j--) {
                    duplicate_keys[j] = duplicate_keys[j - 1];
                }
                duplicate_keys[idx] = value;

                if(is_debug) {
                    cout<<"<INFO>: duplicate_keys: ";
                    for(int i=0; i<max_key_num + 1; i++) {
                        cout<<duplicate_keys[i]<<" ";
                    }
                    cout<<endl;
                }
                

                // insert this child node into duplicate child node array
                for (int j = max_child_num; j>idx+1; j--) {
                    duplicate_child_nodes[j] = duplicate_child_nodes[j - 1];
                }  
                duplicate_child_nodes[idx + 1] = current_node_child;

                // create first leaf node and second leaf node
                Node* first_leaf_node = current_node;
                first_leaf_node->num_keys = int((max_key_num+1)/2);

                Node* second_leaf_node = new Node(max_key_num, max_child_num);
                second_leaf_node->is_leaf = false;
                second_leaf_node->num_keys = max_key_num - int((max_key_num+1)/2);

                for (int i=0, j=0; i<first_leaf_node->num_keys; i++, j++) {
                    first_leaf_node->keys[i] = duplicate_keys[j];
                }
  
                for (int i=0, j=0; i<first_leaf_node->num_keys+1; i++, j++) {
                    first_leaf_node->child_nodes[i] = duplicate_child_nodes[j];
                }
  
                for (int i=0, j = first_leaf_node->num_keys + 1; i < second_leaf_node->num_keys; i++, j++) {
                    second_leaf_node->keys[i] = duplicate_keys[j];
                }
  
                for (int i=0, j = first_leaf_node->num_keys + 1; i < second_leaf_node->num_keys + 1; i++, j++) {
                    second_leaf_node->child_nodes[i] = duplicate_child_nodes[j];
                }

                if(is_debug) {
                    cout<<"<INFO>: first_leaf_node keys: ";
                    for(int i=0; i<first_leaf_node->num_keys; i++) {
                        cout<<first_leaf_node->keys[i]<<" "; 
                    }
                    cout<<endl;

                    cout<<"<INFO>: second_leaf_node keys: ";
                    for(int i=0; i<second_leaf_node->num_keys; i++) {
                        cout<<second_leaf_node->keys[i]<<" "; 
                    }
                }

                

                // current_node_child->parent_node = second_leaf_node;
                if(idx+1 <= int((max_key_num+1)/2)) {
                    current_node_child->parent_node = first_leaf_node;
                } else {
                    current_node_child->parent_node = second_leaf_node;
                }

                // create parent node for second internal node
                if(first_leaf_node == root) {
                    Node* new_root = new Node(max_key_num, max_child_num);
                    new_root->keys[0] = duplicate_keys[first_leaf_node->num_keys];
                    new_root->child_nodes[0] = first_leaf_node;
                    new_root->child_nodes[1] = second_leaf_node;
                    new_root->is_leaf = false;
                    new_root->num_keys = 1;
                    root = new_root;

                    first_leaf_node->parent_node = new_root;
                    second_leaf_node->parent_node = new_root;
                } else {
                    insertInternalValue(duplicate_keys[first_leaf_node->num_keys], first_leaf_node->parent_node, second_leaf_node);
                }
            }

        }

        void printAllLeafNode() {
            if(root == NULL) {
                cout<<"No any node in tree !"<<endl;
                return;
            }

            Node* current_node = root;
            while(current_node->is_leaf == false) {
                current_node = current_node->child_nodes[0];
            }

            cout<<"All leaf node: "<<endl;
            int idx = 0;
            while(current_node != NULL) {
                cout<<"#"<<idx<<":"<<endl;
                for(int i=0; i<current_node->num_keys; i++) {
                    cout<<current_node->keys[i]<<" ";
                }
                cout<<endl;
                idx++;

                current_node = current_node->child_nodes[current_node->num_keys];
            }
        }

        void preorderTraversal(Node* current_node, int level) {

            // visit

            // tabs
            for(int i=0; i<2*level; i++) {
                cout<<" ";
            }

            // left parentheses
            cout<<"(";

            // all keys
            if(current_node != NULL) {
                cout<<current_node->keys[0];
                for(int i=1; i<current_node->num_keys; i++) {
                    cout<<" "<<current_node->keys[i];
                }
            }

            // right parentheses
            cout<<")";
            cout<<endl;

            // next level
            if(current_node != NULL && current_node->is_leaf == false) {
                for(int i=0; i<=current_node->num_keys; i++) {
                    preorderTraversal(current_node->child_nodes[i], level+1);
                }
            }
            
        }

        void printTree() {
            int level = 0;
            Node* current_node = root;

            preorderTraversal(current_node, level);
            cout<<endl;
        }


        void searchValue(int value) {

            Node* current_node = root;

            while(current_node->is_leaf == false)
            {
                // print node
                // left parentheses
                cout<<"(";

                // all keys
                if(current_node != NULL) {
                    cout<<current_node->keys[0];
                    for(int i=1; i<current_node->num_keys; i++) {
                        cout<<" "<<current_node->keys[i];
                    }
                }

                // right parentheses
                cout<<")";
                cout<<endl;

                // find correct child node
                for(int i=0; i<current_node->num_keys; i++) {
                    if(value < current_node->keys[i]) {
                        current_node = current_node->child_nodes[i];
                        break;
                    }

                    if(i == current_node->num_keys-1) {
                        current_node = current_node->child_nodes[i+1];
                        break;
                    }
                }
            }

            // print node
            // left parentheses
            cout<<"(";

            // all keys
            if(current_node != NULL) {
                cout<<current_node->keys[0];
                for(int i=1; i<current_node->num_keys; i++) {
                    cout<<" "<<current_node->keys[i];
                }
            }

            // right parentheses
            cout<<")";
            cout<<endl;

            // find this value in this leaf node
            bool is_find = false;
            for(int i=0; i<current_node->num_keys; i++) {
                if(current_node->keys[i] == value) {
                    is_find = true;
                    break;
                }
            }

            // print result
            if(is_find) {
                cout<<"Found"<<endl;
            } else {
                cout<<"QAQ"<<endl;
            }
            
            cout<<endl;
        }

        void sequentialAccess(int value, int num) {

            Node* current_node = root;

            while(current_node->is_leaf == false)
            {
                // find correct child node
                for(int i=0; i<current_node->num_keys; i++) {
                    if(value < current_node->keys[i]) {
                        current_node = current_node->child_nodes[i];
                        break;
                    }

                    if(i == current_node->num_keys-1) {
                        current_node = current_node->child_nodes[i+1];
                        break;
                    }
                }
            }

            // find this value in this leaf node
            int idx;
            bool is_find = false;
            for(idx=0; idx<current_node->num_keys; idx++) {
                if(current_node->keys[idx] == value) {
                    is_find = true;
                    break;
                }
            }

            if(is_find) {
                int total = 0;
                bool start = false;
                while(current_node != NULL) {
                    for(int i=0; i<current_node->num_keys; i++) {

                        if(total == num) {
                            break;
                        }

                        if(current_node->keys[i] == value) {
                            cout<<value;
                            total++;
                            start = true;
                            continue;
                        }

                        if(start == true) {
                            cout<<" "<<current_node->keys[i];
                            total++;
                        }
                    }

                    if(total == num) {
                        break;
                    }

                    current_node = current_node->child_nodes[current_node->num_keys];
                }

                cout<<endl;

                if(total < num) {
                    cout<<"N is too large"<<endl;;
                }
            } else {
                cout<<"Access Failed"<<endl;
            }

            cout<<endl;
        }
};

int main() {
    // input order of b+ tree
    int order;
    cin>>order;

    // initialize a b+ tree
    BPTree tree = BPTree(order, false);

    // read command
    char command_type;
    while(true) {
        cin>>command_type;

        // insert
        if(command_type == 'i') {
            int value;
            cin>>value;
            tree.insertValue(value);
        }

        // search
        else if(command_type == 's') {
            int value;
            cin>>value;
            tree.searchValue(value);
        }

        // print
        else if(command_type == 'p')
        {
            // tree.printAllLeafNode();
            tree.printTree();
        }

        // sequential access
        else if(command_type == 'a') {
            int value;
            int num;
            cin>>value>>num;
            tree.sequentialAccess(value, num);
        }

        // quit
        else {
            break;
        }
    }
}