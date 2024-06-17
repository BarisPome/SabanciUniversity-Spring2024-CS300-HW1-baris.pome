// Created by Barış Pome on 14.03.2024.
//baris.pome_Pome_Baris_HW1.cpp
//Sabancı University - Data Structures and Algorithms - CS300
//Barış Pome 31311 CS300 HW1
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

struct Cell { // Cell structure for the maze
    int x, y; // Coordinates
    bool visited; // Visited flag
    bool walls[4]; // Left, Up, Right, Down
    Cell(int x = 0, int y = 0) : x(x), y(y), visited(false) {
        for (bool& wall : walls) wall = true;
    }
};

template<typename T> // Templated Stack class
class Stack { // Templated Stack class
private:
    struct StackNode { // Node structure
        T data;
        StackNode* next;
        StackNode(T data, StackNode* next = nullptr) : data(data), next(next) {}
    };
    StackNode* top = nullptr; // Top of the stack
public:
    Stack() { //default constructor
        top = nullptr;
    }
    ~Stack() { // Destructor
        while (!isEmpty()) {
            pop();
        }
    }
    void push(T data) { //the function for add an element to the top of the stack
        top = new StackNode(data, top);
    }
    T pop() { //the function for remove the top element of the stack
        if (!isEmpty()) { // Check if the stack is empty
            StackNode* node = top; // Store the top node
            T data = top->data;
            top = top->next;
            delete node;
            return data;
        }
    }
    T peek() { //the function for return the top element of the stack
        if (!isEmpty()) { // Check if the stack is empty
            return top->data;
        }
    }
    bool isEmpty() const { //the function for check if the stack is empty
        return top == nullptr;
    }
};

class Maze { //The class for the maze
private:
    int row, col; //size of the maze
    vector<vector<Cell> > Cells; //2D vector of cells
public:
    Maze(int M, int N) : row(M), col(N) { //constructor
        Cells.resize(M, vector<Cell>(N));
    }
    vector<Cell*> getUnvisitedNeighbors(Cell* current) { //Get the unvisited neighbors of the cell
        int x = current->x;
        int y = current->y;
        vector<Cell*> neighbors; //vector of unvisited neighbors
        //check if the cell is valid and unvisited
        if (x + 1 < col && !Cells[y][x + 1].visited) { // Right
            Cells[y][x + 1].x = x + 1; // Set the x coordinate of the cell
            Cells[y][x + 1].y = y; // Set the y coordinate of the cell
            neighbors.push_back(&Cells[y][x + 1]); // Add the cell to the neighbors vector
        }
        if (x - 1 >= 0 && !Cells[y][x - 1].visited) { // Left
            Cells[y][x - 1].x = x - 1;  // Set the x coordinate of the cell
            Cells[y][x - 1].y = y; // Set the y coordinate of the cell
            neighbors.push_back(&Cells[y][x - 1]); // Add the cell to the neighbors vector
        }
        if (y + 1 < row && !Cells[y + 1][x].visited) { // Down
            Cells[y + 1][x].x = x; // Set the x coordinate of the cell
            Cells[y + 1][x].y = y + 1; // Set the y coordinate of the cell
            neighbors.push_back(&Cells[y + 1][x]); // Add the cell to the neighbors vector
        }
        if (y - 1 >= 0 && !Cells[y - 1][x].visited) { // Up
            Cells[y - 1][x].x = x; // Set the x coordinate of the cell
            Cells[y - 1][x].y = y - 1; // Set the y coordinate of the cell
            neighbors.push_back(&Cells[y - 1][x]); // Add the cell to the neighbors vector
        }
        return neighbors; // Return the neighbors vector
    }
    void removeWall(Cell* current, Cell* next) { //The function for remove the wall between the current cell and the next cell

        int x_diff = next->x - current->x; // Difference in x
        int y_diff = next->y - current->y; // Difference in y

        if (x_diff == 0 && y_diff == 1) { // Down
            current->walls[1] = false;  // Remove the wall of the current cell
            next->walls[3] = false;  // Remove the wall of the next cell
        } else if (x_diff == 0 && y_diff == -1) {  // Up
            current->walls[3] = false; // Remove the wall of the current cell
            next->walls[1] = false; // Remove the wall of the next cell
        }
        if (x_diff == -1 && y_diff == 0) { // Left
            current->walls[0] = false; // Remove the wall of the current cell
            next->walls[2] = false; // Remove the wall of the next cell
        } else if (x_diff == 1 && y_diff == 0) { // Right
            current->walls[2] = false; // Remove the wall of the current cell
            next->walls[0] = false; // Remove the wall of the next cell
        }
    }
    void generateMaze() { //The function for generate the maze
        random_device rd; // Obtain a random number from hardware
        mt19937 gen(rd()); // Seed the generator

        Stack<Cell*> CellStack;
        Cell* start = &Cells[0][0]; // Start from the top-left cell
        start->visited = true; // Mark the start cell as visited
        CellStack.push(start); // Push the start cell to the stack

        while (!CellStack.isEmpty()) { // While the stack is not empty
            Cell* current = CellStack.peek(); // Get the top element of the stack
            vector<Cell*> neighbors = getUnvisitedNeighbors(current); // Get the unvisited neighbors of the current cell

            if (!neighbors.empty()) { // If there are unvisited neighbors
                uniform_int_distribution<> dis(0, neighbors.size() - 1);
                Cell* next = neighbors[dis(gen)];
                removeWall(current, next);
                next->visited = true;
                CellStack.push(next);
            } else { // If there are no unvisited neighbors
                CellStack.pop(); // Pop the top element of the stack
            }
        }
    }
    void printMazeFile(ofstream& outFile) { //// Left, Up, Right, Down //The function for print the maze to a file
        outFile << row << " " << col << "\n"; // Print the dimensions of the maze
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                outFile << "x=" << j << " y=" << i << " l=" << Cells[i][j].walls[0] << " r=" << Cells[i][j].walls[2] << " u=" << Cells[i][j].walls[1] << " d=" << Cells[i][j].walls[3]<< "\n";
                //make each cell unvisited for the path finding
                Cells[i][j].visited = false; // Mark the cell as unvisited for the path finding
            }
        }
        outFile.close(); // Close the file
    }
    bool isExit(Cell* current, int x_exit, int y_exit) { //The function for check if the current cell is the exit cell
        return current->x == x_exit && current->y == y_exit;
    }

    bool isEntry(Cell* current, int x_entry, int y_entry) { //The function for check if the current cell is the entry cell
        return current->x == x_entry && current->y == y_entry;
    }

    vector<Cell*> getNeighbors(Cell* current) { // Left, Up, Right, Down
        vector<Cell*> neighbors;
        // Adjusted to reflect accurate wall checking as per the second snippet
        //print the info of current
        int x = current->x; // Get the x coordinate of the current cell
        int y = current->y; // Get the y coordinate of the current cell
        if (x + 1 < col && current->walls[2]) { // Right // Check if the right wall is open
            if (!Cells[y][x + 1].visited) { // Check if the right cell is unvisited
                neighbors.push_back(&Cells[y][x + 1]); // Add the right cell to the neighbors vector
            }
        }
        if (y + 1 < row && current->walls[1] ) { // Up // Check if the up wall is open
            if (!Cells[y + 1][x].visited) { // Check if the up cell is unvisited
                neighbors.push_back(&Cells[y + 1][x]); // Add the up cell to the neighbors vector
            }
        }
        if (y >= 1) { //down // Check if the down wall is open
            if (current->walls[3]) { // Down
                if (!Cells[y - 1][x].visited) { // Check if the down cell is unvisited
                    neighbors.push_back(&Cells[y - 1][x]); // Add the down cell to the neighbors vector
                }
            }
        }
        if (x - 1 >= 0 && current->walls[0]) { // Left // Check if the left wall is open
            if (!Cells[y][x - 1].visited) { // Check if the left cell is unvisited
                neighbors.push_back(&Cells[y][x - 1]); // Add the left cell to the neighbors vector
            }
        }
        return neighbors; // Return the neighbors vector
    }

    void MazeSolver(int mazeID) { //The function for find the path from the entry point to the exit point
        random_device rd; // Obtain a random number from hardware
        mt19937 gen(rd()); // Seed the generator

        // Get the entry and exit points from the user
        int x_entry, y_entry, x_exit, y_exit; // Entry and exit points
        cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
        cin >> x_entry >> y_entry; // Get the entry point from the user
        cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
        cin >>  x_exit >> y_exit; // Get the exit point from the user
        // Create the file name
        string filename = "maze_" + to_string(mazeID) + "_path_" + to_string(x_entry) + "_" + to_string(y_entry) + "_" + to_string(x_exit) + "_" + to_string(y_exit) + ".txt";
        ofstream outFile(filename);
        if (!outFile.is_open()) { // Check if the file is open
            cout << "Unable to open file";
            return;
        }
        //finding the path by using Depth First Search algorithm
        Stack<Cell*> pathStack; // Stack for the path
        pathStack.push(&Cells[y_entry][x_entry]); // Push the entry cell to the stack
        Cells[y_entry][x_entry].visited = true; // Mark the entry cell as visited

        //change the wall values of the all cells - if it is false it must be true and if it is true it must be false
        for (int i = 0; i < row; i++) { // Loop through the rows
            for (int j = 0; j < col; j++) { // Loop through the columns
                for (int k = 0; k < 4; k++) { // Loop through the walls
                    Cells[i][j].walls[k] = !Cells[i][j].walls[k]; // Change the wall value
                }
            }
        }

        while (!isExit(pathStack.peek(), x_exit, y_exit)) { // While the top element of the stack is not the exit cell
            Cell* current = pathStack.peek(); // Get the top element of the stack
            vector<Cell*> neighbors = getNeighbors(current); // Get the neighbors of the current cell
            if (neighbors.size() > 0) { // If there are neighbors
                uniform_int_distribution<int> dis(0, neighbors.size() - 1);
                Cell* next = neighbors[dis(gen)];
                pathStack.push(next); // Push the next cell to the stack
                pathStack.peek()->visited = true;
            } else { // If there are no neighbors
                if (!isEntry(pathStack.peek(), x_entry, y_entry)) { // If the top element of the stack is not the entry cell
                    pathStack.pop();
                }
            }
        }
        // Print the path to the file
        vector<Cell*> reversePath; // Vector for the reverse path for the format in the hw description
        while (!pathStack.isEmpty()) {
            Cell* current = pathStack.pop(); // Pop the top element of the stack
            reversePath.push_back(current); // Push elements into the vector
        }
        // The elements are now in reverse order in the vector, write them to the file
        for (int i = reversePath.size() - 1; i >= 0; i--) { // Loop through the reverse path
            outFile << reversePath[i]->x << " " << reversePath[i]->y << "\n"; // Print the x and y coordinates of the cell
        }
        outFile.close(); // Close the file
    }
};

int main() {
    //maze generation
    vector<Maze> mazes;
    int K, M, N;
    cout << "Enter the number of mazes: "; // Get the number of mazes from the user
    cin >> K; // Get the number of mazes from the user
    cout << "Enter the number of rows and columns (M and N): "; // Get the number of rows and columns from the user
    cin >> M >> N; // Get the number of rows and columns from the user
    for (int i = 1; i <= K; i++) { // Loop through the number of mazes
        string filename = "maze_" + to_string(i) + ".txt"; // Create the file name
        ofstream outFile(filename); // Create the file
        if (!outFile.is_open()) { // Check if the file is open
            cout << "Unable to open file";
            return 1;
        }
        Maze maze(M, N); // Create the maze
        maze.generateMaze(); // Generate the maze
        maze.printMazeFile(outFile); // Print the maze to the file
        mazes.push_back(maze); // Add the maze to the mazes vector
    }
    cout << "All mazes are generated." << endl << endl; // Print the message
    //path finding
    int mazeID; // Maze ID
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: "; // Get the maze ID from the user
    cin >> mazeID; // Get the maze ID from the user
    mazes[mazeID - 1].MazeSolver(mazeID); // Find the path for the maze
    return 0;
}

