#include <iostream>
#include <string>
#include <bit>
#include <bitset>
#include <cmath>

using namespace std;

// In dieser Datei wird ein 2D-Array in einem 1D-Array verwendet, für das C feeling

char player_symbols[] = {' ', 'X', 'O'};

int printGrid(int * grid, int size) {
    string output = "";
    for (int i = 0; i < size; i++) {
        if (i % size == 0) {
            output += "---\n";
            continue;
        }
        output += "|numbers|\n";
    }
    cout << output << endl;
    return 0;
}

int* new_array(int size) {
    return new int[size * size];
}

int * new_empty_array(int size) {
    int* arr = new_array(size);
    for (int i = 0; i < size; i++) {
        for(int u = 0; u < size; u++) {
            cout << "accessing i:" << i << " u:" << u << endl;
            arr[i * size + u] = 0;
        }
    }
    return arr;
}

// [xy],[x1],[x2]...
// [y1],[  ],[  ]...
// [y2],[  ],[  ]...
// ...
int get_element_at(int * grid, int size, int x, int y) {
    return grid[y * size + x];
}

// returns -1, when not single,
// else returns the index of the single
int has_single_bit_at(int integer) {
    if (!integer || (integer & (integer - 1))) return -1;
    int count = 0;
    while ((integer >> 1) != 0) {
        count++;
        integer >>= 1;
    }
    return count;
}

// returns the player(-id) who has won
// If no player won, returns 0
int check_won(int * grid, int size) {
    int* verticalIsInLine = new int[size + 2]; // +2 to add the diagonals to this array at the end
    cout << "before for i" << endl;
    for (int i = 0; i < size; i++) { verticalIsInLine[i] = 0; }
    cout << "after for i" << endl;
    int firstDiagonal = 0;
    int seconDiagonal = 0;
    for (int line = 0; line < size; line ++) {
        cout << "line " << line << endl;
        firstDiagonal |= 1 << get_element_at(grid, size, line, line);
        seconDiagonal |= 1 << get_element_at(grid, size, size - line, line);
        int isInLine = 0;
        for (int elem = 0; elem < size; elem++) {
            verticalIsInLine[elem] |= 1 << get_element_at(grid, size, elem, line);
            isInLine |= 1 << get_element_at(grid, size, elem, line);
            // Since both (vertical and horizontal) directions get checked at once, we have to loop all elem
        }
        // If only one player has all the spaces, only one bit should be activated
        // If not, multiple players have placed in that line
        // if 0ed bit is activated, also no player got it (check again, because ther is no way to check, how the previous loop was exited)
        int single_bit = has_single_bit_at(isInLine);
        if (single_bit > 0) return single_bit;
    }
    verticalIsInLine[size +1] = firstDiagonal;
    verticalIsInLine[size +2] = seconDiagonal;

    delete [] verticalIsInLine;
    return 0;
}
int main() {
    for (auto u{0u}; u != 0B1010; ++u) {
        std::cout << "u = " << u << " = " << std::bitset<4>(u);
        cout << " " << has_single_bit_at(u);
        if (has_single_bit_at(u) >= 0)
            std::cout << " = 2^" << std::log2(u) << " (is power of two)";
        std::cout << '\n';
    }
    int * test_grid = new_empty_array(3);
    cout << test_grid << endl << check_won(test_grid, 3);
    printGrid(test_grid, 3);
    delete [] test_grid;
    return 0;
}
