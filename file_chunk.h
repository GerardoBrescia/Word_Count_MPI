#include <stdlib.h>
#include <string.h>

#include "mpi.h"

typedef struct File_chunk{

    double start_offset;
    double end_offset;
    char path[260];
    int task_assigned;

} File_chunk;

File_chunk* create_file_chunk(double start_offset , double end_offset, char * path, int task_assigned);

void create_chunk_datatype(MPI_Datatype *chunktype);