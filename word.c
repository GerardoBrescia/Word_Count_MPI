#include "word.h"

Word_occurrence create_word_occurence(char * word ,int num){

    Word_occurrence w;

    strncpy(w.word, word, 46);
    w.num = num;

    return w;
}

void create_word_datatype(MPI_Datatype *wordtype){

    MPI_Datatype oldtypes[2];
    int blockcounts[2];

    MPI_Aint    offsets[2], lb, extent;

    offsets[0] = 0;
    oldtypes[0] = MPI_CHAR;
    blockcounts[0] = 46;

    MPI_Type_get_extent(MPI_CHAR, &lb, &extent);
    offsets[1] = 46 * extent;
    oldtypes[1] = MPI_INT;
    blockcounts[1] = 1;

    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, wordtype);
    MPI_Type_commit(wordtype);


}