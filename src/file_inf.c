#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <glib.h>
#include <errno.h>

#include "file_inf.h"
#include "logger.h"

File_inf * get_file_info(char * file_path){

    struct stat sb;
    File_inf * file = (File_inf *) malloc(sizeof(File_inf));

    if(stat(file_path, &sb) != -1){
        file->total_size = sb.st_size;
        file->path = malloc(sizeof(char) * strlen(file_path) + 1);
        strncpy(file->path ,file_path , strlen(file_path)+1);
    }

    return file;
    
}

//Funzione che crea la lista di oggetti file_info
GList * get_files_info_from_dir(char * dir_paths, double *files_size){

    GList* list = NULL;
    DIR *d;
    struct dirent *dir;
    char file_path[260];
    char path_sep[]={'/','\0'};
    char end[] ={'\0'};


    //apro la directory
    d = opendir(dir_paths);

    if (d)
    {

        File_inf * temp_info;

        while ((dir = readdir(d)) != NULL) // scorro l'intero contenuto della directory
        {
            if(dir-> d_type != DT_DIR){

                char end_word = '\0';

                //creo la path assoluta del file per ottenere le informazioni
                strncpy(file_path, dir_paths, 260);
                strncat(file_path, path_sep, 2);
                strncat(file_path, dir->d_name, strlen(dir->d_name));
            
                temp_info = get_file_info(file_path);
                *files_size += temp_info->total_size;
                
                list = g_list_append(list, temp_info);

            }

        }

        closedir(d);
        
    }
    else if (ENOENT == errno){
     return NULL;
    }

    return list;

}

void free_files_info(GList * list, int num){

    GList * elem = list;
    for(int i=0; i<num; i++){
            File_inf * f = elem->data;
            free(f->path);
            free(elem->data);
            elem = elem->next;
    }
    g_list_free(list);
}

