#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <math.h>
#include <unistd.h>

#include "mpi.h"
#include "file_inf.h"
#include "logger.h"
#include "lexer.h"

#define COMM_TAG 88
#define MASTER_RANK 0

int main (int argc, char *argv[]){

    int numtasks;
    int rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status Stat;

    //Creazione del datatype per il chunk 
    MPI_Datatype chunktype;
    create_chunk_datatype(&chunktype);

    MPI_Datatype wordtype;
    create_word_datatype(&wordtype);

    // controllo che il numero di processi sia almeno maggiore o uguale a due 
    // se non ci sono più di due processi non ha senso parallelizzare
    if (numtasks < 2) 
            printf("I'm the MASTER and I have no slaves");

    //Master
    if(0 == rank){
        
        //Ottengo informazioni sui file all'interno della cartella (dimensione e nome)
        double total_size = 0;
        GList * files;
        files = get_files_info_from_dir("/home/parole/", &total_size);
        printf("%f\n" , total_size);
        int temp = total_size / (numtasks-1);
        double partition_size = temp;
        int remnant = (int) total_size % (numtasks-1);
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
        int distr_remnant = 1;

        int pack_size;
        //char ** array_packed_message = malloc(sizeof(char *) * numtasks);
        char array_packed_message[numtasks][MAX_PACK_SIZE];

        //Istanzio array di request e status per l'invio e la ricezione asincrona
        MPI_Request * reqs = malloc(sizeof(MPI_Request) * numtasks -1);      
        MPI_Status * stats = malloc(sizeof(MPI_Status) * numtasks -1);
        
        for(int i=0; i<file_nums; i++){

            //Prendo file
            t = (File_inf *) current_item->data;
            file_offset = 0;
            remaining_file = t->total_size;

            while(remaining_file > 0){

                if(to_assign == 0){
                    //se chunk_count non è zero posso inviare 
                    if(chunk_count!=0){
                        printf("Sending array with %d elements to process %d\n" , chunk_count, task_to_assign);
                        pack_send_chunks(chunk_to_send, chunk_count, chunktype, task_to_assign);
                    }  
                    to_assign = partition_size;
                    task_to_assign++; 
                    chunk_count = 0;
                }

                if(distr_remnant && task_to_assign == numtasks -1){
                    to_assign = partition_size + remnant;
                    distr_remnant = 0;
                }

                //se la grandezza del file è più piccolo o uguale di quello da assgenare lo assegno tutto
                if(remaining_file <= to_assign){
                    File_chunk * chunk = create_file_chunk(file_offset , file_offset + remaining_file, t->path);
                    chunk_to_send[chunk_count] = *chunk;
                    chunk_count++;
                    //print_file_chunk(chunk);
                    to_assign -= remaining_file;
                    remaining_file = 0;
                }
                else{ // altrimenti devo assegnare solo una porzione
                    File_chunk * chunk = create_file_chunk(file_offset , file_offset + to_assign, t->path);
                    chunk_to_send[chunk_count] = *chunk;
                    chunk_count++;
                    //print_file_chunk(chunk);
                    file_offset += to_assign;
                    remaining_file -= to_assign;
                    to_assign = 0;
                }   
            }

            current_item = current_item->next;

        }

        printf("Sending array with %d elements to process %d\n" , chunk_count, task_to_assign);
         
        pack_send_chunks(chunk_to_send, chunk_count, chunktype, task_to_assign);

        //Ricevere da tutti i figli
        MPI_Status stat;
        int words_in_message, length, occ;
        Word_occurrence * occurrences;
        GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
        gpointer lookup;

        for(int i = 0; i < numtasks -1; i++){

            MPI_Probe(MPI_ANY_SOURCE, COMM_TAG, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, wordtype, &words_in_message);
            occurrences = malloc(sizeof(Word_occurrence) * words_in_message);
            MPI_Recv(occurrences, words_in_message, wordtype, stat.MPI_SOURCE, COMM_TAG, MPI_COMM_WORLD, &stat);

            for(int j=0; j< words_in_message; j++){

                lookup = g_hash_table_lookup(hash,occurrences[j].word);
                if(lookup == NULL){
                    g_hash_table_insert(hash,occurrences[j].word,GINT_TO_POINTER (occurrences[j].num));
                }
                else{
                    g_hash_table_insert(hash,occurrences[j].word,GINT_TO_POINTER (occurrences[j].num + GPOINTER_TO_INT(lookup)));
                }
            }            
        }

        char ** entries = (char **) g_hash_table_get_keys_as_array (hash , &length);
        
        for(int i = 0; i<length; i++){

            lookup = g_hash_table_lookup(hash,entries[i]); 
            occ =  GPOINTER_TO_INT(lookup);
            printf("%s -------------------- %d\n", entries[i], occ);
        }

    }
    else{

        File_chunk * chunk_to_recv;
        int chunk_number = 0, num_occ = 0;
        Word_occurrence * occurrences;
        chunk_to_recv = unpack_recv_chunks(chunktype, Stat, &chunk_number);

        printf("[worker %d] : ho ricevuto %d chunks\n", rank,chunk_number);
        print_received_chunks(chunk_to_recv, chunk_number, rank);
        
        occurrences = get_lexeme_from_chunk(chunk_to_recv, chunk_number, rank, &num_occ);

        //MPI_Send(&num_occ, 1, MPI_INT, 0, COMM_TAG, MPI_COMM_WORLD);
        MPI_Send(occurrences, num_occ, wordtype, 0, COMM_TAG, MPI_COMM_WORLD);
              
    }


    MPI_Finalize();

    return 0;

}