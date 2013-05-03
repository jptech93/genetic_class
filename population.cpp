#ifndef POP
#define POP

#include<stdlib.h>
#include<assert.h>
#include<string>
#include<string.h>//for strlen
#include<typeinfo>//for typeid in template
#include<sstream>

#include"chromosome.cpp"

using namespace std;


template<class T> class population{
	private:
		
	public:
		chromosome<T> *pop,*old_pop;
		int pop_size,chromo_len,gene_len;
		short crov_prob,mut_prob,elitism;
		double fitness_sum;
		int generation;
		void (*fitness)(chromosome<T> &);
		
		// probabilities are in percentage (0 to 100)..
		population(int size,int chr_len,short cr_prob,short mu_prob,short elite,void (*rnd)(T &),void (*fit)(chromosome<T>&),int gen_len=1):
		pop(0),old_pop(0),pop_size(size),chromo_len(chr_len),gene_len(gen_len),crov_prob(cr_prob),mut_prob(mu_prob),elitism(elite),fitness_sum(0),generation(0),fitness(fit)
		{
			pop=(chromosome<T>*)malloc((size+1)*sizeof(chromosome<T>));
			old_pop=(chromosome<T>*)malloc((size+1)*sizeof(chromosome<T>));
			for(int i=0;i<=size;i++){
				new (pop+i) chromosome<T>(chromo_len,rnd,gene_len);
				new (old_pop+i) chromosome<T>(chromo_len,rnd,gene_len);
			}
		}
		
		void calc_fitness(){
			fitness_sum=0;
			for(int i=0;i<pop_size;i++){
				fitness(pop[i]);
				fitness_sum+=pop[i].fitness;
			}
		}
		
		void arrange(){
			chromosome<T> temp(chromo_len,pop[0].gen_random,gene_len);
			for(int i=0;i<pop_size;i++)
				for(int j=0;j<pop_size-i-1;j++)
					if(pop[j].fitness>pop[j+1].fitness){
						temp=pop[j];
						pop[j]=pop[j+1];
						pop[j+1]=temp;
					}
		}
		
		void generate_random(){
			generation++;
			for(int i=0;i<pop_size;i++)
				for(int j=0;j<chromo_len;j++)
					pop[i].gen_random((pop[i])[j]);
		}		
// RW_SELECT DOESN'T WORK!!!!		
		chromosome<T> &rw_select(chromosome<T> *p){
			int rnd=random(0,fitness_sum-1);
			double sum=0;
			int i;
			for(i=0;sum<rnd;i++)
				sum+=p[i].fitness;
			return p[i];
		}
// MUST CALCULATE FITNESS AND ARRANGE BEFORE USING RANK SELECTION...
		chromosome<T> &rank_select(chromosome<T> *p){
			int rnd=random(0,pop_size*(pop_size-1)/2);
			double sum=0;
			int i;
			for(i=0;sum<rnd;i++)
				sum+=i;
			return p[i];
		}
		
		
		// bool as parameter to select between rank_select and rw_select
		// bool combined is only for bstring...
		void renew(bool rank=true,bool combined=false){
		
			generation++;
			
			if(typeid(T)!=typeid(bstring))
				combined=false;
				
			calc_fitness();
			arrange();
			
			// swap population
			void *temp;
			temp=pop;
			pop=old_pop;
			old_pop=(chromosome<T>*)temp;
			
			// keep best..
			pop[0]=old_pop[pop_size-1];
			pop[1]=old_pop[pop_size-2];
			
			short cross;
			short mut;
			
			int keep_both;
			int rnd,rnd1;
			
			for(int i=2; i<pop_size;i++){
					cross=random(0,100);
					mut=random(0,100);
					
					keep_both=random(0,100);
					
					chromosome<T> *mate1;
					chromosome<T> *mate2;
					if(rank){
						mate1=&(rank_select(old_pop));
						mate2=&(rank_select(old_pop));
					}
					else{
						mate1=&(rw_select(old_pop));
						mate2=&(rw_select(old_pop));
					}
					if(cross<=crov_prob){
						rnd=random(0,chromo_len-1);
						rnd1=random(0,gene_len-1);
						pop[i].sp_crossover(0,*mate1,*mate2,rnd,rnd1,combined);
						if(keep_both<=elitism)
							pop[i+1].sp_crossover(1,*mate1,*mate2,rnd,rnd1,combined);
					}
					
					else{
						bool rndm=random(0,1);
						if(rndm){
							pop[i]=*mate1;
							if(keep_both<=elitism)
								pop[i+1]=*mate2;
						}
						else{
							pop[i]=*mate2;
							if(keep_both<=elitism)
								pop[i+1]=*mate1;
						}
					}
					
					if(mut<=mut_prob){
						rnd=random(0,chromo_len-1);
						rnd1=random(0,gene_len-1);
						pop[i].mutate(rnd,rnd1,combined);
						if(keep_both<=elitism)
							pop[i+1].mutate(rnd,rnd1,combined);
					}
					
					if(keep_both<=elitism)
						i++;

			}
		}
		
		~population(){
			for(int i=0;i<pop_size;i++){
				pop[i].~chromosome<T>();
				old_pop[i].~chromosome<T>();
			}
			free(pop);
			free(old_pop);
		}
		
};



#endif
