#include "SandpileModel.h"

int main (int argc, char* argv[]) {
    Options arguments;
    Grid grid;
    if (GetOptions(argc, argv, &arguments)) {
        CreateGrid(&arguments, &grid);
        Sandpile(&arguments, &grid);
        ToBmp image(grid.x_size, grid.y_size);
        std::string path = arguments.output_option + "\\imageLast.bmp";
        image.Export(path, grid.grid);
    }
}