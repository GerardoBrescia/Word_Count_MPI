#include <glib.h>
#include "mpi.h"

typedef struct File_inf{

    double total_size;
    char * path;

} File_inf;

File_inf * get_file_info(char * file_paths);

GList * get_files_info_from_dir(char * dir_paths, double *files_size);

void free_files_info(GList * list, int num);

