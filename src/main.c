#include <stdio.h>

/*
 * Matrix Calculator - final academic submission (STEPS 1-4)
 * --------------------------------------------------
 * Terminal-based calculator using fixed-size 2D arrays.
 * Supports addition, subtraction, multiplication,
 * transpose and determinant (up to 10 x 10).
 *
 * Compilation (GCC):
 *   gcc -Wall -Wextra -std=c99 src/main.c -o matrix_calculator.exe
 *
 * Running (Windows):
 *   matrix_calculator.exe
 *
 * Standard C only (stdio.h). No external libraries.
 * Portable: no system(), no conio.h, no colors.
 */

/* Fixed maximum size: 10 x 10.
 * Fixed arrays are used because they are easy to explain
 * in a viva, show 2D arrays clearly, and avoid malloc/free. */
#define MAX_SIZE 10

/* Menu and input-helper prototypes */
void displayHeader(void);
void displayOperationHeader(const char *title);
void displayMenu(void);
void displayGoodbye(void);
void printSeparator(void);
void clearInputBuffer(void);
void waitForEnter(void);
int readLine(char *buffer, int size);
int parseInteger(const char *line, int *value);
int readInteger(const char *prompt, int *value);
int readBoundedInt(const char *prompt, int min, int max, int *value);
int readDimensions(const char *name, int *rows, int *cols);

/* Matrix engine prototypes (pure logic, no menu code inside) */
int inputMatrix(const char *name, int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols);
void displayMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols);
void displayLabeledMatrix(const char *label,
                          int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols);
void addMatrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                 int result[MAX_SIZE][MAX_SIZE], int rows, int cols);
void subtractMatrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols);
void multiplyMatrices(int a[MAX_SIZE][MAX_SIZE], int rowsA, int colsA,
                      int b[MAX_SIZE][MAX_SIZE], int rowsB, int colsB,
                      int result[MAX_SIZE][MAX_SIZE]);
void transposeMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols,
                     int result[MAX_SIZE][MAX_SIZE]);
void getMinor(int matrix[MAX_SIZE][MAX_SIZE], int n,
              int skipRow, int skipCol, int minor[MAX_SIZE][MAX_SIZE]);
int determinant(int matrix[MAX_SIZE][MAX_SIZE], int n);

/* Menu operation handlers (input -> validate -> operate -> display).
 * Each returns 1 to stay in the menu loop, 0 when input ended
 * (EOF: main exits cleanly instead of pausing on dead input). */
int handleAddition(void);
int handleSubtraction(void);
int handleMultiplication(void);
int handleTranspose(void);
int handleDeterminant(void);

/* Display the application header (welcome screen). */
void displayHeader(void) {
    printf("\n=============================================\n");
    printf("              MATRIX CALCULATOR\n");
    printf("=============================================\n");
    printf("       C Programming Mini Project\n");
    printf("=============================================\n");
}

/* Display one operation section header, e.g. MATRIX ADDITION. */
void displayOperationHeader(const char *title) {
    int len = 0;
    int pad;
    int i;

    while (title[len] != '\0') {
        len++;
    }
    printf("\n=============================================\n");
    pad = (45 - len) / 2;
    if (pad < 0) {
        pad = 0;
    }
    for (i = 0; i < pad; i++) {
        printf(" ");
    }
    printf("%s\n", title);
    printf("=============================================\n");
}

/* Display the main menu options (prompt is printed by main). */
void displayMenu(void) {
    printf("\n=============================================\n");
    printf("                MAIN MENU\n");
    printf("=============================================\n");
    printf("\n1. Matrix Addition\n");
    printf("2. Matrix Subtraction\n");
    printf("3. Matrix Multiplication\n");
    printf("4. Matrix Transpose\n");
    printf("5. Matrix Determinant\n");
    printf("6. Exit\n");
    printf("\n---------------------------------------------\n");
}

/* Remove leftover characters from stdin after scanf.
 * This handles invalid / non-numeric input safely. */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* Pause until the user presses Enter, then return to menu.
 * Line-based readers leave no stray newline behind, so this
 * waits for a real Enter keypress. */
void waitForEnter(void) {
    int c;

    printf("\nPress Enter to return to the main menu...");
    fflush(stdout);
    c = getchar();
    if (c == EOF) {
        return;
    }
    if (c != '\n') {
        clearInputBuffer();
    }
}

/* Read one full line of text. Returns 1 on success,
 * 0 on end-of-file. The whole line is always consumed,
 * so no leftover newline can skip the next prompt. */
int readLine(char *buffer, int size) {
    int i = 0;
    int c;

    c = getchar();
    if (c == EOF) {
        return 0;
    }
    while (c != '\n' && c != EOF) {
        if (i < size - 1) {
            buffer[i] = (char) c;
            i++;
        }
        c = getchar();
    }
    buffer[i] = '\0';
    return 1;
}

/* Print a thin visual separator between sections. */
void printSeparator(void) {
    printf("\n---------------------------------------------\n");
}

/* Say goodbye and exit cleanly (normal return from main). */
void displayGoodbye(void) {
    printf("\n=============================================\n");
    printf("           Thank you for using\n");
    printf("              MATRIX CALCULATOR\n");
    printf("=============================================\n");
    printf("\nGoodbye!\n");
}

/* Parse a whole line as one integer. Rejects blanks and
 * trailing junk (so "12abc" is invalid). Returns 1 on
 * success, 0 on failure. Uses only simple checks so it
 * stays easy to explain in a viva. */
int parseInteger(const char *line, int *value) {
    int i = 0;
    int sign = 1;
    long number = 0;
    int digits = 0;

    /* skip leading spaces/tabs */
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
    /* optional sign */
    if (line[i] == '+' || line[i] == '-') {
        if (line[i] == '-') {
            sign = -1;
        }
        i++;
    }
    /* digits */
    while (line[i] >= '0' && line[i] <= '9') {
        number = number * 10 + (line[i] - '0');
        digits++;
        i++;
    }
    if (digits == 0) {
        return 0;
    }
    /* skip trailing spaces/tabs; anything else is junk */
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
    if (line[i] != '\0') {
        return 0;
    }
    *value = (int) (sign * number);
    return 1;
}

/* Read one integer with a prompt. Re-prompts on bad input.
 * Returns 1 on success, 0 if input ended (EOF: caller aborts). */
int readInteger(const char *prompt, int *value) {
    char line[64];
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (!readLine(line, sizeof(line))) {
            return 0;
        }
        if (parseInteger(line, value)) {
            return 1;
        }
        printf("[ERROR] Please enter a valid integer.\n");
    }
}

/* Read an integer that must lie between min and max (inclusive).
 * Returns 1 on success, 0 if input ended. */
int readBoundedInt(const char *prompt, int min, int max, int *value) {
    while (1) {
        if (!readInteger(prompt, value)) {
            return 0;
        }
        if (*value >= min && *value <= max) {
            return 1;
        }
        printf("[ERROR] Enter a number between %d and %d.\n", min, max);
    }
}

/* Read and validate rows and columns for one matrix (each 1..MAX_SIZE).
 * Returns 1 on success, 0 if input ended (operation aborts). */
int readDimensions(const char *name, int *rows, int *cols) {
    char prompt[80];

    printf("\nEnter dimensions for Matrix %s.\n", name);
    sprintf(prompt, "Rows (1-%d): ", MAX_SIZE);
    if (!readBoundedInt(prompt, 1, MAX_SIZE, rows)) {
        return 0;
    }
    sprintf(prompt, "Columns (1-%d): ", MAX_SIZE);
    if (!readBoundedInt(prompt, 1, MAX_SIZE, cols)) {
        return 0;
    }
    return 1;
}

/* Read every element using nested loops (row by row).
 * Dimensions are validated before this is called, so we never
 * read outside 0..MAX_SIZE-1. Non-numeric input re-prompts.
 * Returns 1 on success, 0 if input ended early (matrix
 * is left filled with 0 for the missing entries). */
int inputMatrix(const char *name, int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i, j, value;
    char prompt[80];
    printf("\nEnter elements of Matrix %s:\n", name);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            sprintf(prompt, "%s[%d][%d]: ", name, i + 1, j + 1);
            /* readInteger loops until a number (or EOF); on EOF keep 0 */
            if (!readInteger(prompt, &value)) {
                value = 0;
                matrix[i][j] = value;
                return 0;
            }
            matrix[i][j] = value;
        }
    }
    return 1;
}

/* Print a matrix grid only (no title). Caller prints the
 * label first so A, B and results share one routine. */
void displayMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d", matrix[i][j]);
            if (j < cols - 1) {
                printf("  ");
            }
        }
        printf("\n");
    }
}

/* Print a labeled matrix, e.g. "Matrix A:" then the grid. */
void displayLabeledMatrix(const char *label,
                          int matrix[MAX_SIZE][MAX_SIZE],
                          int rows, int cols) {
    printf("\n%s\n", label);
    displayMatrix(matrix, rows, cols);
}

/* C = A + B, element by element. Caller checks equal dimensions. */
void addMatrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                 int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

/* C = A - B, element by element. Caller checks equal dimensions. */
void subtractMatrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE],
                      int result[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

/* C = A x B. A is rowsA x colsA, B is rowsB x colsB.
 * Requires colsA == rowsB; result is rowsA x colsB.
 * Each C[i][j] = sum of A[i][k] * B[k][j] over k. */
void multiplyMatrices(int a[MAX_SIZE][MAX_SIZE], int rowsA, int colsA,
                      int b[MAX_SIZE][MAX_SIZE], int rowsB, int colsB,
                      int result[MAX_SIZE][MAX_SIZE]) {
    int i, j, k;
    (void) rowsB; /* rowsB equals colsA (checked by caller) */
    for (i = 0; i < rowsA; i++) {
        for (j = 0; j < colsB; j++) {
            result[i][j] = 0; /* must reset before accumulating */
            for (k = 0; k < colsA; k++) {
                result[i][j] = result[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

/* Transpose: R x C becomes C x R using result[j][i] = matrix[i][j]. */
void transposeMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols,
                     int result[MAX_SIZE][MAX_SIZE]) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

/* Build the (n-1) x (n-1) minor by copying every element
 * except skipRow and skipCol. Used by determinant(). */
void getMinor(int matrix[MAX_SIZE][MAX_SIZE], int n,
              int skipRow, int skipCol, int minor[MAX_SIZE][MAX_SIZE]) {
    int i, j, mi = 0, mj;
    for (i = 0; i < n; i++) {
        if (i == skipRow) {
            continue;
        }
        mj = 0;
        for (j = 0; j < n; j++) {
            if (j == skipCol) {
                continue;
            }
            minor[mi][mj] = matrix[i][j];
            mj++;
        }
        mi++;
    }
}

/* Recursive cofactor expansion along the first row.
 * Base: 1x1 returns the element; 2x2 uses ad - bc.
 * Larger: alternate sign (+/-) and recurse on each minor. */
int determinant(int matrix[MAX_SIZE][MAX_SIZE], int n) {
    int minor[MAX_SIZE][MAX_SIZE];
    int col, sign, det = 0;

    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    for (col = 0; col < n; col++) {
        getMinor(matrix, n, 0, col, minor);
        sign = (col % 2 == 0) ? 1 : -1; /* + - + - ... */
        det = det + sign * matrix[0][col] * determinant(minor, n - 1);
    }
    return det;
}

/* Addition: both matrices must be identical in size. */
int handleAddition(void) {
    int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];

    int rowsA, colsA, rowsB, colsB;

    displayOperationHeader("MATRIX ADDITION");
    if (!readDimensions("A", &rowsA, &colsA)) return 0;
    if (!readDimensions("B", &rowsB, &colsB)) return 0;

    if (rowsA != rowsB || colsA != colsB) {
        printf("\n[ERROR] Addition requires both matrices");
        printf(" to have the same dimensions.\n");
        printf("\nMatrix A: %d x %d\n", rowsA, colsA);
        printf("Matrix B: %d x %d\n", rowsB, colsB);
        printf("\nPlease enter compatible matrices.\n");
        return 1;
    }
    if (!inputMatrix("A", a, rowsA, colsA)) return 0;
    if (!inputMatrix("B", b, rowsB, colsB)) return 0;
    addMatrices(a, b, c, rowsA, colsA);
    printSeparator();
    printf("                RESULT\n");
    printSeparator();
    displayLabeledMatrix("Matrix A:", a, rowsA, colsA);
    displayLabeledMatrix("Matrix B:", b, rowsB, colsB);
    displayLabeledMatrix("A + B:", c, rowsA, colsA);
    printSeparator();
    return 1;
}

/* Subtraction: same dimension rule as addition. */
int handleSubtraction(void) {
    int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];
    int rowsA, colsA, rowsB, colsB;

    displayOperationHeader("MATRIX SUBTRACTION");
    if (!readDimensions("A", &rowsA, &colsA)) return 0;
    if (!readDimensions("B", &rowsB, &colsB)) return 0;

    if (rowsA != rowsB || colsA != colsB) {
        printf("\n[ERROR] Subtraction requires both matrices");
        printf(" to have the same dimensions.\n");
        printf("\nMatrix A: %d x %d\n", rowsA, colsA);
        printf("Matrix B: %d x %d\n", rowsB, colsB);
        printf("\nPlease enter compatible matrices.\n");
        return 1;
    }
    if (!inputMatrix("A", a, rowsA, colsA)) return 0;
    if (!inputMatrix("B", b, rowsB, colsB)) return 0;
    subtractMatrices(a, b, c, rowsA, colsA);
    printSeparator();
    printf("                RESULT\n");
    printSeparator();
    displayLabeledMatrix("Matrix A:", a, rowsA, colsA);
    displayLabeledMatrix("Matrix B:", b, rowsB, colsB);
    displayLabeledMatrix("A - B:", c, rowsA, colsA);
    printSeparator();
    return 1;
}

/* Multiplication: columns of A must equal rows of B. */
int handleMultiplication(void) {
    int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];
    int rowsA, colsA, rowsB, colsB;

    displayOperationHeader("MATRIX MULTIPLICATION");
    if (!readDimensions("A", &rowsA, &colsA)) return 0;
    if (!readDimensions("B", &rowsB, &colsB)) return 0;

    if (colsA != rowsB) {
        printf("\n[ERROR] Matrix multiplication is not possible.\n");
        printf("\nFor A x B:\n");
        printf("Columns of Matrix A must equal rows of Matrix B.\n");
        printf("\nMatrix A: %d x %d\n", rowsA, colsA);
        printf("Matrix B: %d x %d\n", rowsB, colsB);
        printf("\nPlease enter compatible matrices.\n");
        return 1;
    }
    if (!inputMatrix("A", a, rowsA, colsA)) return 0;
    if (!inputMatrix("B", b, rowsB, colsB)) return 0;
    multiplyMatrices(a, rowsA, colsA, b, rowsB, colsB, c);
    printSeparator();
    printf("                RESULT\n");
    printSeparator();
    displayLabeledMatrix("Matrix A:", a, rowsA, colsA);
    displayLabeledMatrix("Matrix B:", b, rowsB, colsB);
    displayLabeledMatrix("A x B:", c, rowsA, colsB); /* result is rowsA x colsB */
    printSeparator();
    return 1;
}

/* Transpose: any valid R x C matrix becomes C x R. */
int handleTranspose(void) {
    int a[MAX_SIZE][MAX_SIZE], t[MAX_SIZE][MAX_SIZE];
    int rows, cols;

    displayOperationHeader("MATRIX TRANSPOSE");
    if (!readDimensions("A", &rows, &cols)) return 0;
    if (!inputMatrix("A", a, rows, cols)) return 0;
    transposeMatrix(a, rows, cols, t);
    printSeparator();
    printf("                RESULT\n");
    printSeparator();
    displayLabeledMatrix("Original Matrix:", a, rows, cols);
    displayLabeledMatrix("Transpose:", t, cols, rows); /* swapped dimensions */
    printSeparator();
    return 1;
}

/* Determinant: only square matrices (rows == columns). */
int handleDeterminant(void) {
    int a[MAX_SIZE][MAX_SIZE];
    int rows, cols, det;

    displayOperationHeader("MATRIX DETERMINANT");
    if (!readDimensions("A", &rows, &cols)) return 0;

    if (rows != cols) {
        printf("\n[ERROR] Determinant can only be calculated");
        printf(" for a square matrix.\n");
        printf("\nMatrix entered: %d x %d\n", rows, cols);
        printf("\nPlease enter a square matrix.\n");
        return 1;
    }
    if (!inputMatrix("A", a, rows, cols)) return 0;
    det = determinant(a, rows);
    printSeparator();
    printf("           DETERMINANT RESULT\n");
    printSeparator();
    displayLabeledMatrix("Matrix:", a, rows, cols);
    printf("\nDeterminant = %d\n", det);
    printSeparator();
    return 1;
}

int main(void) {
    char line[64];
    int choice;
    int stay;

    while (1) {
        displayHeader();
        displayMenu();

        printf("Enter your choice: ");
        fflush(stdout);
        if (!readLine(line, sizeof(line))) {
            printf("\nNo more input. Exiting.\n");
            return 0;
        }
        if (!parseInteger(line, &choice)) {
            printf("\n[ERROR] Invalid choice.\n");
            printf("Please select an option from 1 to 6.\n");
            continue;
        }

        switch (choice) {
            case 1:
                stay = handleAddition();
                if (!stay) {
                    printf("\nNo more input. Exiting.\n");
                    return 0;
                }
                waitForEnter();
                break;
            case 2:
                stay = handleSubtraction();
                if (!stay) {
                    printf("\nNo more input. Exiting.\n");
                    return 0;
                }
                waitForEnter();
                break;
            case 3:
                stay = handleMultiplication();
                if (!stay) {
                    printf("\nNo more input. Exiting.\n");
                    return 0;
                }
                waitForEnter();
                break;
            case 4:
                stay = handleTranspose();
                if (!stay) {
                    printf("\nNo more input. Exiting.\n");
                    return 0;
                }
                waitForEnter();
                break;
            case 5:
                stay = handleDeterminant();
                if (!stay) {
                    printf("\nNo more input. Exiting.\n");
                    return 0;
                }
                waitForEnter();
                break;
            case 6:
                displayGoodbye();
                return 0;
            default:
                printf("\n[ERROR] Invalid choice.\n");
                printf("Please select an option from 1 to 6.\n");
                break;
        }
    }

    return 0;
}


