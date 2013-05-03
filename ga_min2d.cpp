#include<iostream>
#include<vector>
#include<math.h>

#include"random.cpp"
#include"population.cpp"

using namespace std;

void rnd(int &a){
	a=random(-500,500);
}

void fitness(chromosome<int> &a){

	float y=(a[0])*(a[0])-3*(a[0])*(a[1]);
	y=-y;
	a.fitness=y;
	/*if(y!=0)
		a.fitness=1/y;
	else
		a.fitness=0;*/

}

int main(){
	srand((unsigned)time(0));
	population<int> p(800,2,75,35,5,&rnd,&fitness);
	p.generate_random();
	
	float min,pre_min=0;
	bool found;
	vector<int> v1,v2;
	
	while(p.generation<800){
		found=false;
		p.calc_fitness();
		p.arrange();
		min=p.pop[p.pop_size-1].fitness;
		//cout<<"min "<<min<<"  pre_min "<<pre_min<<"\n";
		if(min>pre_min){
			v1.clear();
			v2.clear();
			p.generation=0;
			pre_min=min;
		}
		for(int i=p.pop_size-1;p.pop[i].fitness==pre_min && i>=0;i--){
			found=false;
			for(int j=0;j<v1.size();j++)
				if(v1.at(j)==p.pop[i][0] && v2.at(j)==p.pop[i][1])
					found=true;
			if(!found){
				v1.push_back(p.pop[i][0]);
				v2.push_back(p.pop[i][1]);
			}
		}
		p.renew();
	}
	float result;
	result=-min;
	cout<<"min value= "<<result<<"\n";
	for(int j=0;j<v1.size();j++){		
		cout<<"at x = "<<v1.at(j)<<"  y = "<<v2.at(j)<<"\n";		
	}
	
	return 0;
}
