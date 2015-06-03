#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>   
#include <bitset> 
#include <string>

using namespace std;

#define CROSSOVER_RATE            0.2
#define MUTATION_RATE             0.001
#define POP                       100
#define CHROMO_LENGTH             28
#define GENES                     6 
#define GENE_LENGTH               4
#define RANDOM_NUM      ((float)rand()/(RAND_MAX+1))

// initialise the population
void initialise(int** population)
{
  srand((unsigned long) time(NULL));
  for(int j=0;j<POP;j++){
    for(int i=0;i<CHROMO_LENGTH;i++) {
      population[j][i] = rand()%2;
    }
  }
}

// roulette selection
int **roulette(int** survivors, float *n){
  //parents memory
  int **parents = new int*[2];
  for(int k=0;k<2;k++) {
    parents[k] = new int[CHROMO_LENGTH];
    //random value between 0 and 1
    float t = (float)rand()/(float)RAND_MAX;
    int i=0;
    float sum = 0.0;
    while (sum<t){
      sum+=*(n+i);
      i++;
    }
    for(int j=0;j<CHROMO_LENGTH;j++){
      parents[k] = survivors[i-1];
    }
  }
  return parents;
}

 // mutate an individual
int **mutate(int** temp){
  for (int j=0;j<CHROMO_LENGTH;j++){ //chech mutation probability for each bit
    float ran = (float)rand()/(float)RAND_MAX;
    if(ran<MUTATION_RATE){
      int r = rand()%CHROMO_LENGTH;  
        for ( int i = 0; i < 2; i++ ){
          int v1 = temp[i][r];
          if(v1==0) temp[i][r] = 1; if(v1==1) temp[i][r] = 0;
          int r = rand()%CHROMO_LENGTH;
        }
      }
  }
  return temp;
}

// cross two individuals
void crossover(int** survivors, int** population, int numb,float *n){
  for(int z=0;z<POP;z++){
    int P = 2;
    //temp array to hold the parents
    int** temp = new int* [P];
    for(int i=0;i<P;i++) temp[i] = new int[CHROMO_LENGTH];
    //roulette wheel selection
    int ** r = roulette(survivors,n);
    //cross over 2 individuals at a time from all parents
    for(int j=0;j<CHROMO_LENGTH;j++){
      temp[0][j] = r[0][j];
    }
    for(int j=0;j<CHROMO_LENGTH;j++){
      temp[1][j] = r[1][j];
    }
    float ran = (float)rand()/(float)RAND_MAX;
    //crossover probability
    if (ran<CROSSOVER_RATE){
      int cp = rand()%(CHROMO_LENGTH-2) + 1;
      // crossover point
      for(int j=cp;j<CHROMO_LENGTH;j++){ 
        temp[0][j] = r[1][j]; temp[1][j] = r[0][j];}
      temp = mutate(temp);
      for(int j=0;j<CHROMO_LENGTH;j++) r[0][j] = temp[0][j];
      for(int j=0;j<CHROMO_LENGTH;j++) r[1][j] = temp[1][j];
    }
    else {
      temp = mutate(temp);
      for(int j=0;j<CHROMO_LENGTH;j++) r[0][j] = temp[0][j];
	    for(int j=0;j<CHROMO_LENGTH;j++) r[1][j] = temp[1][j];
    }
    
    int w=0;
    for(int j=0;j<2;j++) population[j+w] = r[j]; w++;
    for(int j=0;j<P;j++) delete[] temp[j];
    delete[] temp;
  }
}
