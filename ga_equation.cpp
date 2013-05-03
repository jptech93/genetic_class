#include<stdlib.h>
#include<iostream>
#include<limits> //for max values of float and double..
#include<vector>

#include"random.cpp"
#include"population.cpp"

using namespace std;

int TARGET=0;

void bstr_random(bstring &b){
	for(int i=0;i<b.length;i++)
		b[i]=random(0,1);
}

char decode(string seq){
	if(!seq.compare("0000"))
		return '0';
	if(!seq.compare("0001"))
		return '1';
	if(!seq.compare("0010"))
		return '2';
	if(!seq.compare("0011"))
		return '3';
	if(!seq.compare("0100"))
		return '4';
	if(!seq.compare("0101"))
		return '5';
	if(!seq.compare("0110"))
		return '6';
	if(!seq.compare("0111"))
		return '7';
	if(!seq.compare("1000"))
		return '8';
	if(!seq.compare("1001"))
		return '9';
	if(!seq.compare("1010"))
		return 0;
	if(!seq.compare("1011"))
		return '/';
	if(!seq.compare("1100"))
		return '+';
	if(!seq.compare("1110"))
		return '-';
	if(!seq.compare("1101"))
		return '*';
	if(!seq.compare("1111"))
		return 0;
}

char type(char c){
	if(c=='+'||c=='-'||c=='*'||c=='/')
		return 'o';
	else if(c==0)
		return 0;
	else
		return 'n';
}

void fitness(chromosome<bstring> &str){
	bool first=true;
	char c,c_prev;
	char c_type,prev_type;
	float num=0;
	for(int i=0;i<str.length;i++){
		c=decode((string)str[i]);
		c_type=type(c);
		if(first && c_type!='n'){
			continue;
		}
		else if(first && c_type=='n'){
			first=false;
			num=(int)c-48;
			prev_type=c_type;
			c_prev=c;
			continue;
		}
		if( (prev_type=='o' && c_type=='n') ){
			switch(c_prev){
					case '+':
						num+=(int)c-48;
						break;
					case '-':
						num-=(int)c-48;
						break;
					case '*':
						num*=(int)c-48;
						break;
					case '/':
						if (((int)c-48)!=0)
							num/=(int)c-48;
						break;
				}
		}
		prev_type=c_type;
		c_prev=c;
	}
	
	num-=TARGET;
	if(num<0)
		num=-num;
	
	if(num!=0)
		num=1/num;
	else
		num=numeric_limits<float>::max();		
		
	str.fitness=num;
	
}


string display(chromosome<bstring> &str){
	bool first=true;
	char c,c_prev;
	char c_type,prev_type;
	string seq="";
	for(int i=0;i<str.length;i++){
		c=decode((string)str[i]);
		c_type=type(c);
		if(first && c_type!='n'){
			continue;		
		}
		else if(first && c_type=='n'){
			first=false;
			seq.push_back(c);
			seq.append(" ");
			prev_type=c_type;
			c_prev=c;
			continue;
		}
		if (c_prev=='/' && ((int)c-48)==0){
			prev_type=c_type;
			c_prev=c;
			continue;
		}
		else if( (prev_type=='o' && c_type=='n') ){
			seq.push_back(c_prev);
			seq.append(" ");
			seq.push_back(c);
			seq.append(" ");
		}
		prev_type=c_type;
		c_prev=c;
	}
	return seq;
}


int main(){
	srand((unsigned)time(0));
	int n,i,mut,cross,elite;
	char defaults;
	cout<< "\nEnter a number:"<<"\n";
	cin>>TARGET;
	cout<<"\nTake defaults??\n";
	cin>>defaults;
	if (defaults=='y') {
		cross=80;mut=30;elite=15;n=10;i=600;
		cout<<"\nMaximum length of Equations:"<<n<<"\n";
		cout<<"\nPopulation Size:"<<i<<"\n";
		cout<<"\nCrossOver Probability:"<<cross<<"\n";
		cout<<"\nMutation Probability:"<<mut<<"\n";
		cout<<"\nElitism Probability:"<<elite<<"\n";
	}
	else{
		cout<<"\nEnter Maximum length of Equations\n";
		cin>>n;
		cout<<"\nEnter population size\n";
		cin>>i;
		cout<<"\nEnter crossover probability (0 to 100):\n";
		cin>>cross;
		cout<<"\nEnter mutation probability (0 to 100):\n";
		cin>>mut;
		cout<<"\nEnter elitism (0 to 100):\n";
		cin>>elite;		
	}
	
	population<bstring> p(i,n,cross,mut,elite,&bstr_random,&fitness,4);
	p.generate_random();
	vector<string> results;
	string temp;
	bool found;
	while(p.generation<300){
		found=false;
		p.calc_fitness();
		p.arrange();
		for(int i=p.pop_size-1;p.pop[i].fitness==numeric_limits<float>::max() && i>=0;i--){
			temp=display(p.pop[i]);
			for(int i=0;i<results.size();i++)
				if(!results.at(i).compare(temp))
					found=true;
			if(!found){
				cout<<"\n"<<temp;
				results.push_back(temp);
				p.generation=0;
			}
		}
		p.renew();
	}
	/*TEST CODE FOR DEBUGGING AHEAD */
	/*for(int i=0;i<p.pop_size;i++)
		cout<<(string)p.pop[i]<<"\n";
	chromosome<bstring> c(4,&bstr_random,3);
	for(int i=0;i<c.length;i++)
		c[i].set_bits("000",0);
	cout<<(string)c<<"\n";
	chromosome<bstring> d(4,&bstr_random,3);
	for(int i=0;i<d.length;i++)
		d[i].set_bits("111",0);
	cout<<(string)d<<"\n";
	chromosome<bstring> cr(4,&bstr_random,3);
	cr.sp_crossover(0,c,d,1,2,true);
	cout<<(string)cr<<"\n";
	cr.sp_crossover(1,c,d,1,2,true);
	cout<<(string)cr<<"\n\n";
	chromosome<string> a(4,NULL);
	for(int i=0;i<a.length;i++)
		a[i]="aab";
	chromosome<string> x(4,NULL);
	x=a;
	cout<<(string)x<<"\n";
	cout<<(string)a<<"\n";
	chromosome<string> b(4,NULL);
	for(int i=0;i<b.length;i++)
		b[i]="bba";
	chromosome<string> crr(4,NULL);
	crr.sp_crossover(0,a,b,1);
	cout<<(string)crr;*/
	return 0;
}

