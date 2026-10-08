# Matrix Calculator

A terminal-based Matrix Calculator written in standard C for a college
C programming submission. It supports Addition, Subtraction,
Multiplication, Transpose, and Determinant for integer matrices up to
10 x 10, through a simple menu-driven terminal interface.

This README is the final handoff document. It describes the code that
actually exists in this repository — no invented functions, files,
or features.

## Overview

The program starts, shows a welcome header and a main menu, and loops
until the user chooses Exit. Each menu choice runs an operation handler
that reads matrix dimensions, validates them, reads matrix elements,
calls a pure math function, displays the result, and pauses before
returning to the menu.

High-level flow:

```text
User
 ↓
Program starts
 ↓
Welcome screen
 ↓
Main menu
 ↓
User selects operation
 ↓
Operation handler
 ↓
Read matrix dimensions
 ↓
Validate dimensions
 ↓
Read matrix elements
 ↓
Perform mathematical operation
 ↓
Display result
 ↓
Wait for user (Enter)
 ↓
Return to main menu
 ↓
User chooses Exit
 ↓
Program terminates
```

## Features

Implemented and tested:

- Matrix Addition (same dimensions required).
- Matrix Subtraction (same dimensions required).
- Matrix Multiplication (columns of A must equal rows of B).
- Matrix Transpose (R x C becomes C x R).
- Matrix Determinant (square matrices only, recursive cofactor).
- Reusable matrix input with per-element prompts (`A[1][1]:`).
- Reusable matrix display in a readable grid.
- Menu validation (1-6) with non-numeric guard, no infinite loops.
- Dimension validation (1-10) with re-prompt.
- Operation compatibility checks with clear `[ERROR]` messages.
- Pause (`Press Enter to return to the main menu...`) that correctly
  handles leftover newlines and EOF.
- Clean exit with goodbye message, `return 0`.
- Portable standard C only (`stdio.h`), GCC `-Wall -Wextra -std=c99`
  with zero warnings.

## Project Structure

Actual repository structure (verified):

```text
d:\c-project\
├── src/
│   └── main.c            # Complete C application (single file)
├── .vscode/
│   ├── tasks.json        # VS Code default build task
│   └── settings.json     # Run console apps in terminal
├── README.md             # This handoff documentation
├── .gitignore            # Ignores build artifacts, .vscode/
└── LICENSE               # MIT License
```

Note: the brief suggested a `matrix-calculator/` wrapper folder. Since
the working rule is to work ONLY inside `d:\c-project`, that folder
itself acts as `matrix-calculator/` — no extra nesting was created.

## File Responsibilities

- `src/main.c` — The entire application: UI, input, validation, math,
  handlers, and `main()`. Single file is intentional (see Why This Design).
- `README.md` — Usage guide, architecture, function reference, testing,
  viva prep.
- `.gitignore` — Ignores `*.exe`, `*.o`, `*.out`, `build/`, `bin/`,
  temp files, OS files, and `.vscode/` so only source and docs are committed.
- `LICENSE` — MIT License, suitable for a personal academic project.
- `.vscode/tasks.json` — Defines the `build` task: `gcc -Wall -Wextra
  -std=c99 src/main.c -o matrix_calculator.exe`. Run with Ctrl+Shift+B.
- `.vscode/settings.json` — Sets `code-runner.runInTerminal: true` so
  keyboard input works (the OUTPUT panel cannot take input).

## Requirements

- GCC (tested with MinGW GCC 6.3.0 on Windows).
- Any normal terminal (Windows cmd / PowerShell / Linux / macOS terminal).
- No external libraries, no GUI, no package installs.

## Installation / Compilation

From the project root (`d:\c-project`):

```sh
gcc -Wall -Wextra -std=c99 src/main.c -o matrix_calculator
```

On Windows the output is `matrix_calculator.exe`; on Linux/macOS it is
`matrix_calculator`. Expect zero errors and zero warnings.

## Running the Program

Windows:

```sh
matrix_calculator.exe
```

Linux / macOS:

```sh
./matrix_calculator
```

Run in a real terminal, not the VS Code OUTPUT panel, so you can type input.

## VS Code Building

The repo provides `.vscode/tasks.json` with a default `build` task that runs
exactly the GCC command above and produces `matrix_calculator.exe` in the
project root. Press `Ctrl + Shift + B` to build. `.vscode/settings.json`
forces Code Runner to use the terminal for input.

## System Architecture

The app uses a simple function-based architecture inside one C file.
There is no multi-file framework — `main.c` is organized in layers:

```text
┌───────────────────────────────┐
│        User / Terminal        │
└───────────────┬───────────────┘
                ↓
┌───────────────────────────────┐
│       Menu / Control Flow     │
│  main(), displayHeader/Menu/  │
│  Goodbye, waitForEnter        │
└───────────────┬───────────────┘
                ↓
┌───────────────────────────────┐
│      Operation Handlers       │
│  handleAddition/Subtraction/  │
│  Multiplication/Transpose/    │
│  Determinant                  │
└───────────────┬───────────────┘
                ↓
┌───────────────────────────────┐
│    Input + Validation Layer   │
│  readLine, parseInteger,      │
│  readInteger, readBoundedInt, │
│  readDimensions, inputMatrix, │
│  clearInputBuffer             │
└───────────────┬───────────────┘
                ↓
┌───────────────────────────────┐
│       Matrix Math Layer       │
│  add/subtract/multiply/       │
│  transpose, getMinor,         │
│  determinant                  │
└───────────────┬───────────────┘
                ↓
┌───────────────────────────────┐
│       Matrix Output           │
│  displayMatrix,               │
│  displayLabeledMatrix,        │
│  printSeparator, headers      │
└───────────────────────────────┘
```

Why one file: this is a small academic project. One source file keeps it
easy to read, shows functions clearly, and separates math from menu logic
without enterprise boilerplate that would be hard to explain in a viva.

## System Flow

1. `main()` loops forever: `displayHeader()` then `displayMenu()`.
2. Menu choice is read with `readLine()` + `parseInteger()` (not bare
   `scanf`), so `abc`, `12abc`, blank lines, and EOF are handled.
3. `switch` dispatches to one `handle*()` function, or prints
   `[ERROR] Invalid choice` and re-shows the menu.
4. The handler prints its section header (`displayOperationHeader`),
   reads dimensions (`readDimensions`), validates compatibility,
   reads elements (`inputMatrix`), calls the pure math function,
   and displays labeled results.
5. `main()` calls `waitForEnter()` so the user can read the result.
6. Choice `6` calls `displayGoodbye()` and returns 0.

```text
main()
 ↓
displayHeader() + displayMenu()
 ↓
readLine() → parseInteger()
 ↓
switch → handleAddition() | handleSubtraction() | handleMultiplication()
         | handleTranspose() | handleDeterminant() | goodbye | error
 ↓
waitForEnter() → loop (or exit on 6 / EOF)
```

## Data Representation

- `#define MAX_SIZE 10` caps every matrix at 10 x 10.
- Every matrix is `int matrix[MAX_SIZE][MAX_SIZE]` — a fixed 2D array.
  Only the top-left `rows x cols` region is used; dimensions travel in
  separate `int rows, cols` variables.
- `int` was chosen because the brief requires integer elements: simple,
  exact, no float formatting, easy viva explanation.
- Fixed arrays were chosen because they demonstrate 2D arrays clearly
  and avoid `malloc`/`free` complexity for a college project.

Data movement:

```text
User Input
    ↓
int matrix[MAX_SIZE][MAX_SIZE] (+ rows, cols)
    ↓
Operation Function (add/subtract/multiply/transpose/determinant)
    ↓
Result Matrix (+ result rows/cols)
    ↓
displayMatrix() / displayLabeledMatrix()
    ↓
Terminal
```

## Operation Details

### Addition

- Input: matrices A and B.
- Requirement: identical dimensions (`rowsA == rowsB` and `colsA == colsB`).
- Processing: `C[i][j] = A[i][j] + B[i][j]` in `addMatrices()`.
- Result: same size as inputs. Handler shows Matrix A, Matrix B, `A + B:`.
- Why the rule: addition is defined element-wise, so each element needs
  a partner at the same position.

### Subtraction

- Input: matrices A and B.
- Requirement: identical dimensions.
- Processing: `C[i][j] = A[i][j] - B[i][j]` in `subtractMatrices()`.
- Result: same size as inputs. Handler shows `A - B:`.
- Same mathematical reason as addition.

### Multiplication

- Input: A is m x n, B is n x p.
- Requirement: `colsA == rowsB`. Result is m x p.
- Processing in `multiplyMatrices()`: zero each `C[i][j]`, then
  `C[i][j] += A[i][k] * B[k][j]` over k with three nested loops.
- Example from tests: A = `1 2 / 3 4`, B = `5 6 / 7 8`.
  `C[0][0] = (1x5) + (2x7) = 5 + 14 = 19`. Full result
  `19 22 / 43 50`. See Testing below.

### Transpose

- Input: any valid R x C matrix.
- Processing in `transposeMatrix()`: `T[j][i] = A[i][j]`.
- Result dimensions swap: display uses `cols x rows`.
- Example: `1 2 3 / 4 5 6` becomes `1 4 / 2 5 / 3 6`.

### Determinant

- Input: square matrix only (`rows == cols`, size n).
- Base cases in `determinant()`: n == 1 returns `matrix[0][0]`;
  n == 2 returns `ad - bc`.
- Recursive case: cofactor expansion along row 0. For each column j,
  build the minor with `getMinor()` (copy all rows except 0 and all
  columns except j), compute `sign * matrix[0][j] * determinant(minor)`,
  alternate sign `+ - + ...` (`j % 2`), and accumulate.
- `getMinor()` constructs the smaller matrix: iterate source rows/cols,
  skip `skipRow`/`skipCol`, pack survivors into `minor[mi][mj]`.
- Recursion: `determinant()` calls itself on successively smaller minors
  until 1x1/2x2. Cofactor expansion is O(n!) — fine for teaching and
  small matrices, expensive for large n (see Complexity).

## Function Reference

Every function in `src/main.c` (27 total). Math functions contain
no menu code; handlers contain no formulas.

### displayHeader()
Welcome banner. No params, no return. Prints MATRIX CALCULATOR header.
Called by: `main()`.
### displayOperationHeader(title)
Section header (MATRIX ADDITION, etc.). Centers title in 45-char banner
with a manual length loop (no string.h). Called by: all 5 handlers.
### displayMenu()
Main menu 1-6 plus divider line. Called by: `main()`.
### displayGoodbye()
Exit screen. Called by: `main()` case 6.
### printSeparator()
Prints the `-----` divider. Called by: handlers, `displayMenu()`.
### clearInputBuffer()
Discards rest of current input line until newline/EOF. Prevents a bad
scanf tail poisoning the next read. Called by: `readInteger()`,
`inputMatrix()`.
### waitForEnter()
Pause after each operation. Handles leftover-newline bug: if a newline
is already waiting it returns at once, otherwise waits for a full line.
EOF returns at once. Called by: `main()`.
### readLine(buffer, size)
Safe line input with fgets. Returns 1 = line read, 0 = EOF/none. Strips
trailing newline; empty reads count as EOF (matters for piped tests).
Called by: `readInteger()`, `main()`.
### parseInteger(line, value)
Strict string-to-int. Rejects `12abc`, `abc`, blanks. Skips whitespace,
optional sign (needs digit after), digits, then only trailing whitespace.
Returns 1 valid / 0 invalid. Called by: `readInteger()`, `main()`.
### readInteger(prompt, value)
Prompt + validated integer with re-prompt. Returns 1 value / 0 EOF.
Invalid prints `[ERROR] Invalid input. Please enter a number.`
Called by: `readBoundedInt()`, `inputMatrix()`.
Calls: `readLine()`, `parseInteger()`, `clearInputBuffer()`.
### readBoundedInt(prompt, min, max, value)
Integer in [min,max]. Rejects out-of-range with `[ERROR] Please enter
a value between min and max.` Called by: `readDimensions()` (1-10).
### readDimensions(name, rows, cols)
Reads rows/cols for Matrix `name`. Prints `Enter dimensions for Matrix A.`
then `Rows (1-10):` / `Columns (1-10):`. Returns 1/0 EOF.
Called by: all 5 handlers. Calls: `readBoundedInt()`.


### inputMatrix(name, matrix, rows, cols)
Fills matrix with per-element prompts (`A[1][1]:`, 1-based for users,
0-based storage). Returns 1/0 EOF. Assumes dims already validated.
Called by: all 5 handlers.

### displayMatrix(matrix, rows, cols)
Prints grid with `%-4d` per element. Called by: `displayLabeledMatrix()`.

### displayLabeledMatrix(label, matrix, rows, cols)
Prints label then grid. Called by: all handlers.

### addMatrices(a, b, result, rows, cols)
Pure addition: `result[i][j] = a[i][j] + b[i][j]`. No I/O.
Called by: `handleAddition()`.

### subtractMatrices(a, b, result, rows, cols)
Pure subtraction: `result[i][j] = a[i][j] - b[i][j]`.
Called by: `handleSubtraction()`.

### multiplyMatrices(a, rowsA, colsA, b, rowsB, colsB, result)
Pure multiplication. Result is rowsA x colsB; caller guarantees
colsA == rowsB. Zeroes each `result[i][j]`, then
`+= a[i][k] * b[k][j]` over k (three nested loops).
Called by: `handleMultiplication()`.

### transposeMatrix(matrix, rows, cols, result)
Pure transpose: `result[j][i] = matrix[i][j]`. Caller displays with
swapped dims. Called by: `handleTranspose()`.

### getMinor(matrix, n, skipRow, skipCol, minor)
Builds (n-1)x(n-1) minor skipping one row/col, packing survivors with
`mi`/`mj` indexes. Keeps `determinant()` readable.
Called by: `determinant()`.

### determinant(matrix, n)
Recursive cofactor determinant, returns int. Base: n==1 returns
`matrix[0][0]`; n==2 returns `ad - bc`; n<=0 returns 0 guard.
Recursive: expand row 0, sign `+ - + ...` via `col % 2`,
`det += sign * matrix[0][col] * determinant(minor)`.
Called by: `handleDeterminant()`, itself.

### handleAddition() / handleSubtraction()
Full add/subtract interaction. Return 1 = stay in menu, 0 = EOF.
Header, readDimensions(A/B), compatibility check
(`rowsA != rowsB || colsA != colsB` shows both sizes), inputMatrix(A/B),
math call, labeled RESULT block.

### handleMultiplication()
Full multiply interaction. Reads dimensions BEFORE elements so bad pairs
are rejected early. Checks `colsA != rowsB` with columns-of-A/rows-of-B
message. Then input, multiply, display.

### handleTranspose()
Any valid size. Header, dims, input, transpose, shows Original and
Transpose (swapped dims).

### handleDeterminant()
Checks `rows != cols` with square-matrix message showing entered size.
Then input, `determinant()`, DETERMINANT RESULT block.

### main()
Menu loop. `while(1)`: header, menu, `Enter your choice:`, `readLine()`
(EOF exits), `parseInteger()` (invalid shows 1-6 error), `switch` to
handlers + `waitForEnter()`, case 6 goodbye, default invalid error.
Returns 0.

## Function Call Flow

Verified against the code:

### Addition / Subtraction

```text
main()
 ↓
menu selection (1 / 2)
 ↓
handleAddition() / handleSubtraction()
 ├── readDimensions() → readBoundedInt() → readInteger()
 │                        → readLine() + parseInteger()
 ├── inputMatrix() → readInteger() + clearInputBuffer()
 ├── addMatrices() / subtractMatrices()
 └── displayLabeledMatrix() → displayMatrix()
```

### Multiplication

```text
main()
 ↓
menu selection (3)
 ↓
handleMultiplication()
 ├── readDimensions() (A and B, BEFORE elements)
 ├── compatibility check (colsA == rowsB)
 ├── inputMatrix() (A and B)
 ├── multiplyMatrices()
 └── displayLabeledMatrix() → displayMatrix()
```

### Transpose

```text
main()
 ↓
menu selection (4)
 ↓
handleTranspose()
 ├── readDimensions()
 ├── inputMatrix()
 ├── transposeMatrix()
 └── displayLabeledMatrix() (original + transposed, swapped dims)
```

### Determinant

```text
main()
 ↓
menu selection (5)
 ↓
handleDeterminant()
 ├── readDimensions()
 ├── square check (rows == cols)
 ├── inputMatrix()
 ├── determinant()
 │    └── getMinor()
 │         └── determinant()   [recursive until 1x1 / 2x2]
 └── displayLabeledMatrix() + determinant value
```

## Complete User Flow

Example — Addition of two 2x2 matrices:

```text
User launches program
 ↓
Welcome screen (displayHeader)
 ↓
Main menu, selects "1"
 ↓
Addition handler starts (MATRIX ADDITION header)
 ↓
Dimensions requested for A (readDimensions)
 ↓
Dimensions requested for B (readDimensions)
 ↓
Dimensions compared (same? else ERROR + menu)
 ↓
Matrix A entered (inputMatrix)
 ↓
Matrix B entered (inputMatrix)
 ↓
addMatrices()
 ↓
Result displayed (Matrix A, Matrix B, A + B)
 ↓
User presses Enter (waitForEnter)
 ↓
Main menu shown again
```

Same pattern for Subtraction and Multiplication (with its own check).
Transpose uses one matrix and shows Original + Transpose. Determinant
uses one matrix, checks square, shows Matrix + Determinant value.

## Input Validation

- Menu: 1-6 only. `0`, `7`, `-1`, `100` show `[ERROR] Invalid choice.
  Please select an option from 1 to 6.` Non-numeric (`abc`, `12abc`,
  blank) shows the same without hanging, thanks to readLine/parseInteger.
- Dimensions: 1-MAX_SIZE (1-10). `0`, `-1`, `11`, `100` show
  `[ERROR] Please enter a value between 1 and 10.` and re-prompt.
  Non-numeric re-prompts with invalid-input error.
- Addition/Subtraction: `rowsA == rowsB && colsA == colsB`, else the
  handler prints both sizes and returns to menu before reading elements.
- Multiplication: `colsA == rowsB`, else explains columns-of-A/rows-of-B.
- Transpose: any valid size.
- Determinant: `rows == cols`, else shows entered size and asks for square.
- EOF anywhere: handlers return 0, `main()` prints `No more input. Exiting.`
  and returns 0 instead of looping on dead input.
- Array safety: loops use validated rows/cols only, never beyond MAX_SIZE.

## Error Handling

- All errors use `[ERROR]` + plain-English guidance, never silent wrong math.
- Menu errors return to menu via `continue`; dimension errors re-prompt
  inside `readBoundedInt()`; compatibility errors return 1 from the handler
  so `main()` pauses and re-shows the menu.
- Element errors re-prompt for that cell only.
- Program recovers from every tested bad input; verified by piping
  `abc`, `0`, `9`, `2x3+2x2`, `2x3*2x2`, non-square determinant.

## Algorithms / Pseudocode

Addition:
```text
for i in 0..rows-1
  for j in 0..cols-1
    C[i][j] = A[i][j] + B[i][j]
```

Subtraction: same with minus.

Multiplication:
```text
for i in 0..rowsA-1
  for j in 0..colsB-1
    C[i][j] = 0
    for k in 0..colsA-1
      C[i][j] += A[i][k] * B[k][j]
```

Transpose:
```text
for i in 0..rows-1
  for j in 0..cols-1
    T[j][i] = A[i][j]
```

Determinant (cofactor along row 0):
```text
if n == 1: return M[0][0]
if n == 2: return M[0][0]*M[1][1] - M[0][1]*M[1][0]
det = 0
for col in 0..n-1
  minor = matrix without row 0 and column col
  sign = (col even) ? +1 : -1
  det += sign * M[0][col] * determinant(minor)
return det
```

## Complexity Analysis

Approximate, for teaching (not exact guarantees):

- Addition: O(R x C). Subtraction: O(R x C). Transpose: O(R x C).
- Multiplication: O(rowsA x colsB x colsA); square n: O(n^3).
- Determinant (recursive cofactor): about O(n!) — each level spawns n
  minors. Fine for 1x1-4x4 teaching cases, expensive for large n.
  That is why MAX_SIZE 10 is a cap, not a target for determinant.

## C Concepts Used

- `#include <stdio.h>` — only header; printf/scanf/fgets/getchar/fflush.
- `#define MAX_SIZE 10` — compile-time constant cap.
- `int` — all elements, dims, determinants, return codes.
- 1D char arrays — `char line[64]` input buffers.
- 2D arrays — `int m[MAX_SIZE][MAX_SIZE]`; only `rows x cols` used.
- Functions + prototypes at top, definitions below (readable order:
  display, input, math, handlers, main).
- Parameters as outputs — `int *rows`, `int *value` out-params.
- Return codes — 1 success / 0 EOF-or-invalid.
- Nested loops — input, display, all math.
- `if/else` validation; `switch` menu dispatch; `while(1)` menu loop
  with `continue`/`return`.
- Recursion — `determinant()` + `getMinor()`; base cases 1x1/2x2.
- I/O + validation layering — parse before use, never trust raw input.
- Modular style — math/IO/handlers separated without globals.

## Example Usage

Real interactive session (addition, then exit):

```text
=============================================
              MATRIX CALCULATOR
=============================================
       C Programming Mini Project
=============================================

=============================================
                MAIN MENU
=============================================

1. Matrix Addition
2. Matrix Subtraction
3. Matrix Multiplication
4. Matrix Transpose
5. Matrix Determinant
6. Exit

---------------------------------------------
Enter your choice: 1

=============================================
               MATRIX ADDITION
=============================================

Enter dimensions for Matrix A.
Rows (1-10): 2
Columns (1-10): 2

Enter dimensions for Matrix B.
Rows (1-10): 2
Columns (1-10): 2

Enter elements of Matrix A:
A[1][1]: 1
A[1][2]: 2
A[2][1]: 3
A[2][2]: 4

Enter elements of Matrix B:
B[1][1]: 5
B[1][2]: 6
B[2][1]: 7
B[2][2]: 8

---------------------------------------------
                RESULT
---------------------------------------------

Matrix A:
1   2
3   4

Matrix B:
5   6
7   8

A + B:
6   8
10  12
---------------------------------------------

Press Enter to return to the main menu...
```

Note: piped test runs do not echo typed input, so prompts and values
appear on shared lines there — interactive use shows them normally.

## Testing

Build (GCC 6.3.0): `gcc -Wall -Wextra -std=c99 src/main.c -o
matrix_calculator` — zero errors, zero warnings. All tests below were
performed by piping deterministic input files (STEP 2 suite) plus a new
STEP 4 rebuild check — no invented results.

| Test | Input | Expected | Result |
|------|-------|----------|--------|
| Addition | A 1 2/3 4, B 5 6/7 8 | 6 8 / 10 12 | PASS |
| Subtraction | A 5 6/7 8, B 1 2/3 4 | 4 4 / 4 4 | PASS |
| Multiplication | same as Addition | 19 22 / 43 50 | PASS |
| Transpose | 1 2 3 / 4 5 6 | 1 4 / 2 5 / 3 6 | PASS |
| Det 1x1 | [7] | 7 | PASS |
| Det 2x2 | 1 2 / 3 4 | -2 | PASS |
| Det 3x3 | 1 2 3 / 0 1 4 / 5 6 0 | 1 | PASS |
| Det 4x4 | tested in STEP 2 | 49 | PASS |
| Add mismatch 2x3+2x2 | dims only | reject with sizes | PASS |
| Sub mismatch 2x3-2x2 | dims only | reject with sizes | PASS |
| Mul mismatch 2x3*2x2 | dims only | reject colsA/rowsB | PASS |
| Det non-square 2x3 | dims only | reject, ask square | PASS |
| Menu 0/9/abc | bad choice | error + menu loop | PASS |
| Dim 0 then valid | 0, then 2x2 | re-prompt, compute | PASS |

Temporary `in_*.txt` files and test executables were deleted after each
suite; only source, docs, and configs remain.

Temporary `in_*.txt` files and test executables were deleted after each
suite; only source, docs, and configs remain.

## Limitations

Intentional scope decisions, not bugs:

- Fixed 10x10 maximum (`MAX_SIZE`).
- Integer values only — no floats; large products/determinants can
  overflow `int`.
- Terminal-only interface; no colors, file I/O, or GUI.
- No inverse, division, eigenvalues, rank, or elimination.
- Recursive determinant is O(n!) — fine for small teaching matrices,
  slow for large ones.

## Future Improvements

Ideas only — NOT implemented:

- Floating-point support, dynamic allocation, matrix inverse/rank,
  file save/load, faster determinant (e.g. LU), multi-file layout,
  GUI version.

## Why This Design?

- One `main.c`: small project stays readable; functions give separation
  without build complexity.
- Fixed `MAX_SIZE`: teaches 2D arrays, avoids malloc/free in viva.
- Integer matrices: exact, simple I/O, matches brief.
- Separate functions + handlers: math stays testable, UI stays thin.
- Validation before calculation: never compute on incompatible input.
- Recursive determinant: cofactor expansion is the easiest correct
  method to explain line-by-line.
- No external libraries: portable GCC build, zero installs.

## Team Member Quick Start

1. Read Overview + Features above.
2. Read System Architecture + System Flow.
3. Open `src/main.c`, find `main()` at the bottom.
4. Trace one handler, e.g. `handleAddition()`.
5. Read its math function (`addMatrices()`).
6. Read `displayMatrix()` / `displayLabeledMatrix()`.
7. Read `readInteger()` / `parseInteger()` / `readBoundedInt()`.
8. Study `determinant()` + `getMinor()` last (recursion is hardest).

## Where to Change What

- Main menu text → `displayMenu()` in `src/main.c`.
- Welcome/exit banners → `displayHeader()` / `displayGoodbye()`.
- Matrix input prompts → `inputMatrix()`; dims → `readDimensions()`.
- Output formatting → `displayMatrix()` / `displayLabeledMatrix()`.
- Addition logic → `addMatrices()`; subtraction → `subtractMatrices()`;
  multiplication → `multiplyMatrices()`; transpose → `transposeMatrix()`;
  determinant → `determinant()` + `getMinor()`.
- Range checks → `readBoundedInt()`; strict number parsing →
  `parseInteger()`; pause → `waitForEnter()`.
- Compatibility rules → the five `handle*()` functions (checks live there,
  not inside math functions).

## Viva Questions

1. What is a matrix? — Numbers in rows x columns; e.g. 2x3.
2. How is a matrix represented in C? — `int m[MAX_SIZE][MAX_SIZE]` plus
   `rows, cols`; only top-left region used.
3. Why a 2D array? — Natural row/column indexing `m[i][j]`.
4. Why MAX_SIZE? — Fixed cap avoids malloc/free, keeps bounds safe.
5. Addition rule? — `C[i][j] = A[i][j] + B[i][j]`, same dims required.
6. Addition condition? — `rowsA == rowsB && colsA == colsB`.
7. Multiplication condition? — `colsA == rowsB`; result rowsA x colsB.
8. Why three loops? — i=row, j=col, k sums `A[i][k]*B[k][j]`.
9. Transpose? — `T[j][i] = A[i][j]`; R x C becomes C x R.
10. Determinant condition? — Square only; else handler rejects.
11. Recursion? — Function calling itself with smaller input + base case.
12. Why recursive determinant? — Cofactor expansion is easy to explain.
13. Minor matrix? — Matrix minus one row and one column (`getMinor()`).
14. Multiplication complexity? — O(rowsA x colsB x colsA), O(n^3) square.
15. Why is recursive determinant expensive? — O(n!): n minors per level.
16. Why functions? — Reuse, thin main, math/UI separation.
17. Why int? — Brief requires it; exact and simple.
18. Input validation? — readLine + strict parseInteger + bounded ranges.
19. Invalid dimensions? — Re-prompt for range; compatibility errors show
   sizes and return to menu without computing.
20. Math separated from menu? — Handlers do I/O+checks; math functions do
   only formulas, so they are readable and testable.

## Author

- Project Team
- Team Members: [Add names before submission]

## License

MIT License — see `LICENSE`.

## Project Status

- STEP 1 (foundation): done.
- STEP 2 (math engine): done, tested.
- STEP 3 (terminal UI): done, tested.
- STEP 4 (this handoff): done — ready for human review and manual
  Git commit/push (Cline never commits or pushes).

