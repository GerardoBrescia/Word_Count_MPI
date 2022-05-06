#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "mpi.h"
#include "file_inf.h"
#include "logger.h"

int main (int argc, char *argv[]){

    int numtasks;
    int rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype chunktype;
    create_chunk_datatype(&chunktype);

    // controllo che il numero di processi sia almeno maggiore o uguale a due 
    // se non ci sono più di due processi non ha senso parallelizzare
    if (numtasks < 2) 
            printf("I'm the MASTER and I have no slaves");

    //Master
    if(0 == rank){
        
        //Ottenere il nome dal quale prendere la cartella
        //Ottenere informazioni riguardo i vari files
        //le informazioni che devono essere memorizzate :
        //(Taglia del file , nome del file)           O(n)

        double total_size = 0;
        GList * files;
        files = get_files_info_from_dir("/home/parole/", &total_size);
        printf("%f\n" , total_size);
        double partition_size = total_size / (numtasks-1);
        printf("%f\n" , partition_size);
    
        //Produzione dei chunk per ogni processo che non sia quello master
        File_inf * t;
        GList * current_item = files;
        int file_nums = g_list_length(files);

        //la size dei chuank per ogni processo può essere al più 
        File_chunk * chunk_to_send = malloc(sizeof(File_chunk)* file_nums);

        double file_offset, to_assign = 0;
        double remaining_file;
        int task_to_assign = 0, chunk_count = 0;
        
        for(int i=0; i<file_nums; i++){

            //Prendo file
            t = (File_inf *) current_item->data;

            file_offset = 0;
            remaining_file = t->total_size;

            while(remaining_file > 0){

                if(to_assign == 0){
                    to_assign = partition_size;
                    task_to_assign++; 
                    chunk_count = 0;
                }

                //se la grandezza del file è più piccolo o uguale di quello da assgenare lo assegno tutto
                if(remaining_file <= to_assign){
                    File_chunk * chunk = create_file_chunk(file_offset , remaining_file, t->path, task_to_assign);
                    chunk_to_send[chunk_count] = *chunk;
                    chunk_count++;
                    print_file_chunk(chunk);
                    to_assign -= remaining_file;
                    remaining_file = 0;
                }
                else{
                    File_chunk * chunk = create_file_chunk(file_offset , to_assign, t->path, task_to_assign);
                    chunk_to_send[chunk_count] = *chunk;
                    chunk_count++;
                    print_file_chunk(chunk);
                    file_offset += to_assign;
                    remaining_file -= to_assign;
                    to_assign = 0;
                }
                // altrimenti devo assegnare solo una porzione
            }

            current_item = current_item->next;

        }
        //Per dividere i file creerò un altra struct che sarà poi quella che 
        //effettivamente rappresenterà il chunk da passare ad ogni processo.
    }
    else{

        

    }


    MPI_Finalize();

    return 0;

}


