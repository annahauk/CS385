/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Anna Hauk, Christopher Kalish
 * Version     : 1.0
 * Date        : 12-7-2023
 * Description : Solve the all pairs shortest paths problem with Floyd’s algorithm. 
 *              For every vertex in the graph, we'll find the shortest path 
 *              (if a path exists) to every other vertex
 * Pledge      : I pledge my Honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <climits>

using namespace std;

/**
 * Taken from previous labs: 
 * Calculates the number of digits in num
*/
int num_digits(long num) {
    if (num == 0) {
        return 1;
    }
    int digits = 0;
    while (num > 0) {
        digits++;
        num /= 10;
    }
    return digits;
}

/**
 * Displays the matrix on the screen formatted as a table.
*/
void display_table(long** const matrix, const string &label, int num_vertices, const bool use_letters = false) {
    cout << label << endl;
    long max_val = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            long cell = matrix[i][j];
            if (cell < LLONG_MAX && cell > max_val) {
                max_val = matrix[i][j];
            }
        }
    }
    int max_cell_width = use_letters ? num_digits(max_val) : num_digits(max(static_cast<long>(num_vertices), max_val));
    cout << ' ';
    for (int j = 0; j < num_vertices; j++) {
        cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
    }
    cout << endl;
    for (int i = 0; i < num_vertices; i++) {
        cout << static_cast<char>(i + 'A');
        for (int j = 0; j < num_vertices; j++) {
            cout << " " << setw(max_cell_width);
            if (matrix[i][j] == LLONG_MAX) {
                cout << "-";
            } else if (use_letters) {
                cout << static_cast<char>(matrix[i][j] + 'A');
            } else {
                cout << matrix[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Initializes each row of the matrix as an array of longs, making all values Long_Max besides the diagonal of zeros
*/
void initialize_matrix(long** matrix, int size, const bool diag = true) {
    for (int i=0; i<size; i++) {
        // Create each inner array and fill it with false
        matrix[i] = new long[size];
        fill(matrix[i], matrix[i]+size, LLONG_MAX);
        if (diag) {
            matrix[i][i] = 0;
        }
    }
}

/**
 * Deletes the matrix from memory
*/
void kill_matrix(long** matrix, int num_vertices) {
    for (int i=0; i<num_vertices; i++) {
        delete [] matrix[i];
    }
    delete [] matrix;
}

void floyd(long** matrix, int num_vertices, long** ivmatrix) {
    for (int k = 0; k < num_vertices; k++) { // intermediate vertex
        for (int i = 0; i < num_vertices; i++) { // start vertex
            for (int j = 0; j < num_vertices; j++) { // end vertex
                if (i != j && k != i && k != j && matrix[i][k] != LLONG_MAX && matrix[k][j] != LLONG_MAX) { // if there is a path from i to k and k to j
                // then check if the path from i to j is shorter than the current path
                    if (matrix[i][k] + matrix[k][j] < matrix[i][j]) { // if it is, update the matrix
                        ivmatrix[i][j] = k; // update the intermediate vertex matrix
                        matrix[i][j] = matrix[i][k] + matrix[k][j]; // update the distance matrix
                    }
                }
            }
        }
    }
}

/**
 * Finds the path from start to end using the intermediate vertex matrix
*/
string find_paths(int start, int end, long** ivmatrix) {
    string str = "";
    if (ivmatrix[start][end] == LLONG_MAX){
        str += (char)('A' + end);
        return str;
    }
    str += find_paths(start, ivmatrix[start][end], ivmatrix) + " -> " + find_paths(ivmatrix[start][end], end, ivmatrix);
    return str;
}

/**
 * Backtracks through the intermediate vertex matrix to find the shortest path from each vertex to each other vertex
*/
void backtrack(long** matrix, long** ivmatrix, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j] == LLONG_MAX) {
                cout << (char)('A' + i) << " -> " << (char)('A' + j) << ", distance: infinity, path: none" << endl;
            } else {
                cout << (char)('A' + i) << " -> " << (char)('A' + j) << ", distance: " << matrix[i][j] << ", path: " << (char)('A' + i);
                if (i != j) {
                    cout << " -> " << find_paths(i, j, ivmatrix);

                    /*long from = i;
                    string path = "";
                    while (ivmatrix[from][j] != LLONG_MAX) {
                        path += " -> ";
                        path += (char)('A' + ivmatrix[from][j]);
                        from = ivmatrix[from][j];
                    }
                    cout << path << " -> " << (char)('A' + j);*/
                }
                cout << endl;
            }
        }
    }
}

/**
 * Main function for inputs, error checking, and function calling
*/
int main(int argc, const char *argv[]) {
    // Make sure the right number of command line arguments exist.
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    // Create an ifstream object.
    ifstream input_file(argv[1]);
    // If it does not exist, print an error message.
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    // Add read errors to the list of exceptions the ifstream will handle.
    input_file.exceptions(ifstream::badbit);
    string line; //line is the contents of the line that is read in
    try {
        int num_vertices;
        unsigned int line_number = 1;
        // Use getline to read in a line.
        // See http://www.cplusplus.com/reference/string/string/getline/
        string message;
        long **matrix;
        while (getline(input_file, line)) { //line_number keeps track of the current line number
            istringstream iss(line);
            //for graph
            string vertex1, vertex2;
            long weight;
            
            if (line_number == 1) {
                // If the data on line 1 is not an integer
                if (!(iss >> num_vertices)) {
                    cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
                    input_file.close();
                    return 1;
                }
                // If the value on line 1 is not between 1 and 26
                if (num_vertices < 1 || num_vertices > 26) {
                    cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
                    input_file.close();
                    return 1;
                }
                matrix = new long*[num_vertices];
                initialize_matrix(matrix, num_vertices);
            } else{ 
                iss.clear();
                // If the line does not fit all 3 values on it or the vertices cannot be stored in strings
                if (line.size() < 5 || !(iss >> vertex1 >> vertex2)) {
                    cerr << "Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
                    input_file.close();
                    kill_matrix(matrix, num_vertices);
                    return 1;
                }
                // If vertexvertex 1 is longer than 1 character or is outside the range of vertices 1 is longer than 1 character or is outside the range of vertices
                if (vertex1.size() > 1 || vertex1[0] > 'A' + num_vertices - 1 || vertex1[0] < 'A') {
                    cerr << "Error: Starting vertex '" << vertex1 << "' on line " << line_number << " is not among valid values A-" << (char)('A' + num_vertices - 1) << "." << endl;
                    input_file.close();
                    kill_matrix(matrix, num_vertices);
                    return 1;
                }
                // If vertex 2 is longer than 1 character or is outside the range of vertices
                if (vertex2.size() > 1 || vertex2[0] > 'A' + num_vertices - 1 || vertex2[0] < 'A') {
                    cerr << "Error: Ending vertex '" << vertex2 << "' on line " << line_number << " is not among valid values A-" << (char)('A' + num_vertices - 1) << "." << endl;
                    input_file.close();
                    kill_matrix(matrix, num_vertices);
                    return 1;
                }
                // If weight is not a positive integer
                if (!(iss >> weight) || weight <= 0) {
                    cerr << "Error: Invalid edge weight '" << line.substr(4) << "' on line " << line_number << "." << endl;
                    input_file.close();
                    kill_matrix(matrix, num_vertices);
                    return 1;
                }
                // Add line to output message - cannot do cout yet in case an error comes later in the file
                //message += to_string(line_number) + ":\t" + line + "\n";
                matrix[vertex1[0]-'A'][vertex2[0]-'A'] = weight;
            }
            ++line_number;
        }
        // Print the message of all the lines
        //cout << message;

        // display the Distance Matrix
        display_table(matrix, "Distance matrix:", num_vertices);

        long** ivmatrix = new long*[num_vertices];
        initialize_matrix(ivmatrix, num_vertices, false);

        floyd(matrix, num_vertices, ivmatrix);
        display_table(matrix, "Path lengths:", num_vertices);
        display_table(ivmatrix, "Intermediate vertices:", num_vertices, true);
        backtrack(matrix, ivmatrix, num_vertices);

        kill_matrix(matrix, num_vertices);
        kill_matrix(ivmatrix, num_vertices);
        
        // Don't forget to close the file.
        input_file.close();
     } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    } 
    return 0;
}