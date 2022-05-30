#include "file_chunk.h"

void print_list(char *elem, char*user_data);

void print_file_chunk(File_chunk * chunk);

void printn_file_chunk(File_chunk chunk);

void print_file_chunk_array(File_chunk * chunks, int num_elements);

void print_received_chunks(File_chunk * chunks, int num_elements, int rank);