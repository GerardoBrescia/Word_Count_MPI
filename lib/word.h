#include <string.h>
#include <stdlib.h>
#include "mpi.h"

typedef struct Word_occurrence{

    int num;
    char word[46];

} Word_occurrence;

Word_occurrence * create_word_occurence(char * word ,int num);

void create_word_datatype(MPI_Datatype *wordtype);

void sort_occurrences(Word_occurrence ** p_occurences, int n);