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
    printf("task %d\n" , chunk->task_assigned);
    printf("path %s\n\n" , chunk->path);


}