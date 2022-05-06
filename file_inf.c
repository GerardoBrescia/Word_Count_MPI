#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <glib.h>

#include "file_inf.h"
#include "logger.h"

File_inf * get_file_info(char * file_path){

    struct stat sb;
    File_inf * file = (File_inf *) malloc(sizeof(File_inf));

    if(stat(file_path, &sb) != -1){
        file->total_size = sb.st_size;
        file->path = malloc(sizeof(char) * strlen(file_path) +1);
        strncpy(file->path ,file_path , strlen(file_path));
    }

    return file;
    
}

File_inf ** get_files_info_from_file(char * paths_file){

    File_inf ** info = (File_inf **) malloc(sizeof(File_inf *));

    return info;

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

        g_list_foreach(list, (GFunc)print_list, NULL);

    }

    return list;

}


