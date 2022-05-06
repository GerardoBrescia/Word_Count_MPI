#include "file_chunk.h"

#include <stdio.h>

File_chunk* create_file_chunk(double start_offset , double end_offset, char * path, int task_assigned){

    File_chunk * chunk = malloc(sizeof(File_chunk));

    chunk->start_offset = start_offset;
    chunk->end_offset = end_offset;
    strncpy(chunk->path, path, 260);
    chunk->task_assigned = task_assigned;
    
    return chunk;

}

void create_chunk_datatype(MPI_Datatype *chunktype){

    MPI_Datatype oldtypes[3];
    int blockcounts[3];

    MPI_Aint    offsets[2], lb, extent;

    offsets[0] = 0;
    oldtypes[0] = MPI_DOUBLE;
    blockcounts[0] = 2;
/*
    MPI_Type_get_extent(MPI_DOUBLE, &lb, &extent);
    offsets[1] = 2 * extent;
    oldtypes[1] = MPI_CHAR;
    blockcounts[1] = 2;
*/


}