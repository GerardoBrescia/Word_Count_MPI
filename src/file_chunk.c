#include "file_chunk.h"

#include <stdio.h>

File_chunk* create_file_chunk(double start_offset , double end_offset, char * path){

    File_chunk * chunk = malloc(sizeof(File_chunk));

    chunk->start_offset = start_offset;
    chunk->end_offset = end_offset;
    strncpy(chunk->path, path, 260);
    
    return chunk;

}

void create_chunk_datatype(MPI_Datatype *chunktype){

    MPI_Datatype oldtypes[2];
    int blockcounts[2];

    MPI_Aint    offsets[2], lb, extent;

    offsets[0] = 0;
    oldtypes[0] = MPI_DOUBLE;
    blockcounts[0] = 2;

    MPI_Type_get_extent(MPI_DOUBLE, &lb, &extent);
    offsets[1] = 2 * extent;
    oldtypes[1] = MPI_CHAR;
    blockcounts[1] = 260;

    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, chunktype);
    MPI_Type_commit(chunktype);

}

File_chunk * probe_recv_chunks(MPI_Datatype chunktype, MPI_Status Stat, int * chunk_number){

    *chunk_number = 0;

    MPI_Probe(MASTER_RANK, COMM_TAG, MPI_COMM_WORLD, &Stat);
    MPI_Get_count(&Stat, chunktype, chunk_number);

    File_chunk * chunks_to_recv;
    MPI_Alloc_mem(sizeof(File_chunk ) * (*chunk_number), MPI_INFO_NULL , &chunks_to_recv);

    MPI_Recv(chunks_to_recv, *chunk_number, chunktype, MASTER_RANK, COMM_TAG, MPI_COMM_WORLD, &Stat);
    
    return chunks_to_recv;
}
