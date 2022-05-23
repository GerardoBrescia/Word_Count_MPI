#include <string.h>
#include "mpi.h"

typedef struct Word_occurrence{

    char word[46];
    int num;

} Word_occurrence;

Word_occurrence create_word_occurence(char * word ,int num);

void create_word_datatype(MPI_Datatype *wordtype);