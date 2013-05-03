#ifndef CHROMO
#define CHROMO

#include<stdlib.h>
#include<assert.h>
#include<string>
#include<string.h>//for strlen
#include<typeinfo>//for typeid in template
#include<sstream>

#include"bstring"
#include"random.cpp"

using namespace std;

template <class T> class chromosome{
	private:
		T *genes;
	public:
		int length;
		int len_per_gene;	// for bstring
		double fitness; //for use by other classes or functions..
		
		void (*gen_random)(T &);
		
		chromosome(chromosome &c):genes(0),length(c.length),len_per_gene(c.len_per_gene),fitness(0),gen_random(c.gen_random)
		{
			genes=(T*)malloc(c.length*sizeof(T));
			for(int i=0;i<c.length;i++)
				new (genes+i) T(c[i]);
		}
		
		chromosome(int len,void (*rand)(T&),int gene_len=1):genes(0),length(len),len_per_gene(gene_len),fitness(0),gen_random(rand)
		{
			genes=(T*)malloc(len*sizeof(T));
			for(int i=0;i<len;i++)
				new (genes+i) T;
			
		}

// pnt is gene number WHICH if crossed to other parent and so are genes after it..
// pnt1 is point IN A gene WHICH is crossed over to other parent and so are others after it.. 
		void sp_crossover(bool _child,chromosome<T> &mate1,chromosome<T> &mate2,int pnt,int pnt1=0,bool combined=false){
			assert(mate1.length==mate2.length);
			assert(mate1.len_per_gene==mate2.len_per_gene);
			assert(len_per_gene==mate2.len_per_gene);
			assert(length==mate1.length);
			assert(pnt>=0);
			assert(pnt<=mate1.length-1);
			if(!_child){
				for(int i=0;i<pnt;i++)
					(*this)[i]=mate1[i];
				for(int i=pnt;i<mate1.length;i++)
					(*this)[i]=mate2[i];
			}
			
			if(_child){
				for(int i=0;i<pnt;i++)
					(*this)[i]=mate2[i];
				for(int i=pnt;i<mate1.length;i++)
					(*this)[i]=mate1[i];
			}
			
		}
		
		void mutate(int pnt,int pnt2=0, bool combined=false){
			gen_random(genes[pnt]);
		}
		
		string get_seq(){
			stringstream ss;
			for(int i=0;i<length;i++)
				ss << (*this)[i]<<" ";
			return ss.str();
		}
		
		operator string(){
			return get_seq();
		}
		
		T& operator[](int n){
			return genes[n];
		}
		
		chromosome &operator=(chromosome &c){
// REALLOCATION DOESN'T WORK ON STRINGS ??!!
			if(length!=c.length)
				genes=(T*)realloc(genes,c.length*sizeof(T));
			length=c.length;
			len_per_gene=c.len_per_gene;
			gen_random=c.gen_random;
			fitness=c.fitness;
			for(int i=0;i<length;i++)
				genes[i]=c[i];
			return *this;
		}
		
		~chromosome(){
			for(int i=0;i<length;i++)
					(genes+i)->~T();
			free(genes);
		}
};






template <> class chromosome<bstring>{
	private:
		bstring *genes;
	public:
		int length;
		int len_per_gene;	// for bstring
		double fitness; //for use by other classes or functions..
		
		void (*gen_random)(bstring &);
		
		chromosome(chromosome &c):genes(0),length(c.length),len_per_gene(c.len_per_gene),fitness(0),gen_random(c.gen_random)
		{
			genes=(bstring*)malloc(c.length*sizeof(bstring));
			for(int i=0;i<c.length;i++)
				new (genes+i) bstring(c[i]);
		}
		
		chromosome(int len,void (*rand)(bstring &),int gene_len=1):genes(0),length(len),len_per_gene(gene_len),fitness(0),gen_random(rand)
		{
			genes=(bstring*)malloc(len*sizeof(bstring));
			for(int i=0;i<len;i++)
				new (genes+i) bstring(gene_len);
			
		}
// COMBINED FUNCTION IS CURRENTLY FOR BSTRING.... HAS TO BE CHANGED FOR STRING OR ANY OTHER!!!
// pnt is gene number WHICH if crossed to other parent and so are genes after it..
// pnt1 is point IN A gene WHICH is crossed over to other parent and so are others after it.. 
		void sp_crossover(bool _child,chromosome<bstring> &mate1,chromosome<bstring> &mate2,int pnt,int pnt1=0,bool combined=false){
			assert(mate1.length==mate2.length);
			assert(mate1.len_per_gene==mate2.len_per_gene);
			assert(len_per_gene==mate2.len_per_gene);
			assert(length==mate1.length);
			assert(pnt>=0);
			assert(pnt<=mate1.length-1);
			assert(pnt1>=0);
			assert(pnt1<=mate1.len_per_gene-1);
			if(!_child){
				for(int i=0;i<pnt;i++)
					(*this)[i]=mate1[i];
				for(int i=pnt;i<mate1.length;i++)
					(*this)[i]=mate2[i];
				
				if(combined){
					for(int i=0;i<pnt1;i++)
						(genes[pnt])[i]=(mate1.genes[pnt])[i];
					for(int i=pnt1;i<mate1.len_per_gene;i++)
						(genes[pnt])[i]=(mate2.genes[pnt])[i];
				}
			}
			
			if(_child){
				for(int i=0;i<pnt;i++)
					(*this)[i]=mate2[i];
				for(int i=pnt;i<mate1.length;i++)
					(*this)[i]=mate1[i];
					
				if(combined){
					for(int i=0;i<pnt1;i++)
						(genes[pnt])[i]=(mate2.genes[pnt])[i];
					for(int i=pnt1;i<mate1.len_per_gene;i++)
						(genes[pnt])[i]=(mate1.genes[pnt])[i];
				}
			}
			
		}
		
		void mutate(int pnt, int pnt2=0, bool combined=false){
			bstring rnd(len_per_gene);
			gen_random(rnd);
			if(combined)
				(genes[pnt])[pnt2]=rnd[pnt2];
			else
				genes[pnt]=rnd;
		}
		
		string get_seq(){
			string s="";
			for(int i=0;i<length;i++){
				s.append((string)((*this)[i]));
				s.append(" ");
			}
			return s;
		}
		
		operator string(){
			return get_seq();
		}
		
		bstring &operator[](int n){
			return genes[n];
		}
		
		chromosome &operator=(chromosome &c){
			if(length!=c.length)
				genes=(bstring*)realloc(genes,c.length*sizeof(bstring));
			length=c.length;
			len_per_gene=c.len_per_gene;
			gen_random=c.gen_random;
			fitness=c.fitness;
			for(int i=0;i<length;i++)
				genes[i]=c[i];
			return *this;
		}
		
		~chromosome(){
			for(int i=0;i<length;i++)
					(genes+i)->~bstring();
			free(genes);
		}
};



#endif
