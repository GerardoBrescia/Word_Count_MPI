#include "lexer.h"

int chech_if_end_char(char ch){

    if(ch == '\n' || ch =='\r' || ch == '\t' 
                || ch == ' ' || ch == EOF){

        return 1;

    }
    return 0;
}

void count_word(GHashTable* hash, char * lexeme, int * lex_counter){

    gpointer lookup;
    int val;
    lookup = g_hash_table_lookup(hash,lexeme);
    if(lookup == NULL){
        val = 1;
        (*lex_counter)++;
    }
    else{
        val = GPOINTER_TO_INT(lookup) + 1;
    }
    g_hash_table_insert(hash,lexeme,GINT_TO_POINTER (val));
}

//Questa funzione deve ricevere una matrice di Word_Occurrence instanziare ogni word occurrence
//Ovviamente questo deve essere gestito da chi riceve (deve cambiare tutto).
Word_occurrence* create_inf_to_send(GHashTable* hash, int  *num_occ){

    int length;
    char ** lexems = (char **) g_hash_table_get_keys_as_array (hash , &length);
    char * lexeme;
    gpointer lookup;
    int occ;

    Word_occurrence * occurrences;
    MPI_Alloc_mem(sizeof(Word_occurrence) * length, MPI_INFO_NULL , &occurrences);

    for (int i = 0; i < length; i++)
    {
        lexeme = lexems[i]; 
        lookup = g_hash_table_lookup(hash,lexeme); 
        occ =  GPOINTER_TO_INT(lookup);
        occurrences[i].num = occ;
        strncpy(occurrences[i].word, lexeme, 46);  
        free(lexeme);
    }

    free(lexems);
    g_hash_table_destroy (hash);
 
    return occurrences;

}

Word_occurrence * get_lexeme_from_chunk(File_chunk *chunks_received, int chunk_number, int rank, int * num_word){

    FILE *fp;
    
    char lexeme[45];
    int n = 0, lex_num = 0;
    int num_occ = 0;

    File_chunk chunk;
    char ch;
    GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
    Word_occurrence * occurrences;

    int no_ended= 0;

    for(int i = 0; i < chunk_number; i++){

        chunk = chunks_received[i];
        fp = fopen(chunk.path , "r");
        fseek(fp, chunk.start_offset , SEEK_CUR);

        //skip della prima parola se questa non è all'inizio
        if(ftell(fp) != 0){
            fseek(fp,  -1 , SEEK_CUR);

            ch = fgetc(fp);

            if(!chech_if_end_char(ch)){

                while (!chech_if_end_char(ch))
                {
                    ch = fgetc(fp);
                }
            }
        }
        
        int state = 0;  

        while (ftell(fp) < (chunk.end_offset)  || no_ended)
        {
            
            lexeme[n] = fgetc(fp);

            switch (state)
            {
                case 0: 
                    if(isalpha(lexeme[n]) || isdigit(lexeme[n])){
                        state = 1;
                        n++;
                        no_ended = 1;   
                    }
                break; 
                case 1: 
                    if(isalpha(lexeme[n]) || isdigit(lexeme[n])){
                        state = 1;
                        n++ ; 
                        no_ended = 1; 
                    }
                    else{
                        if(lexeme[n] == '\n' || lexeme[n] =='\r' || lexeme[n] == '\t' 
                                            || lexeme[n] == ' ' || lexeme[n] == EOF){
                            lexeme[n] = '\0';
                            for(int j = 0; lexeme[j]; j++){
                                lexeme[j] = tolower(lexeme[j]);
                            }
                            count_word(hash , strdup(lexeme), &lex_num);
                            n = 0;
                            state = 0;
                            no_ended = 0;
                        }  
                    }
                break;
            }
        }

    }

    
    occurrences = create_inf_to_send(hash, &num_occ);
    * num_word = lex_num; 

    return occurrences;


}


