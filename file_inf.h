#include <glib.h>
#include "mpi.h"

typedef struct File_inf{

    double total_size;
    char * path;

} File_inf;

File_inf * get_file_info(char * file_paths);

File_inf ** get_files_info_from_file(char * paths_file);

GList * get_files_info_from_dir(char * dir_paths, double *files_size);

