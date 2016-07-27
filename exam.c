#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define     MAXCHAPTERS     10          /* max. chapters */
#define     MAXQUESTIONS   100	        /* max. questions per chapter */
#define     MAXANSWERS	     4	        /* max. answers per question */
#define     MAXSTRING	  1000	        /* max. lenght of text unit */
#define     SELECTED	     1
#define     NOTSELECTED      0

#define     BEGCH	    '$'
#define     BEGQ	    '#'
#define     BEGA	    '@'
#define     ENDFILE	    '~'

typedef struct _QUESTANSW {
  char *q;		                /* question */
  char *a[MAXANSWERS];                  /* array of answers */
  int n;		                /* number of answers */
  int s;		                /* 0 not selected, 1 selected */
  size_t size;
} QUESTANSW;

typedef struct _CHAPTER {
  char *name;	                        /* chapter name */
  QUESTANSW *qa[MAXQUESTIONS];
  int n;		                /* number of questions */
  size_t size;
} CHAPTER;

typedef struct _LECTURE {
  char *name;	                        /* lecture name */
  CHAPTER *ch[MAXCHAPTERS];
  int n;		                /* number of chapters */
  size_t size;
} LECTURE;

#define MAXPERMUTATIONS     12
int IndxAnsw [MAXPERMUTATIONS] [MAXANSWERS] = {{ 1, 2, 3, 4 } ,
					       { 2, 1, 4, 3 } ,
					       { 3, 1, 2, 4 } ,
					       { 4, 2, 1, 3 } ,
					       { 3, 4, 1, 2 } ,
					       { 4, 3, 2, 1 } ,
					       { 1, 3, 4, 2 } ,
					       { 2, 4, 3, 1 } ,
					       { 1, 3, 2, 4 } ,
					       { 2, 4, 1, 3 } ,
					       { 3, 1, 4, 2 } ,
					       { 4, 2, 3, 1 }};
LECTURE *L;
CHAPTER *CH;
QUESTANSW *QA;
int QuestionEx[MAXCHAPTERS];
char Buffer[MAXSTRING];
FILE *Input, *OutQst, *OutAns;
size_t MemSize;
time_t t;
int ChapterNo, QuestionNo, AnswerNo;
int Trace = 1;
int i, j;
char *endptr;

LECTURE *new_Lecture() {
  LECTURE *retVal =  malloc(sizeof(LECTURE));
  if (retVal == NULL) {
    free (retVal);
    return NULL;
  }
  retVal->size = sizeof(LECTURE);
  return retVal;
}

CHAPTER *new_Chapter() {
  CHAPTER *retVal =  malloc(sizeof(CHAPTER));
  if (retVal == NULL) {
    free (retVal);
    return NULL;
  }
  retVal->size = sizeof(CHAPTER);
  return retVal;
}

QUESTANSW *new_QuestAns() {
  QUESTANSW *retVal =  malloc(sizeof(QUESTANSW));
  if (retVal == NULL) {
    free (retVal);
    return NULL;
  }
  retVal->size = sizeof(QUESTANSW);
  return retVal;
}

char GetString(char *Buffer) {
  char *Buff, *EndOfLine, c;
  Buff = Buffer;
  EndOfLine = NULL;
  do {
    if (feof(Input)) {
      printf ( "ERROR: End of file.\n" );
      exit (1);
    }
    c = *Buff = getc(Input);
    if ((EndOfLine == NULL) && ((c == '\r') || (c == '\n'))) {
      EndOfLine = Buff;
    }
    Buff++;
  } while ((c != BEGCH) && (c != BEGQ) && (c != BEGA) && (c != ENDFILE));
  *EndOfLine = '\0';
  return (c);
}

int PlaceString(char *Buffer, char **Place) {
  MemSize = strlen (Buffer);
  if ((*Place = malloc(MemSize)) == NULL) {
    printf("ERROR: Can't allocate memmory.\n");
    exit (1);
  }
  strcpy(*Place, Buffer);
  return (0);
}

int SelectQuestions() {
  int i, j, k, l;
  QuestionNo = L->ch[ChapterNo]->n;
  if (QuestionNo < QuestionEx[ChapterNo]) {
    printf ("ERROR: Too many questions required for chapter %d.\n", ChapterNo);
    exit(1);
  }
  for(j=0; j<QuestionNo; j++) {
    L->ch[ChapterNo]->qa[j]->s = NOTSELECTED;
  }
  i = 0;
  while (i != QuestionEx[ChapterNo]) {
    j = (rand()/3+rand()/3+rand()/3) % QuestionNo;
    if (L->ch[ChapterNo]->qa[j]->s == NOTSELECTED) {
      L->ch[ChapterNo]->qa[j]->s = SELECTED;
      fprintf(OutQst, "\n%d. %s\n", i+1, L->ch[ChapterNo]->qa[j]->q);
      AnswerNo = L->ch[ChapterNo]->qa[j]->n;
      k = (rand()/2+rand()/2) % MAXPERMUTATIONS;
      for (l=0; l<AnswerNo; l++) {
	fprintf(OutQst, "%c. %s\n", 'A'+l, L->ch[ChapterNo]->qa[j]->a[IndxAnsw[k][l]-1]);
	if (IndxAnsw[k][l] == 1)
	  fprintf(OutAns, "%c", 'A'+l);
      }
      i++;
    }
  }
  return(0);
}

int main(int argc, char *argv[]) {
  char c;
  if (argc < 4) {
    printf("exam InFile SerialNumber NoOfQuestions1 NoOfQuestions2 ...\n");
    exit(1);
  }
  if ((Input = fopen (argv[1], "r")) == NULL) {
    printf("ERROR: Can't open %s.\n", argv[1]) ;
    exit(1);
  }
  if ((OutQst = fopen (strcat(strcpy(Buffer,argv[2]),".qst"), "w")) == NULL) {
    printf("ERROR: Can't open %s.\n", Buffer);
    exit(1);
  }
  fprintf(OutQst, "SERIAL No: %s\n", argv[2]);
  if((OutAns = fopen (strcat(strcpy(Buffer,argv[2]),".ans"), "w")) == NULL) {
    printf("ERROR: Can't open %s.\n", Buffer);
    exit (1);
  }
  for (i=0; i<argc-3; i++) {
    QuestionEx[i] = strtol(argv[i+3], &endptr, 10);
  }
  if ((c = GetString(Buffer)) != BEGCH) {
    printf("ERROR: Syntax.\n");
    exit(1);
  }

  L = new_Lecture();
  assert(L != NULL);
  
  PlaceString(Buffer, &L->name);
  if (Trace) printf( "%s\n", L->name);
  ChapterNo = -1;
  while (c == BEGCH) {
    if ((c = GetString(Buffer)) != BEGQ) {
      printf ("ERROR: Syntax.\n");
      exit(1);
    }

    ChapterNo++;
    QuestionNo = -1;
    CH = new_Chapter();
    assert(CH != NULL);
    L->ch[ChapterNo] = CH;
    PlaceString (Buffer, &L->ch[ChapterNo]->name);
    if (Trace) printf("%s\n", L->ch[ChapterNo]->name);

    while (c == BEGQ) {
      if ((c = GetString(Buffer)) != BEGA) {
	printf("ERROR: Syntax.\n");
	exit(1);
      }
      QuestionNo++;
      AnswerNo = -1;
      L->ch[ChapterNo]->qa[QuestionNo] = new_QuestAns();
      assert(L->ch[ChapterNo]->qa[QuestionNo] != NULL);
      PlaceString(Buffer, &L->ch[ChapterNo]->qa[QuestionNo]->q);
      if(Trace) printf("%s\n", L->ch[ChapterNo]->qa[QuestionNo]->q);
      while (c == BEGA) {
	AnswerNo++;
	c = GetString(Buffer);
	PlaceString(Buffer, &L->ch[ChapterNo]->qa[QuestionNo]->a[AnswerNo]);
	if (Trace) printf("%s\n", L->ch[ChapterNo]->qa[QuestionNo]->a[AnswerNo]);
      }
      L->ch[ChapterNo]->qa[QuestionNo]->n = AnswerNo+1;
    }
    L->ch[ChapterNo]->n = QuestionNo+1;
  }
  L->n = ChapterNo+1;
  fclose (Input);

  ChapterNo = 0;
  srand((unsigned) time(&t));
  while (ChapterNo != L->n) {
    fprintf(OutQst, "\n\n****** Chapter %d **********\n", ChapterNo+1);
    SelectQuestions();
    ChapterNo++;
  }
  fclose(OutQst);
  fprintf(OutAns, "%c", ENDFILE);
  fclose(OutAns);
  exit(0);
}
