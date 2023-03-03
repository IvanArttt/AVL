#include <iostream>
#include <iomanip>

using namespace std;


template <typename T>
class AVLTree {

private:
    struct Node {
        T data;
        int height;
        Node* left;
        Node* right;

        Node(T data) {
            this->data = data;
            this->height = 1;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    Node* root;
    int _size = 0;
    int height(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalance(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* insert(Node* node, T data) {
        ++_size;
        if (node == nullptr) {
            return new Node(data);
        }

        if (data < node->data) {
            node->left = insert(node->left, data);
        }
        else if (data > node->data) {
            node->right = insert(node->right, data);
        }
        else {
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && data < node->left->data) {
            return rightRotate(node);
        }

        if (balance < -1 && data > node->right->data) {
            return leftRotate(node);
        }

        if (balance > 1 && data > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && data < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* deleteNode(Node* root, T data) {
        if (root == nullptr) {
            return root;
        }
        if (data < root->data) {
            root->left = deleteNode(root->left, data);
        }
        else if (data > root->data) {
            root->right = deleteNode(root->right, data);
        }
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else {
                    *root = *temp;
                }
                delete temp;
            }
            else {
                Node* temp = minValueNode(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->data);
            }
        }

        if (root == nullptr) {
            return root;
        }

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }
   
    bool search(T data) {
        return search(root, data);
    }

    void RoundL(Node* node, int& i, T* a)
    {
        if (node)
        {
            RoundL(node->_left, i, a);
            a[i] = node->val;
            ++i;
            RoundL(node->_right, i, a);
        }
    }

    void RoundR(Node* node, int& i, T* a)
    {
        if (node)
        {
            RoundR(node->_right, i, a);
            a[i] = node->val;
            ++i;
            RoundR(node->_left, i, a);
        }
    }
    void PrintTree(Node* node, int indent) const {
        if (node != nullptr) {
            PrintTree(node->right, indent + 4);
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            std::cout << node->data << "\n";
            PrintTree(node->left, indent + 4);
        }
    }
public:
    void Output() const {
        PrintTree(root, 0);
    }
    void RoundRightRootLeft(Node* node) {
        if (node != nullptr) {
            RoundRightRootLeft(node->left);
            cout << node->data << " ";
            RoundRightRootLeft(node->right);
        }
    }
    void RoundLeftRootRight(Node* node) {
        if (node != nullptr) {
            RoundLeftRootRight(node->right);
            cout << node->data << " ";
            RoundLeftRootRight(node->left);
        }
    }
    bool search(Node* node, T data) {
        if (node == nullptr) {
            return false;
        }

        if (node->data == data) {
            return true;
        }

        if (data < node->data) {
            return search(node->left, data);
        }
        else {
            return search(node->right, data);
        }
    }

    

    AVLTree() {
        root = nullptr;
    }
    void insert(T data) {
        if (search(data)) return;
        ++_size;
        root = insert(root, data);
    }

    void deleteNode(T data) {
        if (!search(data)) return;
        --_size;
        root = deleteNode(root, data);
    }

    void Output(const char option)
    {
        T* tmp = new T[_size];
        switch (option)
        {
        case 'L':
            tmp = this->RoundLeftRootRight();
            for (int i = 0; i < _size; i++)
            {
                printf("%d\t", tmp[i]);
            }
            break;
        case 'R':
            tmp = this->RoundRightRootLeft();
            for (int i = 0; i < _size; i++)
            {
                printf("%d\t", tmp[i]);
            }
            break;
        default:
            printf("Wrong input\n");
            break;
        }
        printf("\n");
        delete[] tmp;
    }

};


int main()
{
    AVLTree<int> A;
    for (size_t i = 0; i < 100; i++)
    {
        A.insert(i);
    }
    A.Output();
    return 0;
}