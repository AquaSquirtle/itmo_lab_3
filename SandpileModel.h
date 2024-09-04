#include "ParseArgs.h"

struct Grid {
    int16_t max_x = INT16_MIN;
    int16_t min_x = INT16_MAX;
    int16_t max_y = INT16_MIN;
    int16_t min_y = INT16_MAX;
    int16_t y_size {};
    int16_t x_size {};
    uint64_t** grid {};
};

struct Node {
    int j;
    int i;
    Node* next = nullptr;
    Node(int _j, int _i): j(_j), i(_i) {}
};

struct Queue {
    Node* first = nullptr;
    Node* last = nullptr;
    bool IsEmpty() {
        return first == nullptr;
    }
    void Push (int j, int i) {
        Node* p = new Node(j, i);
        if (IsEmpty()) {
            first = p;
            last = p;
            return;
        }
        last->next = p;
        last = p;
    }
    Node* PopFirst () {
        Node* temp = first;
        first = first->next;
        return temp;
    }
};

void ExpandBot (Grid* grid);

void ExpandTop (Grid* grid);

void ExpandLeft (Grid* grid);

void ExpandRight (Grid* grid);

void Toppling (Grid* grid, int y_coord, int x_coord);

void Sandpile (Options* arguments, Grid* grid);

void CreateGrid (Options* arguments, Grid* grid);