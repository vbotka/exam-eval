#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define     MAXQUESTIONS   100	/* max. questions */
#define     MAXSTRING	  1000
#define     ENDFILE	    '~'
#define     OMITTED	    'X'

char Buffer[MAXSTRING], Exm[MAXQUESTIONS] ;
FILE *InExm, *InAns ;
int Right=0, Wrong=0, Omitt=0 ;

int main ( int argc, char *argv[] ) {
  char Ans ;
  int i ;
  if (argc < 2) {
    printf ( "eval SerialNumber\n" ) ;
    exit (1) ;
  }
  if (( InExm = fopen (strcat(strcpy(Buffer,argv[1]),".exm"), "r")) == NULL ) {
    printf ("ERROR: Can't open %s.\n", Buffer ) ;
    exit (1) ;
  }
  if (( InAns = fopen (strcat(strcpy(Buffer,argv[1]),".ans"), "r")) == NULL ) {
    printf ("ERROR: Can't open %s.\n", Buffer ) ;
    exit (1) ;
  }
  printf ( "\nSERIAL No: %s\n         ", argv[1] ) ;
  i = 0 ;
  while ( !feof(InExm)) {
    Exm[i] = getc(InExm) ;
    printf ( "%c ", Exm[i] ) ;
    i++ ;
  }
  fclose (InExm) ;
  printf ( "\nCORRECT: " ) ;
  i = 0 ;
  while (1) {
    Ans = getc(InAns) ;
    if ( Ans==ENDFILE)
      break ;
    if ( feof(InAns)) {
      printf ( "\nERROR: Unexpected end of file.\n" ) ;
      exit (1) ;
    }
    printf ( "%c ", Ans ) ;
    if ( Exm[i] == OMITTED || Exm[i] < 64  || Exm[i] > 91 )
      Omitt++ ;
    else if ( Exm[i] == Ans )
      Right++ ;
    else
      Wrong++ ;
    i++ ;
  }
  fclose (InAns) ;
  printf ( "\nCORRECT: %d\tWRONG: %d\tOMITTED: %d\n", Right, Wrong, Omitt ) ;
  printf ( "\n\n\n---------------------------------------------------------\n" ) ;
  exit (0) ;
}
