#include<iostream>
#include<vector>
#include<math.h>

#include"random.cpp"
#include"population.cpp"

using namespace std;

void rnd(int &a){
	a=random(-1000,1000);
}

void fitness(chromosome<int> &a){

	float y=sin((float)a[0]);
	y=-y;
	a.fitness=y;
	/*if(y!=0)
		a.fitness=1/y;
	else
		a.fitness=0;*/

}

int main(){
	srand((unsigned)time(0));
	population<int> p(800,1,75,35,5,&rnd,&fitness);
	p.generate_random();
	
	float min,pre_min=0;
	bool found;
	vector<int> v;
	
	while(p.generation<800){
		found=false;
		p.calc_fitness();
		p.arrange();
		min=p.pop[p.pop_size-1].fitness;
		//cout<<"min "<<min<<"  pre_min "<<pre_min<<"\n";
		if(min>pre_min){
			v.clear();
			p.generation=0;
			pre_min=min;
		}
		for(int i=p.pop_size-1;p.pop[i].fitness==pre_min && i>=0;i--){
			found=false;
			for(int j=0;j<v.size();j++)
				if(v.at(j)==p.pop[i][0])
					found=true;
			if(!found)
				v.push_back(p.pop[i][0]);
		}
		p.renew();
	}
	float result;
	result=-min;
	cout<<"min value= "<<result<<"\n";
	for(int j=0;j<v.size();j++){		
		cout<<"at x = "<<v.at(j)<<"\n";		
	}
	
	return 0;
}
