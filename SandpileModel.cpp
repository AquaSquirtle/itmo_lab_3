#include "SandpileModel.h"

void ExpandBot (Grid* grid){
    uint64_t** temp = new uint64_t* [grid->y_size + 1];
    for (int i = 0; i < grid->y_size + 1; ++i) {
        temp[i] = new uint64_t [grid->x_size]{};
    }
    for (int i = 0; i < grid->y_size; ++i) {
        for (int j = 0; j < grid->x_size; ++j) {
            temp[i][j] = grid->grid[i][j];
        }
    }
    for (int i = 0; i < grid->y_size; ++i) {
        delete[] grid->grid[i];
    }
    delete[] grid->grid;
    grid->grid = temp;
    ++grid->y_size;
}

void ExpandTop (Grid* grid){
    uint64_t** temp = new uint64_t* [grid->y_size + 1];
    for (int i = 0; i < grid->y_size + 1; ++i) {
        temp[i] = new uint64_t [grid->x_size]{};
    }
    for (int i = 1; i < grid->y_size + 1; ++i) {
        for (int j = 0; j < grid->x_size; ++j) {
            temp[i][j] = grid->grid[i - 1][j];
        }
    }
    for (int i = 0; i < grid->y_size; ++i) {
        delete[] grid->grid[i];
    }
    delete[] grid->grid;
    grid->grid = temp;
    ++grid->y_size;
}

void ExpandLeft (Grid* grid){
    uint64_t** temp = new uint64_t* [grid->y_size];
    for (int i = 0; i < grid->y_size; ++i) {
        temp[i] = new uint64_t [grid->x_size + 1]{};
    }
    for (int i = 0; i < grid->y_size; ++i) {
        for (int j = 1; j < grid->x_size + 1; ++j) {
            temp[i][j] = grid->grid[i][j - 1];
        }
    }
    for (int i = 0; i < grid->y_size; ++i) {
        delete[] grid->grid[i];
    }
    delete[] grid->grid;
    grid->grid = temp;
    ++grid->x_size;
}

void ExpandRight (Grid* grid){
    uint64_t** temp = new uint64_t* [grid->y_size];
    for (int i = 0; i < grid->y_size; ++i) {
        temp[i] = new uint64_t [grid->x_size + 1]{};
    }
    for (int i = 0; i < grid->y_size; ++i) {
        for (int j = 0; j < grid->x_size; ++j) {
            temp[i][j] = grid->grid[i][j];
        }
    }
    for (int i = 0; i < grid->y_size; ++i) {
        delete[] grid->grid[i];
    }
    delete[] grid->grid;
    grid->grid = temp;
    ++grid->x_size;
}

void Toppling (Grid* grid, int y_coord, int x_coord, int& move_j, int& move_i){
    if (x_coord == grid->x_size - 1) {
        ExpandRight(grid);
    }
    if (y_coord == grid->y_size - 1) {
        ExpandBot(grid);
    }
    if (x_coord == 0) {
        ++x_coord;
        ++move_i;
        ExpandLeft(grid);
    }
    if (y_coord == 0) {
        ++y_coord;
        ++move_j;
        ExpandTop(grid);
    }
    grid->grid[y_coord][x_coord + 1] += 1;
    grid->grid[y_coord][x_coord - 1] += 1;
    grid->grid[y_coord + 1][x_coord] += 1;
    grid->grid[y_coord - 1][x_coord] += 1;
    grid->grid[y_coord][x_coord] -= 4;
}


void Sandpile (Options* arguments, Grid* grid) {
    int cur_iteration = 0;
    int cur_saved_picture = 1;
    Queue list;
    while (true) {
        if (arguments->max_iter_option != 0) {
            if (arguments->max_iter_option == cur_iteration) {
                return;
            }
        }
        if (arguments->freq_option != 0) {
            if (cur_iteration % arguments->freq_option == 0) {
                ToBmp image(grid->x_size, grid->y_size);
                std::string path = arguments->output_option + "\\image"  + std::to_string(cur_saved_picture) + ".bmp";
                image.Export(path, grid->grid);
                std::cout << path << '\n';
                ++cur_saved_picture;
            }
        }
        for (int y = 0; y < grid->y_size; ++y) {
            for (int x = 0; x < grid->x_size; ++x) {
                if (grid->grid[y][x] > 3) {
                    list.Push(y, x);
                }
            }
        }
        if (list.IsEmpty()) {
            break;
        }
        int move_j = 0;
        int move_i = 0;
        while (list.first != nullptr) {
            Node* temp = list.PopFirst();
            Toppling(grid, temp->j + move_j, temp->i + move_i, move_j, move_i);
            delete temp;
        }
        ++cur_iteration;
    }
}

void CreateGrid (Options* arguments, Grid* grid) {
    int16_t x;
    int16_t y;
    uint64_t grains;
    while (!arguments->input_option.eof()) {
        arguments->input_option >> x >> y >> grains;
        if (grid->max_x < x) {
            grid->max_x = x;
        }
        if (grid->min_x > x) {
            grid->min_x = x;
        }
        if (grid->max_y < y) {
            grid->max_y = y;
        }
        if (grid->min_y > y) {
            grid->min_y = y;
        }
    }
    grid->y_size =  grid->max_y - grid->min_y + 1;
    grid->x_size = grid->max_x - grid->min_x + 1;
    grid->grid = new uint64_t* [grid->y_size];
    for (int i = 0; i < grid->y_size; ++i) {
        grid->grid[i] = new uint64_t [grid->x_size]{};
    }
    arguments->input_option.clear();
    arguments->input_option.seekg(0, arguments->input_option.beg);
    while (!arguments->input_option.eof()) {
        arguments->input_option >> x >> y >> grains;
        grid->grid[y - grid->min_y][x - grid->min_x] = grains;
    }
}