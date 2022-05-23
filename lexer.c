
#include "lexer.h"

FILE *out;

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

Word_occurrence* create_inf_to_send(GHashTable* hash, int  *num_occ){

    int length;
    char ** lexems = (char **) g_hash_table_get_keys_as_array (hash , &length);
    char * lexeme;
    gpointer lookup;
    int occ;

    //to free after received from master
    Word_occurrence * occurrences = malloc(sizeof(Word_occurrence) * length);

    for (int i = 0; i < length; i++)
    {
        lexeme = lexems[i]; 
        lookup = g_hash_table_lookup(hash,lexeme); 
        occ =  GPOINTER_TO_INT(lookup);
        occurrences[i] = create_word_occurence(lexeme, occ);   
        fprintf(out, "%s....--.-.-.-.-. %d\n", occurrences[i].word, occurrences[i].num);    
    }

    g_hash_table_destroy (hash);
 
    return occurrences;

}

Word_occurrence * get_lexeme_from_chunk(File_chunk *chunks_received, int chunk_number, int rank, int * num_word){

    FILE *fp;
    
    char lexeme[45];
    int n = 0, lex_num = 0;
    int num_occ = 0;

    char file_output[50];
    char rankc[2];
    rankc[0] = rank + '0';
    rankc[1] = '\0';
    strncpy(file_output, "out", 50);
    strncat(file_output, rankc, 50);
    out = fopen (file_output , "w");

    File_chunk chunk;
    char ch;
    GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
    GList * list = NULL;
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
                            count_word(hash , strdup(lexeme), &lex_num);
                            //list = g_list_append(list, strdup(lexeme));
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


