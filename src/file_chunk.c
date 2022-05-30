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

void pack_send_chunks(File_chunk * chunk_to_send, int chunk_count, MPI_Datatype chunktype, int receiver,  MPI_Request *req){

    int  position = 0;
    char outbuf [MAX_PACK_SIZE];

    int size1, size2;
    int uno = 1;

    MPI_Pack_size(uno,
                 MPI_INT,
                 MPI_COMM_WORLD,
                 &size1);

    MPI_Pack_size(chunk_count,
                 chunktype,
                 MPI_COMM_WORLD,
                 &size2);

    MPI_Pack(&chunk_count, 1, MPI_INT, outbuf, MAX_PACK_SIZE, &position, MPI_COMM_WORLD);
    MPI_Pack(chunk_to_send, chunk_count, chunktype, outbuf, MAX_PACK_SIZE, &position, MPI_COMM_WORLD);

    MPI_Isend(outbuf, MAX_PACK_SIZE, MPI_PACKED, receiver, COMM_TAG, MPI_COMM_WORLD, &(req[receiver-1]));
    
}

File_chunk * unpack_recv_chunks(MPI_Datatype chunktype, MPI_Status Stat, int * chunk_number){

    char outbuf [MAX_PACK_SIZE];
    int position = 0;
    *chunk_number = 0;

    MPI_Recv(outbuf, MAX_PACK_SIZE, MPI_PACKED, MASTER_RANK, COMM_TAG, MPI_COMM_WORLD, &Stat);
    MPI_Unpack(outbuf, MAX_PACK_SIZE, &position, chunk_number, 1,
	    MPI_INT, MPI_COMM_WORLD);

    File_chunk * chunks_to_recv =(File_chunk *) malloc(sizeof(File_chunk ) * (*chunk_number));
    MPI_Unpack(outbuf, MAX_PACK_SIZE, &position, chunks_to_recv, *chunk_number,
	    chunktype, MPI_COMM_WORLD);

    return chunks_to_recv;
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
