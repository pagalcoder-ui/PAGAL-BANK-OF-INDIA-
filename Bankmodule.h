#ifndef BANK_MODULE_H
#define BANK_MODULE_H

#include<iostream>
#include<string>
#include<sqlite3.h>
#include<vector>
#include<cstdlib>
#include<ctime>
using namespace std;

class BANKMODULE{
private:
 vector<int>ac_id;
 vector<string>ac_holder;
 vector<long long>ac_number;
 vector<float>ac_balance;
 vector<int>ac_pin;
 vector<string>ac_status;
 
public:
 int create_account(string name,int pin,float bal=0.0f){
	 sqlite3*db;
	 sqlite3_stmt*stmt;
	 sqlite3_open("bank_data.xdb",&db);
	 
	 string create=
	 "CREATE TABLE IF NOT EXISTS bank_data("
	 "AID INTEGER PRIMARY KEY AUTOINCREMENT,"
	 "ac_holder TEXT,"
	 "ac_number TEXT,"
	 "ac_bal REAL,"
	 "ac_status TEXT,"
	 "ac_pin INT);";
	 
	 sqlite3_exec(db,create.c_str(),nullptr,nullptr,nullptr);
	 
	 string insert="INSERT INTO bank_data(ac_holder,ac_number,ac_bal,ac_status,ac_pin)VALUES(?,?,?,?,?);";
	 sqlite3_prepare_v2(db,insert.c_str(),-1,&stmt,nullptr);
	 
	 ac_holder.push_back(name);
	 long long acn=1234567890+(int)ac_holder.size();
	 string status="ACTIVE";
	 
	 sqlite3_bind_text(stmt,1,name.c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_text(stmt,2,to_string(acn).c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_double(stmt,3,bal);
	 sqlite3_bind_text(stmt,4,status.c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_int(stmt,5,pin);
	 
	 if(sqlite3_step(stmt)==SQLITE_DONE){
        sqlite3_finalize(stmt);
	    sqlite3_close(db);
	    return 1;
	 }
	 return -1;
 }
 void getdata(){
	 ac_holder.clear();
	 ac_number.clear();
	 ac_balance.clear();
	 ac_pin.clear();
	 ac_status.clear();
	 
	 sqlite3*db;
	 sqlite3_stmt*stmt;
	 sqlite3_open("bank_data.xdb",&db);
	 
	 string select="SELECT AID,ac_holder,ac_number,ac_bal,ac_status,ac_pin FROM bank_data";
	 sqlite3_prepare_v2(db,select.c_str(),-1,&stmt,nullptr);
	 
	 while(sqlite3_step(stmt)==SQLITE_ROW){
		 
		 int id=sqlite3_column_int(stmt,0);ac_id.push_back(id);
		 string h=(const char*)sqlite3_column_text(stmt,1);ac_holder.push_back(h);
		 string n=(const char*)sqlite3_column_text(stmt,2);ac_number.push_back(stoll(n));
		 float bal=sqlite3_column_double(stmt,3);ac_balance.push_back(bal);
		 string status=(const char*)sqlite3_column_text(stmt,4);ac_status.push_back(status);
		 int pin=sqlite3_column_int(stmt,5);ac_pin.push_back(pin);
		 
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
 }
 void update_file(int uindex,float value=0,int update_type=0){
	 //UPDATE TYPE -0-DISABLE/1-BAL/2-PIN
	 if(update_type==1||update_type==2||update_type==0){
		 if(update_type==1)ac_balance[uindex]=value;
		 else if(update_type==2)ac_pin[uindex]=value;
		 else if(update_type==0)ac_status[uindex]="DISABLE";
		 
		 sqlite3*db;
		 sqlite3_stmt*stmt;
		 sqlite3_open("bank_data.xdb",&db);
		 
		 string update="UPDATE bank_data SET ac_bal=?,ac_pin=?,ac_status=? WHERE AID=?;";
		 sqlite3_prepare_v2(db,update.c_str(),-1,&stmt,nullptr);
		 sqlite3_bind_double(stmt,1,ac_balance[uindex]);
		 sqlite3_bind_int(stmt,2,ac_pin[uindex]);
		 sqlite3_bind_text(stmt,3,ac_status[uindex].c_str(),-1,SQLITE_TRANSIENT);
		 sqlite3_bind_int(stmt,4,ac_id[uindex]);
		 
		 sqlite3_step(stmt);
		 sqlite3_finalize(stmt);
		 sqlite3_close(db);
		 
		 cout<<"successfully updated!\n";
		 
		 
	}
 }
 int search_index(){
	 long long n;cout<<"Enter Account Number:";cin>>n;cin.ignore();
	 for(int i=0;i<(int)ac_holder.size();i++){
		 if(n==ac_number[i]){
			 if(ac_status[i]=="ACTIVE"){
				 return i;
			  }
			  else cout<<"kutte ye account active nahi hain!\n";
		  }
	  }
	  cout<<"kutte aisa koi account exist nahi karta!\n";
	  return -1;
 }
 int pin_verification(int uindex){
	 int p;cout<<"Enter Pin:";cin>>p;cin.ignore();
	 if(p==ac_pin[uindex])return uindex;
	 else{
		 cout<<"beta ye galat pin hain ja baap se puch kr Aa!\n";
		 return -1;
	 }
 }
 int withdraw(int uindex,float amt){
	 if(amt*(1.05)<=ac_balance[uindex]){
		 ac_balance[uindex]-=amt*(1.05);
		 update_file(uindex,ac_balance[uindex],1);
		 return uindex;
	 }
	 cout<<"beta teri aukat se bhar hain!\n";
	 return -1;
 }
 void deposite(int uindex,float amt){
	 ac_balance[uindex]+=amt;
	 cout<<"Successfully Deposited!\n";
	 cout<<"Transation ID:"<<rand()%100000000000<<endl;
	 update_file(uindex,ac_balance[uindex],1);
 }
 float getbal(int uindex){
	 return ac_balance[uindex];
 }
 long long getacn(int uindex){
	 return ac_number[uindex];
 }
	 

	 
		 

		 
 
};
#endif
	 
	 
	 
	 
