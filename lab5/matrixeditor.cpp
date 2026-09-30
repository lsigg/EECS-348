/*
 * Program: EECS 348 Lab 5 - Matrix Editor
 *
 * Description:
 * This C++ program reads two square integer matrices from text files
 * It displays their sum and product, calculates both diagonal sums,
 * swaps rows and columns, and replaces an element with an entered value
 *
 * Inputs:
 * Two matrix filenames, each containing a size followed by the matrix values
 * Files are expected to contain valid square matrices of the same size,
 * with dimensions from 1 through 100
 * Row and column indices for swapping start at 0
 * The row and column entered for element replacement start at 1
 * An integer value replaces the selected element in Matrix 2
 *
 * Outputs:
 * The original matrices, their sum and product, and both diagonal sums
 * The matrices after row swapping, column swapping, and element replacement
 * Error messages for files that cannot be opened or invalid edit indices
 *
 * Collaborators:
 * Not recorded
 *
 * Other Sources:
 * ChatGPT assisted with diagonal sum output and element replacement examples
 * Codex assisted with comments and formatting based on the AI assignments
 *
 * Author: Logan Sigg
 * Creation Date: 09/29/2026
 * Revision Date: 09/29/2026
 *
 */

/*
 * These libraries provide file handling, terminal I/O, and strings
 */
#include <fstream>
#include <iostream>
#include <string>

/*
 * This statement permits standard-library names to be used without std::
 */
using namespace std;

/*
 * The Matrix class stores a square integer matrix and its operations
 * Only the first size rows and columns of the array are used
 */
class Matrix {
private:
    /* Store the number of rows and columns in the square matrix */
    int size;
    /* Reserve storage for matrices with up to 100 rows and columns */
    int data[100][100];

public:
    /* Construct an empty matrix and initialize all array entries to zero */
    Matrix() {
        size = 0;
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                data[i][j] = 0;
            }
        }
    }

    /* Read the dimension and row-by-row values from a valid matrix file */
    bool readFromFile(const string& filePath) {
        ifstream file(filePath);

        /* Report an open failure so main can stop processing */
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            return false;
        }

        /* The first integer gives the dimension of the square matrix */
        file >> size;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                file >> data[i][j];
            }
        }

        file.close();
        return true;
    }

    /* Print each matrix row on its own line without modifying the data */
    void display() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    /* Add corresponding entries of two matrices with the same dimensions */
    Matrix operator+(const Matrix& other) const {
        Matrix result;
        result.size = size;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }

        return result;
    }

    /* Multiply two matrices with the same dimensions and return the result */
    Matrix operator*(const Matrix& other) const {
        Matrix result;
        result.size = size;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result.data[i][j] = 0;
                /* Compute the dot product of this row and the other column */
                for (int k = 0; k < size; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }

        return result;
    }

    /* Return both diagonal sums through reference parameters */
    void sumOfDiagonals(int& mainDiagonal, int& secondaryDiagonal) const {
        mainDiagonal = 0;
        secondaryDiagonal = 0;

        for (int i = 0; i < size; i++) {
            /* Main runs top-left to bottom-right; secondary runs top-right to bottom-left */
            mainDiagonal += data[i][i];
            secondaryDiagonal += data[i][size - 1 - i];
        }
    }

    /* Exchange two rows using indices that start at zero */
    void swapRows(int row1, int row2) {
        if (row1 < 0 || row1 >= size || row2 < 0 || row2 >= size) {
            cout << "Invalid row indices." << endl;
            return;
        }

        /* Exchange the entries in each column of the selected rows */
        for (int j = 0; j < size; j++) {
            int temp = data[row1][j];
            data[row1][j] = data[row2][j];
            data[row2][j] = temp;
        }
    }

    /* Exchange two columns using indices that start at zero */
    void swapColumns(int col1, int col2) {
        if (col1 < 0 || col1 >= size || col2 < 0 || col2 >= size) {
            cout << "Invalid column indices." << endl;
            return;
        }

        /* Exchange the entries in each row of the selected columns */
        for (int i = 0; i < size; i++) {
            int temp = data[i][col1];
            data[i][col1] = data[i][col2];
            data[i][col2] = temp;
        }
    }

    /* Replace one entry with a new value using indices that start at zero */
    void swapElement(int row, int col, int value) {
        if (col < 0 || col >= size || row < 0 || row >= size) {
            cout << "Invalid matrix indices." << endl;
            return;
        }
        data[row][col] = value;
    }
};

/*
 * Read the matrices and demonstrate arithmetic, diagonal sums, and editing
 * Return 1 if an input file cannot be opened, or 0 after completing the edits
 */
int main() {
    string file1, file2;
    int row, col, value;

    /* Get the paths to the two matrix input files */
    cout << "Enter first matrix file: ";
    cin >> file1;

    cout << "Enter second matrix file: ";
    cin >> file2;

    Matrix mat1, mat2;

    /* Load both matrices before performing any operations */
    if (!mat1.readFromFile(file1) || !mat2.readFromFile(file2)) {
        return 1;
    }

    /* Display the original matrices for comparison with the results */
    cout << "\nMatrix 1:" << endl;
    mat1.display();

    cout << "\nMatrix 2:" << endl;
    mat2.display();

    /* Use the overloaded addition operator to add the matrices */
    Matrix sum = mat1 + mat2;
    cout << "\nSum of matrices:" << endl;
    sum.display();

    /* Use the overloaded multiplication operator to multiply the matrices */
    Matrix product = mat1 * mat2;
    cout << "\nProduct of matrices:" << endl;
    product.display();

    /* Calculate and print each diagonal sum of Matrix 1 separately */
    int mainDiagonal, secondaryDiagonal;
    mat1.sumOfDiagonals(mainDiagonal, secondaryDiagonal);
    cout << "\nSum of main diagonal of Matrix 1: " << mainDiagonal << endl;
    cout << "Sum of secondary diagonal of Matrix 1: " << secondaryDiagonal << endl;

    /* Read zero-based row indices and swap those rows in Matrix 1 */
    int row1, row2;
    cout << "\nEnter two rows to swap in Matrix 1: ";
    cin >> row1 >> row2;

    mat1.swapRows(row1, row2);
    cout << "\nMatrix 1 after swapping rows:" << endl;
    mat1.display();

    /* Read zero-based column indices and swap those columns in Matrix 2 */
    int col1, col2;
    cout << "\nEnter two columns to swap in Matrix 2:" << endl;
    cin >> col1 >> col2;

    mat2.swapColumns(col1, col2);
    cout << "\nMatrix 2 after swapping columns:" << endl;
    mat2.display();

    /* Read a one-based position and the replacement value for Matrix 2 */
    cout << "Enter row and column (starting at 1):";
    cin >> row >> col;

    cout << "Enter a Value:";
    cin >> value;

    /* Convert the entered position to zero-based array indices */
    mat2.swapElement(row - 1, col - 1, value);

    cout << "\nMatrix 1 after updating the element:" << endl;
    mat2.display();

    return 0;
}
