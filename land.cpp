#include <iostream>
#include <cstdlib>
#include <ctime>     
#include <cmath>    
#include <string>
#include <fstream>

#include "genetic.cpp"

using namespace std;

const int NROWS = 40;
const int NCOLS = 40;

class Square;
class Robot;

//ENVIRONMENT
class Square {
  public:
	  Square(){};
	  Square(int i, int j);
    friend class Robot;
	  void   display(int i, int j);
	  bool   addRobot(int a, int b, int c, int d, int e, int f, int i, int j);
	  bool   addEnergySources(int i, int j);
	  bool   addPatches(int i, int j);
	  bool   step();
	  bool   energySourceAt(int i, int j);
	  bool   patchAt(int i,int j);
	  Robot* getRobots();
		  int nrobots;
      int nsources;
	  bool npatches;
  private:
    int x, y;
    Square* env[NROWS]; //pointer to an object of class square
};

Square::Square(int i, int j){
 nrobots = 0; 
 nsources=0;
 npatches = false;
 x=i;
 y=j;
}

// ROBOT
enum Dir {
	N, E, S, W, NW, SW, NE, SE
};

int total_sources = 0;
int total_patches = 0;
int total_robots = 0;

class Robot{
 public:
  Robot(){}; //always include the deault empty constructor
  Robot(int a,int b,int c,int d,int e,int f, int i, int j, int z);
    int e_rate, max_e, e_res, cast_rate, max_cast, cast_res, cast_res_orig, e_res_orig, energy_gain,cast_gain,energy_loss,cast_loss; 
    bool step(Robot* robots);
    friend class Square;
    int energy, casting;
    int getX();
    int getY();
    Dir dir;
    bool d;
    int id; // robot's name
    int x,y; //coordinates
    int survived_for;
};

Robot::Robot(int a,int b,int c,int d,int e,int f,int i, int j,int z){
  id =z;
  survived_for=0; //counting the number of iterations the robot has made it through to assess his fitness
  // energy depletion rate
  if (a>=1 && a<=8) e_rate = a;
  if (a<1) e_rate = 1;
  if (a>8) e_rate = 8;
  //max energy level
  if (b<=8) max_e = b;
  else max_e = 8;
  //energy depletion reserve
  if (c<=50) e_res = c;
  else e_res = 50;
  //energy depletion reserve (as specified by the chrom, not to be modified during any process)
  if (c<=50) e_res_orig = c;
  else e_res_orig = 50;
  //protective casing repair rate
  if (d>=1 && d<=8) cast_rate = d;
  if (d<1) cast_rate = 1;
  if (d>8) cast_rate = 8; 
  //max protective casing
  if (e<=8) max_cast = e;
  else max_cast = 8; 
  //protective casting repair reserve
  if (f<=50) cast_res = f;
  else cast_res = 50; 
  //protective casting repair reserve (as specified by the chrom, not to be modified during any process)
  if (f<=50) cast_res_orig = f;
  else cast_res_orig = 50; 
  x = i;
  y = j;
  dir = Dir(rand() % 8);
  d=false;
  energy = 0;
  casting = max_cast;
  energy_gain = 0;
  cast_gain=0;
  energy_loss = 0;
  cast_loss=0;
  }

//Implementation
int Robot::getX() {
  return this->x;
}
int Robot::getY() {
  return this->y;
}
bool Robot::step(Robot* robots) {
  dir = robots->dir;
  energy = robots->energy;
  if(robots->energy < 0 || robots->casting < 0) return false;
  else {
    switch (dir){
	    case N:
	      robots->x++;
	      robots->y=robots->y;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
		  break;
	    case S:
	      robots->x--;
        robots->y=robots->y;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
	     break;
	    case W:
	      robots->x=robots->x;robots->y--;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
	    break;
	    case E:
	      robots->x=robots->x;robots->y++;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
	    break;
      case NW:
	      robots->x++;robots->y--;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
		  break;
	    case SW:
	      robots->x--;robots->y--;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
	    break;
	    case NE:
	      robots->x++;robots->y++;
		    if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
		    if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;
	    break;
	    case SE:
	      robots->x--;robots->y++;
        if (robots->x > 39) robots->x=0;
		    if (robots->y > 39) robots->y=0;
        if (robots->x < 0) robots->x=39;
		    if (robots->y < 0) robots->y=39;		
	    break;
    }
    return true;
  }
}
 
int getRobotId(int i, int j, Robot** robots){
  for(int z=0;z<POP;z++){ 
    if(robots[z]->x==i && robots[z]->y == j) return robots[z]->id;
  }
}

// fitness function value for individual
//pass the fit function (f) as one of the params
float f_value(Robot* robots) {
  float fv = 0.01*(robots->energy_gain)+0.08*(robots->cast_gain)+0.87*(robots->energy_loss)+0.04*(robots->cast_loss); // the final fm for 100
  return fv;
}

void assessFitness(Robot** robots, int gen, int num,int** survivors,int** population) {
  ofstream myfile;
  myfile.open("stopping.txt", fstream::app);
  float y;
  float m[num]; //array
  float n[num]; //array of normalised floats
  float sum = 0.0; //one value
  for (int i=0;i<num;i++) {
    m[i] = f_value(robots[i]);
    sum+=m[i];
    if (m[i] > m[i-1]){
      y = m[i];
    }
  }
  //normalised fitness, adds up to 1
  for (int i=0;i<num;i++) n[i]= m[i]/sum;
  float af = sum/num;
  myfile <<y   <<'	'<<  af  <<'	'<<gen<< '	'<<total_sources<<'	'<<endl;
  myfile.close();
  crossover(survivors,population,num,n);
}

void Square::display(int i, int j){   
  char Sq;
	if (this->nsources > 0) { Sq = '+';}
	else if (this->npatches) { Sq = '-';}
	else if (this->nrobots > 0) { Sq = 'R';}
	else {Sq = '.';}
}

bool Square::addRobot(int a,int b,int c,int d,int e, int f,int i,int j){
  this->nrobots++;
  total_robots++;
  return true;
}

bool Square::addEnergySources(int i, int j){
  if(this->npatches) nsources=0;
  else{
    int r = rand()%2;
    this->nsources = this->nsources+r;
    total_sources=total_sources+r;
    return true;
  }
}

bool Square::addPatches(int i, int j){
  if(this->npatches) return false;
  else{
    int r = rand()%2;
    if (r==1) this->npatches=true; else this->npatches=false;
    total_patches=total_patches+r;
    return true;
  }  
}

bool Square::step(){ 
}

bool Square::energySourceAt(int i, int j){
  if (env[i][j].nsources >0) return true;
  else return false;
}

bool Square::patchAt(int i, int j){
  if (env[i][j].npatches) return true;
  else return false;
}

//GENERAL
double * toDec(int* population){
  double m[GENES];
  int **temp = new int*[GENES];
  for(int k=0;k<GENES;k++)
  temp[k] = new int[GENES];
  int c=0;
  int G;
  
  for(int i=0;i<GENES;i++) {
    if(c==8 || c == 22)G=6;
	  else G=GENE_LENGTH;
    for(int j=0;j<G;j++) {
      temp[i][j] = population[c+j];
    }
    if(c==8 || c == 22) c=c+6;
    else c=c+GENE_LENGTH;
  }
  
  for(int i=0;i<GENES;i++) {
    m[i]=0;
    if(i == 2 || i == 5)G=6;
    else G = GENE_LENGTH;
    for(int p=0;p<G;p++) {
      int k = p;
      m[i] += temp[i][G-p-1]*pow(2.0,k);
    }
  }
  double * p = m;
  return p;
}

int main(void){
  srand((unsigned long) time(NULL));
  ofstream myfile;
  myfile.open("file.txt");
  Square ** env = new Square *[NROWS]; //allocate memory for grid
  for(int i=0;i<NROWS;i++) env[i] = new Square[NCOLS]; 
  //make a 40x40 grid and initialise a Square object in each cell
  for(int i=0;i<NROWS;i++){
    for(int j=0;j<NROWS;j++) env[i][j] = Square(i,j);
  }
   //Add energy sources and corrosive patches
  while(total_patches<350) {
    int i=rand()%40;
    int j=rand()%40;
    env[i][j].addPatches(i,j);
  }

  while(total_sources<3500) {
    int i=rand()%40;
    int j=rand()%40;
    env[i][j].addEnergySources(i,j);
  }
  
  int** population = new int*[POP];   // allocate memory for population
  for(int i=0;i<POP;i++){ 
    population[i] = new int[CHROMO_LENGTH];
  }
  initialise(population);

  double** params = new double*[POP];   // allocate memory for parameters
  for(int i=0;i<POP;i++){ 
    params[i] = new double[GENES];
  }
 
 Robot** robots;
 robots = new Robot *[POP];
 
 int gen = 0; 
 
 int iter=0; 
 //run the simulation until all resources are out
 while(gen<120) {
    //clean the grid 
    for(int i=0;i<NROWS;i++){
      for(int j=0;j<NCOLS;j++) env[i][j].nrobots=0;
    }
 
  //Add each individual to the grid
    for(int z=0;z<POP;z++){	
      params[z]  = toDec(population[z]);
      int a = params[z][0];
      int b = params[z][1];
      int c = params[z][2];
      int d = params[z][3];
      int e = params[z][4];
      int f = params[z][5];
	    //Add robots to randomly picked squares
	    int i=rand()%40;
	    int j=rand()%40;
	    env[i][j].addRobot(a,b,c,d,e,f,i,j);
	    robots[z] = new Robot(a,b,c,d,e,f,i,j,z);
	    if(env[i][j].nsources>0) {
	      if(robots[z]->energy < robots[z]->max_e) {
	        robots[z]->energy++; 
	        robots[z]->energy_gain++;
	        env[i][j].nsources--;
	        total_sources--; 
	      }
	    }
    if(env[i][j].npatches) {
	    robots[z]->casting--; 
	    robots[z]->cast_loss--;
	  }
  }

// this is to count the number of dead robots after each cycle (update) 
//number of times to run each turn / cycle. call GA after this loop is done so this is also a generation
    int half = POP/2; //50% of the population for the mating pool
    int dead=0;
    int num=0;
    for(int i=0;i<7;i++){
      //cycle or turn: move the population one by one
      for(int i=0;i<POP;i++){
        if (robots[i]->d==true) continue; //already dead, no need to move him
        else {
          int temp_coor[0];
          temp_coor[0] = robots[i]->x; temp_coor[1] = robots[i]->y;
          if(!robots[i]->step(robots[i])){
            total_robots--; dead++;
            env[robots[i]->x][robots[i]->y].nrobots--;
            robots[i]->d=true;
            continue;
          }
          else{ 
            env[temp_coor[0]][temp_coor[1]].nrobots--;
            env[robots[i]->x][robots[i]->y].nrobots++;
            if(env[robots[i]->x][robots[i]->y].nsources>0) {
	            if(robots[i]->energy < robots[i]->max_e) {
	              robots[i]->energy++;
	              robots[i]->energy_gain++;
	            }
		          env[robots[i]->x][robots[i]->y].nsources--;
		          total_sources--;
		        }
            if((env[robots[i]->x][robots[i]->y].npatches)) robots[i]->casing--;robots[i]->cast_loss--;
            if (robots[i]->casting<0 || robots[i]->energy<0){
              robots[i]->d=true;
              dead++;
              total_robots--;
              env[robots[i]->x][robots[i]->y].nrobots--;
            }
          }
        }
      }

//updates that take place with time
      for(int i=0;i<POP;i++){
        if(robots[i]->d==true) continue;
        else{ 
          robots[i]->e_res = robots[i]->e_res-robots[i]->e_rate;
	        if(robots[i]->e_res < 0) {
	          robots[i]->energy--;robots[i]->energy_loss--;
	          robots[i]->e_res = robots[i]->e_res_orig;
	        }
	        robots[i]->cast_res = robots[i]->cast_res-robots[i]->cast_rate;
	        if(robots[i]->cast_res < 0) {
    	      if(robots[i]->casting < robots[i]->max_cast){ robots[i]->casting++;robots[i]->cast_gain++;} robots[i]->cast_res = robots[i]->cast_res_orig;
	        }
	        if (robots[i]->casting<0 || robots[i]->energy<0) robots[i]->d=true;
	        if(robots[i]->d==true){
	          dead++;
	          total_robots--;
	          env[robots[i]->x][robots[i]->y].nrobots--;
	        }
	        else robots[i]->survived_for++;
        }
      }
      iter++;
    }//when to stop a cycle

    cout<<"Iterations: "<<iter<<endl;
    num = POP-dead;
    cout<<"NUM: "<<num<<endl;
    cout<<dead<<endl;

    //make a mating pool
    int** survivors = new int*[num];   // allocate memory for survived chromosomes
    for(int i=0;i<num;i++) survivors[i] = new int[CHROMO_LENGTH];
  
    Robot** survivors_phen;
    survivors_phen = new Robot *[num]; 
  
    int s=0; //survivors counter
    for(int i=0;i<NROWS;i++){
      for(int j=0;j<NCOLS;j++){
        if(env[i][j].nrobots>0){
          for(int z=0;z<env[i][j].nrobots;z++) {
            int id = getRobotId(i,j,robots);
            survivors[s] = population[id]; //an array of chromosomes corresponding to the survived robots for crossover etc.
            survivors_phen[s] = robots[id]; // an array of survived robots for fitness assessment
            if(num==1){ 
              survivors[0] = population[id]; //an array of chromosomes corresponding to the survived robots for crossover etc.
              survivors_phen[0] = robots[id]; // an array of survived robots for fitness assessment
              survivors[1] = population[id]; //an array of chromosomes corresponding to the survived robots for crossover etc.
              survivors_phen[1] = robots[id]; // an array of survived robots for fitness assessment
              for(int j=0;j<CHROMO_LENGTH;j++) {cout<<population[id][j]<<endl;
                cout<<robots[id]->e_rate <<'	'<<robots[id]->max_e <<'	' <<robots[id]->e_res<<'	' <<
                robots[id]->cast_rate <<'	' << robots[id]->max_cast <<'	'<< robots[id]->cast_res <<endl;break;
              }
            }
            s++;
          }
        } 
      }
    }

    if(num==1)break; 
    assessFitness(survivors_phen,gen,num,survivors,population);

    cout<<"New population:"<<endl;
    for(int i=0;i<POP;i++){
      for(int j=0;j<CHROMO_LENGTH;j++) cout<<population[i][j];
      cout<<endl;
    }  
    gen++;
  }
  cout<<"Resources are out!"<<endl<<total_sources<<endl<<"Total bots left "<<total_robots<<endl;
  return 0;
  }
