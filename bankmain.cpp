#include<iostream>
#include"Bankmodule.h"
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<ctime>


class BOOT{
private:
 BANKMODULE BM;
 int uindex=-1;
 int eindex=-1;

public:
 void signup(){
	 BM.getdata();
	 string name;cout<<"Enter Name:";getline(cin,name);
	 int pin;cout<<"SET PIN:";cin>>pin;cin.ignore();
	 BM.create_account(name,pin);
 }
 void t_history(string sender,string amount_type,float amt,long long acn,long long tid){
	 //SENDER|AMOUNT TYPE|AMOUNT|AC NUMBER|TRANSATION ID if-withdrwal/deposit-SENDER SELF
	 
	 ofstream Tfile("transation.xdb",ios::app);
	 Tfile<<sender<<"|"<<amount_type<<"|"<<amt<<"|"<<acn<<"|"<<tid<<endl;
 }
 void gethistory(int uindex){
	 ifstream of("transation.xdb");
	 string a,b,c,d,e;
	 tformat();
	 while(getline(cin,a,'|') && getline(cin,b,'|') && getline(cin,c,'|') &&
	       getline(cin,d,'|') && getline(cin,e)){
			   if(stoll(a)==BM.getacn(uindex)){
				   if(a==d){
					   a="SELF";
					   show(a,b,c,d,e);
				   }
					   
				   
				}
	}
	of.close();
 }
 void show(string a,string b,string c,string d,string e){
	 cout<<left<<setw(25)<<a
	     <<left<<setw(25)<<b
	     <<left<<setw(12)<<c
	     <<left<<setw(25)<<d
	     <<left<<e<<endl;
 }
 void tformat(){
	 cout<<left<<setw(25)<<"SENDER"
	     <<left<<setw(25)<<"AMOUNT TYPE"
	     <<left<<setw(12)<<"AMOUNT"
	     <<left<<setw(25)<<"AC NUMBER"
	     <<left<<"TRANSATION ID\n";
 }
		
	 
 void start(){
	 int c;
	 BM.getdata();
	 while(true){
		 cout<<"\n||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n";
		 cout<<"||                 PAGAL BANK OF INDIA                    ||\n";
		 cout<<"||________________________________________________________||\n";
		 cout<<"||        SINCE 2026  | POWERD BY-PAGAL ECO SYSTEM        ||\n";
		 cout<<"||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"; 
		 cout<<"\n1.SIGNUP\n2.LOGIN\n0.EXIT\n";
		 cout<<"Enter Choice:";cin>>c;cin.ignore();
		 
		 if(c==1)signup();
		 else if(c==2){
			 uindex=BM.search_index();
			 if(uindex!=-1){
				 uindex=BM.pin_verification(uindex);
				 if(uindex!=-1){
					 while(true){
						 cout<<"\n||=======MENU=======||\n";
						 cout<<"||  1.SEND MONEY    ||\n";
						 cout<<"||  2.ADD MONEY     ||\n";
						 cout<<"||  3.GET CASH      ||\n";
						 cout<<"||  4.CHECK BALANCE ||\n";
						 cout<<"||  5.DISABLE/DELETE||\n";
						 cout<<"||  6.PIN CHANGE    ||\n";
						 cout<<"||  7.GET HISTORY   ||\n";
						 cout<<"||  0.EXIT          ||\n";
						 cout<<"||__________________||\n";
						 
						 cout<<"Enter Choice:";cin>>c;cin.ignore();
						 
						 if(c==1){
							 eindex=BM.search_index();
							 if(eindex!=-1){
								 uindex=BM.pin_verification(uindex);
								 if(uindex!=-1){
									 float amt;cout<<"Enter Amount:";cin>>amt;cin.ignore();
									 BM.withdraw(uindex,amt);
									 BM.deposite(eindex,amt);
									 srand(time(0));
									 long long Tid=rand()%10000000000;
									 t_history(to_string(BM.getacn(uindex)),"SEND",amt,BM.getacn(eindex),Tid);
								     t_history(to_string(BM.getacn(eindex)),"RECIVE",amt,BM.getacn(uindex),Tid);
								     
								    
								 }
							  }
						  }
						  else if(c==2){
							  float amt;cout<<"Enter amount:";cin>>amt;cin.ignore();
							  BM.deposite(uindex,amt);
							  srand(time(0));
							  long long Tid=rand()%10000000000;
							  t_history(to_string(BM.getacn(uindex)),"DEPOSITE",amt,BM.getacn(uindex),Tid);
						  }
						  else if(c==3){
							  float amt;cout<<"Enter amount:";cin>>amt;cin.ignore();
							  uindex=BM.pin_verification(uindex);
							  if(uindex!=-1){
								  BM.withdraw(uindex,amt);
								  cout<<"successfully withdrawal!\n";
								  srand(time(0));
								  int Tid=rand()%100000000000;
								  t_history(to_string(BM.getacn(uindex)),"WITHDAWAL",amt,BM.getacn(uindex),Tid);
								  
								 
							  }
						  }
						  else if(c==4)cout<<"Your Bal:"<<BM.getbal(uindex);
						  else if(c==5){
							  BM.update_file(uindex);
							  cout<<"Your Account Successfully disabled!\n";
							  break;
						  }
						  else if(c==6){
							  uindex=BM.pin_verification(uindex);
							  if(uindex!=-1){
								  int pin;cout<<"Enter New Pin:";cin>>pin;
								  BM.update_file(uindex,pin,2);
							  }
						  }
						  else if(c==7){
							  gethistory(uindex);
						  }
						  else if(c==0)break;
						  else cout<<"aisa koi option nahi hain andhe!\n";
					}
				}
			}
		}else break;
	}
 }
};

int main(){
	BOOT boot;
	boot.start();
}
						  
							 
				 
		 
		 
