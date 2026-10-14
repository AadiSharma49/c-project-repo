#include <stdio.h>  /* needed for printf (output) and scanf (input) */

#define MAX 10  /* biggest matrix size allowed; one place to change it */

/* takes the elements of one matrix from the user */
void readMatrix(char name, int m[MAX][MAX], int r, int c) {
    int i, j;  /* i = row counter, j = column counter for the loops */
    printf("Enter elements of matrix %c:\n", name);  /* name is 'A' or 'B' so user knows which one */
    for (i = 0; i < r; i++) {         /* go through every row */
        for (j = 0; j < c; j++) {     /* go through every column in that row */
            scanf("%d", &m[i][j]);    /* read one number and store it at row i, column j */
        }
    }
}

/* prints one matrix in row and column form */
void printMatrix(int m[MAX][MAX], int r, int c) {
    int i, j;  /* loop counters for rows and columns */
    for (i = 0; i < r; i++) {         /* each row */
        for (j = 0; j < c; j++) {     /* each column */
            printf("%d\t", m[i][j]);  /* print the element, \t puts a tab space after it */
        }
        printf("\n");                 /* new line after each row so it looks like a matrix */
    }
}

/* ask rows and columns until they are between 1 and MAX */
void readSize(char name, int *r, int *c) {  /* r and c are pointers so the values go back to main */
    do {
        printf("Enter rows and columns of matrix %c (1-%d): ", name, MAX);
        scanf("%d %d", r, c);  /* r and c are already addresses, so no & needed */
    } while (*r < 1 || *r > MAX || *c < 1 || *c > MAX);  /* ask again if size is out of range */
}

/* res = a + b, element by element */
void add(int a[MAX][MAX], int b[MAX][MAX], int res[MAX][MAX], int r, int c) {
    int i, j;  /* loop counters */
    for (i = 0; i < r; i++)          /* each row */
        for (j = 0; j < c; j++)      /* each column */
            res[i][j] = a[i][j] + b[i][j];  /* add the two elements at the same position */
}

/* res = a - b, element by element */
void subtract(int a[MAX][MAX], int b[MAX][MAX], int res[MAX][MAX], int r, int c) {
    int i, j;  /* loop counters */
    for (i = 0; i < r; i++)          /* each row */
        for (j = 0; j < c; j++)      /* each column */
            res[i][j] = a[i][j] - b[i][j];  /* subtract b's element from a's element */
}

/* res = a x b. r1 = rows of a, c1 = columns of a (= rows of b), c2 = columns of b */
void multiply(int a[MAX][MAX], int b[MAX][MAX], int res[MAX][MAX],
              int r1, int c1, int c2) {
    int i, j, k;  /* i = row of a, j = column of b, k = moves along the row of a and column of b */
    for (i = 0; i < r1; i++) {            /* result has r1 rows */
        for (j = 0; j < c2; j++) {        /* result has c2 columns */
            res[i][j] = 0;                /* start the sum from 0 before adding */
            for (k = 0; k < c1; k++)      /* multiply matching pairs and add them up */
                res[i][j] += a[i][k] * b[k][j];
        }
    }
}

/* res = transpose of a (rows become columns) */
void transpose(int a[MAX][MAX], int res[MAX][MAX], int r, int c) {
    int i, j;  /* loop counters */
    for (i = 0; i < r; i++)          /* each row of a */
        for (j = 0; j < c; j++)      /* each column of a */
            res[j][i] = a[i][j];     /* swap the row and column positions */
}

/* determinant by expanding along the first row (recursive) */
int determinant(int a[MAX][MAX], int n) {  /* n = size of the square matrix */
    int minor[MAX][MAX];  /* smaller (n-1 x n-1) matrix made by removing a row and a column */
    int det = 0;          /* running total of the determinant */
    int sign = 1;         /* + and - alternate for each column: + - + - ... */
    int col, i, j, k;     /* col = column being expanded, i/j = loops over a, k = column index in minor */

    if (n == 1)           /* base case: 1x1 matrix, determinant is the element itself */
        return a[0][0];

    for (col = 0; col < n; col++) {       /* take each element of the first row one by one */
        /* build the minor without row 0 and column col */
        for (i = 1; i < n; i++) {         /* skip row 0, so start from row 1 */
            k = 0;                        /* column position in the minor starts at 0 for every row */
            for (j = 0; j < n; j++) {     /* go through the columns of a */
                if (j == col)             /* skip the column we are expanding on */
                    continue;
                minor[i - 1][k] = a[i][j];  /* copy element; i-1 because row 0 was skipped */
                k++;                      /* move to next column in the minor */
            }
        }
        det += sign * a[0][col] * determinant(minor, n - 1);  /* element x determinant of smaller matrix */
        sign = -sign;                     /* flip the sign for the next column */
    }
    return det;  /* final answer */
}

int main(void) {
    int a[MAX][MAX], b[MAX][MAX], res[MAX][MAX];  /* a and b = input matrices, res = result matrix */
    int r1, c1;   /* rows and columns of matrix A (1 = first matrix) */
    int r2, c2;   /* rows and columns of matrix B (2 = second matrix) */
    int choice;   /* menu option the user picks */

    do {
        printf("\n--- Matrix Calculator ---\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Transpose\n");
        printf("5. Determinant\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {  /* scanf returns 1 only if a number was read */
            break; /* input ended or not a number, so stop the program */
        }

        if (choice >= 1 && choice <= 3) {  /* add, subtract and multiply all need two matrices */
            readSize('A', &r1, &c1);       /* & passes the address so the function can fill r1 and c1 */
            readSize('B', &r2, &c2);

            if (choice != 3 && (r1 != r2 || c1 != c2)) {  /* add/subtract need equal sizes */
                printf("Both matrices must have the same size.\n");
                continue;  /* go back to the menu */
            }
            if (choice == 3 && c1 != r2) {  /* multiply needs columns of A = rows of B */
                printf("Columns of A must be equal to rows of B.\n");
                continue;  /* go back to the menu */
            }

            readMatrix('A', a, r1, c1);  /* sizes are valid now, so take the elements */
            readMatrix('B', b, r2, c2);

            if (choice == 1) {
                add(a, b, res, r1, c1);          /* result has same size as A */
                printf("A + B =\n");
                printMatrix(res, r1, c1);
            } else if (choice == 2) {
                subtract(a, b, res, r1, c1);     /* result has same size as A */
                printf("A - B =\n");
                printMatrix(res, r1, c1);
            } else {
                multiply(a, b, res, r1, c1, c2); /* result is r1 x c2 */
                printf("A x B =\n");
                printMatrix(res, r1, c2);
            }
        } else if (choice == 4) {  /* transpose needs only one matrix */
            readSize('A', &r1, &c1);
            readMatrix('A', a, r1, c1);
            transpose(a, res, r1, c1);
            printf("Transpose =\n");
            printMatrix(res, c1, r1);  /* rows and columns are swapped in the result */
        } else if (choice == 5) {  /* determinant needs one square matrix */
            readSize('A', &r1, &c1);
            if (r1 != c1) {        /* square means rows == columns */
                printf("Determinant needs a square matrix.\n");
                continue;          /* go back to the menu */
            }
            readMatrix('A', a, r1, c1);
            printf("Determinant = %d\n", determinant(a, r1));
        } else if (choice != 6) {  /* anything other than 1-6 is wrong */
            printf("Invalid choice.\n");
        }
    } while (choice != 6);  /* repeat the menu until the user picks Exit */

    return 0;  /* 0 means the program ended normally */
}
