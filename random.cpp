#ifndef RND
#define RND

#include<stdlib.h>
#include<time.h>

using namespace std;

inline int random(int lowest,int highest){
	
    //srand((unsigned)time(0)); 
    int rnd=rand()%10;
    if (rnd>3)
    	srand(rand()*rand()*time(0));
    int random_integer; 
    int range=(highest-lowest)+1; 
    //for(int index=0; index<20; index++)
    //random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0)); 
    random_integer=lowest+(rand()%(range));
    return random_integer;
}

#endif
