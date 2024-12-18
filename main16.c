#include <stdio.h>
#include <stdlib.h>

// Định nghĩa cấu trúc node của cây AVL
typedef struct Node {
    int data;             // Dữ liệu của node
    struct Node* left;    // Con trái
    struct Node* right;   // Con phải
    int height;           // Chiều cao của node
} Node;

// Hàm tính chiều cao của node
int height(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

// Hàm tính sự chênh lệch chiều cao (Factor)
int balance_factor(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

// Hàm xoay trái
Node* rotate_left(Node* root) {
    Node* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    
    // Cập nhật chiều cao của các node
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    new_root->height = 1 + (height(new_root->left) > height(new_root->right) ? height(new_root->left) : height(new_root->right));

    return new_root;
}

// Hàm xoay phải
Node* rotate_right(Node* root) {
    Node* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    
    // Cập nhật chiều cao của các node
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    new_root->height = 1 + (height(new_root->left) > height(new_root->right) ? height(new_root->left) : height(new_root->right));

    return new_root;
}

// Hàm xoay trái phải (Left-Right rotation)
Node* rotate_left_right(Node* root) {
    root->left = rotate_left(root->left);
    return rotate_right(root);
}

// Hàm xoay phải trái (Right-Left rotation)
Node* rotate_right_left(Node* root) {
    root->right = rotate_right(root->right);
    return rotate_left(root);
}

// Hàm chèn một node vào cây AVL
Node* insert(Node* node, int data) {
    if (node == NULL) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->data = data;
        new_node->left = new_node->right = NULL;
        new_node->height = 1;  // Mới chèn thì chiều cao của node là 1
        return new_node;
    }

    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    } else {
        return node;  // Dữ liệu đã tồn tại trong cây, không chèn lại
    }

    // Cập nhật chiều cao của node
    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));

    // Kiểm tra sự cân bằng và thực hiện các phép xoay nếu cần
    int balance = balance_factor(node);

    // Xoay đơn trái
    if (balance > 1 && data < node->left->data) {
        return rotate_right(node);
    }

    // Xoay đơn phải
    if (balance < -1 && data > node->right->data) {
        return rotate_left(node);
    }

    // Xoay trái phải
    if (balance > 1 && data > node->left->data) {
        return rotate_left_right(node);
    }

    // Xoay phải trái
    if (balance < -1 && data < node->right->data) {
        return rotate_right_left(node);
    }

    return node;
}

// Hàm in cây AVL theo thứ tự duyệt inorder (trái - node - phải)
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main() {
    // Dữ liệu đầu vào
    int values[] = {17, 23, 201, 98, 67, 83, 13, 23, 10, 191, 84, 58};
    int n = sizeof(values) / sizeof(values[0]);

    Node* root = NULL;

    // Chèn các giá trị vào cây AVL
    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }

    // In cây theo thứ tự
    printf("Cay AVL theo thu tu duyet:\n");
    inorder(root);

    return 0;
}
