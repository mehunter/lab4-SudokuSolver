/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/***************************************/


#include <stdio.h>


/* This function reads in a sudoku puzzle, line-by-line.  It checks each
 * read character to see if it is valid \n, EOF, '.', or 0 - 9.  If it
 * finds EOF, sets an error bit and returns immediately.  If it finds an 
 * invalid character, it sets an error bit.  It maintains a counter for 
 * the number of characters read to give an error for a puzzle that is too
 * long or too short if true sets an error bit. 
 * 
 * errorCodes:
 * bit 1 - end of file
 * bit 2 - invalid character in puzzle
 * bit 3 - too many characters in puzzle
 * bit 4 - not enough characters in puzzle
 *
 * If no errors, int readPuzzle() will return 0.
 */
int readPuzzle(int puzzle[])
{
  int c, i, errorCode;
  c = i = errorCode = 0;
 
  while ( (c = getchar() ) != '\n' )
    {
      if ( c == EOF )                               /* check for EOF */
        {
          errorCode |= 1;
          return errorCode;                         /* reached EOF, return */
        }
      putchar(c);                                   /* echo puzzle */
      if ( c == '.') c = '0';                       /* assign . to 0 */
      if ( (c < '0') || (c > '9') )
        {
          errorCode |= (1 << 1);                    /* set bit 2 */
        }
      if ( i == 81 )
        {
          errorCode |= (1 << 2);                    /* set bit 3 */
          i -= 81;                                  /* avoid array overflow */
        }
      puzzle[i] = c - '0';                          /* turn c in to number */
      ++i;
    }
  
  putchar('\n');                                    /* done with puzzle echo */

  if ( ( (errorCode & 4) != 4) && (i < 81))         /* not enough characters */
    {
      errorCode |= (1 << 3);                        /* set bit 4 */
    }
 
  return errorCode;
}


/* This function check rows for duplicates, sets bit 5 in errorCode if
 * a duplicate is found.  Returns errorCode.
 */
int validatePuzzleRows(int puzzle[])
{
  int i, j, errorCode;
  i = j = errorCode = 0;

  for (i = 0; i < 81; i += 9)
    {
      for (j = i+1; j < 9; j++)
        {
          if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
            {
              errorCode |= (1 << 4);                /* set bit 5 */
            }
        }
    }
  return errorCode;
}

/* This function checks columns for duplicates, sets bit 6 in errorCode
 * if a duplicate is found.  Returns errorCode.
 */
int validatePuzzleCols(int puzzle[])
{
  int i, j, errorCode;
  i = j = errorCode = 0;
  
  for (i = 0; i < 81; i += 9)
    {
      for (j = i+9; j < 81; j +=9)
        {
          if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
            {
              errorCode |= (1 << 5);               /* set bit 6 */
            }
        }
    }
  return errorCode;
}

/* This function check boxes for duplicates, sets bit 7 in errorCode 
 * if a duplicate is found.  Returns errorCode.  Loop through each box
 * converting the box into a mini-array, then checking the array for
 * duplicates.
 */
int validatePuzzleBoxes(int puzzle[])
{
  int  boxToRow[9];
  int i, j, k, box, boxDelta, errorCode;
  i = j = k = box = boxDelta = errorCode = 0;
  for (box = 0; box < 9; box++)
    {
      boxDelta = ((box % 3) * 3) + ((box / 3) * 27);
      for (k = 0; k < 9; k++)
        {
          boxToRow[k] = puzzle[ ((k / 3) * 9) + (k % 3) + boxDelta];
        }

      for (i = 0; i < 9; i++)
        {
          for (j = i+1; j < 9; j++)
            {
              if (boxToRow[i] != 0 && (boxToRow[i] == boxToRow[j]) )
                {
                  errorCode |= (1 << 6);                /* set bit 7 */
                }
            }
        }
    }
  return errorCode;
}

/* This function logically reduces the given puzzle by using the rules of
 * Sudoku to fill in any spaces that can be found by inspecting rows,
 * columns, and boxes.  For blank squares in puzzle, sets bits 1-9. For filled
 * squares, sets bit for number - i.e. if 7 in square, sets bit #7.  Once
 * put in this form, uses NAND over each row, column, and box.  The bits
 * left on in the square represent the possible values that square could
 * have.  Keep looping over the puzzle until no more bits change.  If total
 * number of bits in puzzle is 81- then puzzle is solved!  If any square has
 * zero bits turned on- then puzzle is un-solvable.
 */
unsigned int bitCount (unsigned int value)  /* counts bits turned on */
{
  unsigned int count = 0;
  while (value > 0)                         /* until all bits are zero */
    {
      if ((value & 1) == 1)                 /* check lower bit */
      count++;
      value >>= 1;                          /* shift bits, removing lower bit */
    }
  return count;
}

void printBitPuzzle(int bitPuzzle[])        /* prints out bitPuzzle in hex */
{
  int i;
  for (i = 0; i < 81; i += 3)
    {
      if (!(i%9)) printf("\n | ");
      if (!(i%27)) printf("-----------------------------------------\n | ");
      printf("%03x %03x %03x", bitPuzzle[i], bitPuzzle[i+1], bitPuzzle[i+2]);
      printf(" | ");
    }
  printf("\n | --------------------------------------- |");
  printf("\n\n");
}
  
void crunchPuzzle(int puzzle[])
{
  int bitPuzzle[81];
  int i, totalBits;
  /* create bitPuzzle with # of bit on equal to value in puzzle */
  for (i = 0; i < 81; i++)
    {
      if (puzzle[i] == 0) bitPuzzle[i] = 0x1ff;
      else bitPuzzle[i] = 1 << (puzzle[i]-1);
    }
  printBitPuzzle(bitPuzzle);

  /* calculate total bitCount of the puzzle - solved if 81 */
  totalBits = 0;
  for (i = 0; i < 81; i++) totalBits += bitCount(bitPuzzle[i]);
  if (totalBits == 81) printf("SOLVED -- ");
  printf("totalBits = %d\n\n", totalBits);

  if (puzzle[0]) return;
}

/* This function uses recursive backtracking to solve the puzzle.
 */
void solvePuzzle(int puzzle[])
{
  /* WRITE ME */
  if (puzzle[0]) return;
}


/* This function writes out the (hopefully solved) puzzle.
 */
void writePuzzle(int puzzle[])
{
  int i;
  for ( i = 0; i < 81; ++i)
    {
      if (puzzle[i] != 0 ) printf("%d", puzzle[i]);
      else printf(".");
    }
  printf("\n\n");
}


int main(int argc, char *argv[])
{
  int puzzle[81];
  int puzzleError;
  while (1)                                        /* read puzzles until EOF */
    {
      puzzleError = readPuzzle(puzzle);
      if (puzzleError & 1) break;                  /* reached EOF, done! */
      puzzleError |= validatePuzzleRows(puzzle);
      puzzleError |= validatePuzzleCols(puzzle);
      puzzleError |= validatePuzzleBoxes(puzzle);

      /* Command line argument -e used to print error numbers for debugging */
      if (puzzleError && (argc == 2) && (argv[1][0] == '-') &&
          (argv[1][1] == 'e') && (argv[1][2] == '\0'))
        {
          printf("Error - %d\n\n", puzzleError);            
        }
      else if (puzzleError) printf("Error\n\n");
      else                                         /* puzzle is 'well-formed' */
        {
          crunchPuzzle(puzzle);
          solvePuzzle(puzzle);
          writePuzzle(puzzle);
        }
    }
  return 0;
}
