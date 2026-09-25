#include <iostream>
#include <limits>
#include <string>

// Debug:
#include <bitset>
#include <tuple>


using namespace std;

#include <cstdint>
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = intmax_t;
using iptr = intptr_t;

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = uintmax_t;

// In dieser Datei wird ein 2D-Array in einem 1D-Array verwendet, für das C feeling

const usize PLAYERCOUNT = 2;
const char player_symbols[PLAYERCOUNT + 1] = {' ', 'X', 'O'/*, 'I', 'U'*/};

string repeatString(const string & str, usize n) {
    string result;
    for (usize i = 0; i < n; i++) {
        result += str;
    }
    return result;
}

string spacingLinePlus(usize size) {
    return repeatString("+   ", size) + "+\n";
}

usize* new_array(usize size) {
    return new usize[size * size];
}

usize * new_empty_array(usize size) {
    usize* arr = new_array(size);
    for (usize i = 0; i < size; i++) {
        for(usize u = 0; u < size; u++) {
            //cout << "accessing i:" << i << " u:" << u << endl;
            arr[i * size + u] = 0;
        }
    }
    return arr;
}

// [xy],[x1],[x2]...
// [y1],[  ],[  ]...
// [y2],[  ],[  ]...
// ...
// Try to loop all x's first, since this gets read from the cache
inline usize getElementAt(const usize * grid, usize size, usize x, usize y) {
    return grid[y * size + x];
}

inline void writeElementAt(usize * grid, usize size, usize x, usize y, usize value) {
    grid[y * size + x] = value;
}

void printGrid(usize * grid, usize size) {
    string output = "";
    for (usize i = 0; i < size; i++) {
        output += spacingLinePlus(size);
        for (usize u = 0; u < size; u++) {
            output += "  " + std::string(1, player_symbols[getElementAt(grid, size, u, i)]) + " ";
        }
        output += " \n";
    }
    output += spacingLinePlus(size);
    cout << output;
    return;
}

// returns -1, when not single,
// else returns the index of the single
isize has_single_bit_at(usize integer) {
    if (!integer || (integer & (integer - 1))) return -1;
    usize count = 0;
    while ((integer >> 1) != 0) {
        count++;
        integer >>= 1;
    }
    return count;
}

// returns the player(-id) who has won
// If no player won, returns 0
usize check_won(usize * grid, usize size) {
    usize* vertIsInLine = new usize[size + 2]; // +2 to add the diagonals to this array at the end
    for (usize i = 0; i < size; i++) { vertIsInLine[i] = 0; }
    usize firstDiagonal = 0;
    usize seconDiagonal = 0;

    for (usize line = 0; line < size; line ++) {
        //cout << "line " << line << endl;
        firstDiagonal |= 1 << getElementAt(grid, size, line, line);
        seconDiagonal |= 1 << getElementAt(grid, size, size - line - 1, line); // -1 bc the grid has 0-index but size has not
        usize horiIsInLine = 0;
        for (usize elem = 0; elem < size; elem++) {
            vertIsInLine[elem] |= 1 << getElementAt(grid, size, elem, line);
            horiIsInLine |= 1 << getElementAt(grid, size, elem, line);
            // Since both (vertical and horizontal) directions get checked at once, we have to loop all elems
        }
        isize single_bit = has_single_bit_at(horiIsInLine);
        if (single_bit > 0) return single_bit;
    }

    //cout << "Starting evaluating lines and diagonals" << endl;
    vertIsInLine[size] = firstDiagonal;     // no +1, because its 0-index
    vertIsInLine[size+1] = seconDiagonal;   // no +2, because its 0-index
    for (usize i = 0; i < (size +2); i++) {
        isize single_bit = has_single_bit_at(vertIsInLine[i]);
        if (single_bit > 0) return single_bit;
    }

    delete [] vertIsInLine;
    return 0;
}

usize getInputUsize(const string & question) {
    usize out = 0;
    do {
        cout << question;
        cin >> out;
        if (cin.fail()){
            cout << "\"" << out << "\" is invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else break;
        cout << "Try Again" << endl;
    } while (true);
    return out;
}

std::tuple<usize, usize> getCoordsInput(){
    return std::tuple<usize, usize> {
        getInputUsize("x-coord: ")-1,
        getInputUsize("y-coord: ")-1
    };
}

void game() {
    usize winner = 0;
    usize size = 3;
    usize * play_grid = new_empty_array(size);
    usize active_player = PLAYERCOUNT;

    do {
        cout << "Next player: " << player_symbols[active_player] << endl;
    ask_for_coords:
        active_player = !(active_player/PLAYERCOUNT) * active_player + 1; // Increment by one and return to 1 at max player count - cheesy :)
        printGrid(play_grid, size);
        std::tuple<usize, usize> coords = getCoordsInput();
        cout << "(" << std::get<0>(coords) << "," << std::get<1>(coords) << ")" << endl;
        if (std::get<0>(coords) >= size || std::get<1>(coords) >= size) { // should be above 0, since its usize
            cout << "Your input was not in range, try again!" << endl;
            goto ask_for_coords;
        }
        else if (getElementAt(play_grid, size, std::get<0>(coords), std::get<1>(coords)) != 0) {
            cout << "This cell already has an input, try again!" << endl;
            goto ask_for_coords;
        }

        // all inputs are expected to be correct at this point
        writeElementAt(play_grid, size, std::get<0>(coords), std::get<1>(coords), active_player);
        winner = check_won(play_grid, size);
        cout << endl << endl;

    } while (winner == 0);
    cout << "Winner is: " << player_symbols[active_player] << endl;
    printGrid(play_grid, size);

    delete [] play_grid;
}

int main() {
    game();
    cout << "Exiting" << endl;
    return 0;
}
