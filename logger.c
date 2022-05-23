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

void print_received_chunks(File_chunk * chunks, int num_elements, int numstask){

    for (int i = 0; i< num_elements; i++){
        printf("[Worker  %d]\n", numstask);
        printn_file_chunk(chunks[i]);
    }

}