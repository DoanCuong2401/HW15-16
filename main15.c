#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Định nghĩa kiểu dữ liệu cho node
typedef struct Node {
    char title[100];         // Tiêu đề của mục
    int page_number;         // Số trang bắt đầu
    char node_type[20];      // Loại mục: chapter, section, subsection, subsubsection
    struct Node** B;         // Con trỏ tới danh sách các node B (con của node A)
    int num_B;               // Số lượng con B
} Node;

// Hàm tạo một node mới
Node* create_node(const char* title, int page_number, const char* node_type) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->title, title);
    new_node->page_number = page_number;
    strcpy(new_node->node_type, node_type);
    new_node->B = NULL;
    new_node->num_B = 0;
    return new_node;
}

// Hàm thêm con vào node A
void add_B(Node* A, Node* B) {
    A->B = (Node**)realloc(A->B, (A->num_B + 1) * sizeof(Node*));
    A->B[A->num_B] = B;
    A->num_B++;
}

// Hàm tính tổng số chương trong cuốn sách
int count_chapters(Node* A) {
    if (A == NULL) return 0;

    int chapter_count = 0;
    if (strcmp(A->node_type, "chapter") == 0) {
        chapter_count++;
    }

    // Đệ quy tìm số chương trong các B
    for (int i = 0; i < A->num_B; i++) {
        chapter_count += count_chapters(A->B[i]);
    }
    return chapter_count;
}

// Hàm tính tổng số trang của một node
int get_total_pages(Node* A) {
    if (A == NULL) return 0;

    int total_pages = A->page_number;

    // Tính tổng số trang của các B
    for (int i = 0; i < A->num_B; i++) {
        total_pages += get_total_pages(A->B[i]);
    }
    return total_pages;
}

// Hàm tìm chương dài nhất trong cuốn sách
Node* find_longest_chapter(Node* A) {
    if (A == NULL) return NULL;

    Node* longest_chapter = NULL;
    int max_pages = -1;

    if (strcmp(A->node_type, "chapter") == 0) {
        int total_pages = get_total_pages(A);
        if (total_pages > max_pages) {
            max_pages = total_pages;
            longest_chapter = A;
        }
    }

    // Đệ quy tìm trong các B
    for (int i = 0; i < A->num_B; i++) {
        Node* candidate = find_longest_chapter(A->B[i]);
        if (candidate != NULL) {
            int candidate_pages = get_total_pages(candidate);
            if (candidate_pages > max_pages) {
                max_pages = candidate_pages;
                longest_chapter = candidate;
            }
        }
    }
    return longest_chapter;
}

// Hàm tìm và xoá một node theo tên
Node* remove_node(Node* A, const char* title) {
    if (A == NULL) return NULL;

    // Nếu node A này cần xoá, trả về NULL để xoá node A này
    if (strcmp(A->title, title) == 0) {
        for (int i = 0; i < A->num_B; i++) {
            free(A->B[i]);  // Giải phóng bộ nhớ của B
        }
        free(A->B);
        free(A);
        return NULL;
    }

    // Tìm và xoá trong các B
    for (int i = 0; i < A->num_B; i++) {
        A->B[i] = remove_node(A->B[i], title);
    }

    return A;
}

// Hàm giải phóng bộ nhớ cây
void free_tree(Node* A) {
    if (A == NULL) return;

    for (int i = 0; i < A->num_B; i++) {
        free_tree(A->B[i]);
    }
    free(A->B);
    free(A);
}

// Hàm in số trang của các chương còn lại
void print_remaining_pages(Node* A) {
    if (A == NULL) return;

    // Chỉ in số trang cho các chương, bỏ qua các loại khác
    if (strcmp(A->node_type, "chapter") == 0) {
        printf("Chuong: %s, Tong so trang: %d\n", A->title, get_total_pages(A));
    }

    // Đệ quy in số trang của các chương con
    for (int i = 0; i < A->num_B; i++) {
        print_remaining_pages(A->B[i]);
    }
}

int main() {
    // Khởi tạo cây sách
    Node* book = create_node("Book", 1, "book");

    // Thêm các chapter, section, subsection vào cây
    Node* chapter1 = create_node("Chapter 1", 1, "chapter");
    add_B(book, chapter1);

    Node* section1 = create_node("Section 1.1", 5, "section");
    add_B(chapter1, section1);

    Node* subsection1 = create_node("Subsection 1.1.1", 10, "subsection");
    add_B(section1, subsection1);

    Node* chapter2 = create_node("Chapter 2", 20, "chapter");
    add_B(book, chapter2);

    Node* section2 = create_node("Section 2.1", 25, "section");
    add_B(chapter2, section2);

    // Xác định số chương trong sách
    printf("So chuong trong sach: %d\n", count_chapters(book));

    // Tìm chương dài nhất
    Node* longest_chapter = find_longest_chapter(book);
    if (longest_chapter != NULL) {
        printf("Chuong dai nhat: %s, voi tong so trang: %d\n", longest_chapter->title, get_total_pages(longest_chapter));
    }

    // Xoá một mục
    printf("Delete Section 1.1...\n");
    book = remove_node(book, "Section 1.1");

    // In số trang của các chương còn lại sau khi xoá
    printf("So trang cac chuong con lai:\n");
    print_remaining_pages(book);

    // Giải phóng bộ nhớ
    free_tree(book);

    return 0;
}
