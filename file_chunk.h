#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#define MAX_PACK_SIZE 8192
#define COMM_TAG 88
#define MASTER_RANK 0

#ifndef FILE_CHUNK
#define FILE_CHUNK
    typedef struct File_chunk{

        double start_offset;
        double end_offset;
        char path[260];

    } File_chunk;
#endif

File_chunk* create_file_chunk(double start_offset , double end_offset, char * path);

void create_chunk_datatype(MPI_Datatype *chunktype);

void pack_send_chunks(File_chunk * chunk_to_send, int chunk_count, MPI_Datatype chunktype, int receiver);

File_chunk * unpack_recv_chunks(MPI_Datatype chunktype, MPI_Status Stat, int * chunk_number);