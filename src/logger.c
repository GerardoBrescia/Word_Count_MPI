#include <stdio.h>
#include "file_inf.h"
#include "logger.h"


void print_list(char *elem, char*user_data){

    File_inf * file = (File_inf *) elem;
    printf("File name: %s\n" , file->path);
    printf("Size: %f\n\n", file->total_size);

}

void print_file_chunk(File_chunk * chunk){

    printf("start_offset %f\n" , chunk->start_offset);
    printf("end_offset %f\n" , chunk->end_offset);
    printf("path %s\n\n" , chunk->path);


}

void printn_file_chunk(File_chunk chunk){

    printf("start_offset %f\n" , chunk.start_offset);
    printf("end_offset %f\n" , chunk.end_offset);
    printf("path %s\n\n" , chunk.path);

}

void print_file_chunk_array(File_chunk * chunks, int num_elements){

    for (int i = 0; i< num_elements; i++){
        printn_file_chunk(chunks[i]);
    }

}

void print_received_chunks(File_chunk * chunks, int num_elements, int rank){

    char file_output[50];
    char rankc[2];
    rankc[0] = rank + '0';
    rankc[1] = '\0';
    strncpy(file_output, "out", 50);
    strncat(file_output, rankc, 50);
    FILE * out = fopen (file_output , "w");

    for (int i = 0; i< num_elements; i++){
        fprintf(out,"[Worker  %d]\n", rank);
        fprintf(out,"start_offset %f\n" , chunks[i].start_offset);
        fprintf(out,"end_offset %f\n" , chunks[i].end_offset);
        fprintf(out,"path %s\n\n" , chunks[i].path);
    }

}