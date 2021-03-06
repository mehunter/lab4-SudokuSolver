/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/*                                     */
/* Program to solve sudoku puzzles if  */
/* they are 'well-formed'.             */
/***************************************/

/* Set DEBUG to FALSE to perform per spec.  Set to TRUE to run in debug mode */
#define DEBUG FALSE
#define TRUE 1
#define FALSE 0

#include <stdio.h>

/* function declarations */
int readPuzzle(int puzzle[9][9]);
int findRowErrors(int puzzle[9][9]);
int findColErrors(int puzzle[9][9]);
int findBoxErrors(int puzzle[9][9]);
int solvePuzzle(int puzzle[9][9], int index);
int isSafe(int puzzle[9][9], int index, int num);
int noSolution(int puzzle[9][9]);
void writePuzzle(int puzzle[9][9]);

int main (void)
{
  int puzzle[9][9];
  int index = 0;
  int error;

  while ((error = readPuzzle(puzzle)) != EOF)
  {
    error += findRowErrors(puzzle);
    error += findColErrors(puzzle);
    error += findBoxErrors(puzzle);

    if (error) printf("Error\n\n");
    else
    {
      /* in DEBUG mode, show initial puzzle in standard sudoku form */
      if (DEBUG) writePuzzle(puzzle);
      solvePuzzle(puzzle, index);
      if (!noSolution(puzzle)) writePuzzle(puzzle);
    }
  }
  return 0;
}


/* readPuzzle() function fills in the array puzzle[][] with sudoku values from 
 * standard in.  If it finds a value that is not . or 1-9, then it returns
 * TRUE as an error code.  If a puzzle consists of some quantity other than
 * 81 entries, it returns TRUE as an error code.  If there are no errors
 * in the quantity or range of values, it returns FALSE.  
 * As required by the spec, puzzle is echoed to standard out regardless of
 * whether it contains errors or not.
 */
int readPuzzle(int puzzle[9][9])
{
  int i, num, row, col;
  int error = FALSE;

  for (i = 0; (num = getchar()) != '\n'; i++)
    {
      if (num == EOF) return EOF;
      putchar(num);
      if ((num < '1' || num > '9') && (num != '.')) error = TRUE;
      if (num == '.') num = '0';

      row = (i / 9) % 9;
      col = i % 9;
      puzzle[row][col] = num - '0';
    }
  putchar('\n');
  if (i != 81) error = TRUE;
  return error;
}


/* findRowErrors() function checks to see if there is a duplicate entry in any
 * row.  If there is, it returns TRUE to signal an error, otherwise returns
 * FALSE for no error.
 */
int findRowErrors(int puzzle[9][9])
{
  int row, col, i;
  
  /* check rows */
  for (row = 0; row < 9; row++)
  {
    for (col = 0; col < 9; col++)
    {
      for (i = col + 1; i < 9; i++)
      {
        if ( (puzzle[row][col] != 0) && (puzzle[row][col] == puzzle[row][i]) )
        {
          return TRUE;                      /* row error found in puzzle */
        }
      }
    }
  }
  return FALSE;
}


/* findColErrors() function checks to see if there is a duplicate entry in any
 * column.  If there is, it returns TRUE to signal an error, otherwise returns
 * FALSE for no error.
 */
int findColErrors(int puzzle[9][9])
{
  int row, col, i;
  for (col = 0; col < 9; col++)
  {
    for (row = 0; row < 9; row++)
    {
      for (i = row + 1; i < 9; i++)
      {
        if ( (puzzle[row][col] != 0) && (puzzle[row][col] == puzzle[i][col]) )
        {
          return TRUE;                      /* column error found in puzzle */
        }
      }
    }
  }
  return FALSE;
}

/* findBoxErrors() function checks to see if there is a duplicate entry in any
 * box.  If there is, it returns TRUE to signal an error, otherwise returns
 * FALSE for no error.
 */
int findBoxErrors(int puzzle[9][9])
{
  int row, col, i, j;
  for (row = 0; row < 9; row += 3)
  {
    for (col = 0; col < 9; col += 3)
    {
      for (i = 0; i < 9; i++)
      {
        for (j = i + 1; j < 9; j++)
        {
          if ( (puzzle[row + i / 3][col + i % 3] != 0) &&
               (puzzle[row + i / 3][col + i % 3] ==
                puzzle[row + j / 3][col + j % 3]) )
            {
              return TRUE;                  /* box error found in puzzle */
            }
        }
      }
    }
  }
  return FALSE;
}


/* noSolution() function checks to see if the puzzle that came back from
 * solvePuzzle doesn't have any 0's, which means that no solution was
 * found.  If the puzzle does not have a solution, it prints "No solution"
 * and returns TRUE.  Otherwise, it returns FALSE.
 */
int noSolution(int puzzle[9][9])
{
  int row, col;
  for (row = 0; row < 9; row++)
  {
    for (col = 0; col < 9; col++)
    {
      if (!puzzle[row][col])
      {
        printf("No solution\n\n");
        return TRUE;
      }
    }
  }
  return FALSE;
}


/* writePuzzle() function writes out the sudoku puzzle from the array 
 * puzzle[][].
 * If DEBUG is TRUE, then it writes out the puzzle in standard sudoku form. 
 * If DEBUG is FALSE, it writes out the puzzle in the form required by the spec.
 */
void writePuzzle(int puzzle[9][9])
  {
    int row, col;
    for (row = 0; row < 9; row++)
      {
        if (DEBUG) printf("\n");
        if ((DEBUG) && (row == 3 || row == 6))
        {
          printf("----------------------\n");
        }
        for (col = 0; col < 9; col++)
          {
            if (DEBUG) printf(" ");
            if (puzzle[row][col]) printf("%d", puzzle[row][col]);
            else printf(".");
            if ((DEBUG) && (col == 2 || col == 5)) printf(" |");
          }
      }
    printf("\n\n");
  }


/* solvePuzzle() function is the recursive function at the heart of solving
 * the sudoku puzzles once they have been screened for errors.
 */
int solvePuzzle(int puzzle[9][9], int index)
{
  int num;
  int row = index / 9;
  int col = index % 9;

  if (index == 81) return TRUE;                 /* all cells are filled */
  
  if (puzzle[row][col] != 0)
  {
    return solvePuzzle(puzzle, ++index);        /* recursive call */
  }

  else
  {
    for (num = 1; num <= 9; num++)
    {
      if (isSafe(puzzle, index, num))
      {
        puzzle[row][col] = num;
        if (solvePuzzle(puzzle, index)) return TRUE;
        puzzle[row][col] = 0;
      }
    }
    return FALSE;    
  }
}


/* isSafe() function is a helper function to solvePuzzle which determines 
 * whether num is a safe number to put into the puzzle at the index.
 */
int isSafe(int puzzle[9][9], int index, int num)
{
  int i, j;
  int row = index / 9;
  int col = index % 9;
  int boxStartRow = row - (row % 3);
  int boxStartCol = col - (col % 3);

  /* check if num okay to use in row */
  for (i = 0; i < 9; i++)
  {
    if (puzzle[row][i] == num)
    {
      return FALSE;                       /* num already used in row */
    }
  }
          
  /* check if num okay to use in col */
  for (i = 0; i < 9; i++)
  {
    if (puzzle[i][col] == num)
      {
        return FALSE;                   /* num already used in column */
      }
  }
 
  /* check if num okay to use in box */
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (puzzle[boxStartRow + i][boxStartCol + j] == num)
      {
        return FALSE;                     /* num already used in box */
      }
    }
  }

  /* num not already used in row, col or box.  Returns TRUE. */
  return TRUE;
}
