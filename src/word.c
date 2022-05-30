#include "word.h"

Word_occurrence * create_word_occurence(char * word ,int num){

    Word_occurrence * w = malloc(sizeof(Word_occurrence));

    strncpy(w->word, word, 46);
    w->num = num;

    return w;
}

void create_word_datatype(MPI_Datatype *wordtype){

    MPI_Datatype oldtypes[2];
    int blockcounts[2];

    MPI_Aint    offsets[2], lb, extent;

    offsets[0] = 0;
    oldtypes[0] = MPI_INT;
    blockcounts[0] = 1;

    MPI_Type_get_extent(MPI_INT, &lb, &extent);
    offsets[1] = 1 * extent;
    oldtypes[1] = MPI_CHAR;
    blockcounts[1] = 46;


    MPI_Type_create_struct(2, blockcounts, offsets, oldtypes, wordtype);
    MPI_Type_commit(wordtype);


}

//funzione per lo swap degli elementi
void swap(Word_occurrence *a, Word_occurrence *b) {
  Word_occurrence t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(Word_occurrence * array, int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high].num;
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j].num >= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(Word_occurrence *array, int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

void sort_occurrences(Word_occurrence ** p_occurrences, int n){
     
     Word_occurrence * occurrences = *p_occurrences;
     quickSort(occurrences, 0, n - 1);

}