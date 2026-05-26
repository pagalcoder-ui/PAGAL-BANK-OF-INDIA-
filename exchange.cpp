#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<sqlite3.h>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

class STOCKMODULE{
protected:
 vector<string>stock_name;
 vector<long long>stock_marketcap;
 vector<float>stock_price;
 vector<int>stock_units;
 vector<int>stock_listing_year;
 vector<int>stock_uid;
 
 
public:
 void listing_stock(){
	 string data[5];
	 cout<<"Enter Stock Name:";getline(cin,data[0]);stock_name.push_back(data[0]);
	 cout<<"Enter Market cap:";getline(cin,data[1]);stock_marketcap.push_back(stoll(data[1]));
	 cout<<"Enter Stock unit:";getline(cin,data[2]);stock_units.push_back(stoi(data[2]));
	 cout<<"Per Share Price :"<<stoll(data[1])/stoi(data[2])<<endl;
	 data[3]=to_string(stoll(data[1])/stoi(data[2]));stock_price.push_back(stoi(data[3]));
	 cout<<"Enter Listing Year:";getline(cin,data[4]);stock_listing_year.push_back(stoi(data[4]));
	 cout<<"Stock_Unique ID :"<<(int)stock_name.size()<<endl;stock_uid.push_back((int)stock_name.size());
	 
	 sqlite3*stock_pointer;
	 sqlite3_stmt*stock_machine;
	 
	 sqlite3_open("stock.zip",&stock_pointer);
	 string stock_table=
	 "CREATE TABLE IF NOT EXISTS STOCK_DATA("
	 "SID INTEGER PRIMARY KEY AUTOINCREMENT,"
	 "S_name TEXT,"
	 "S_mcap TEXT,"
	 "S_unit INT,"
	 "S_price INT,"
	 "S_lyear INT);";
	 
	 sqlite3_exec(stock_pointer,stock_table.c_str(),nullptr,nullptr,nullptr);
	 
	 string stock_datatable="INSERT INTO STOCK_DATA(S_name,S_mcap,S_unit,S_price,S_lyear)VALUES(?,?,?,?,?);";
	 sqlite3_prepare_v2(stock_pointer,stock_datatable.c_str(),-1,&stock_machine,nullptr);
	 
	 sqlite3_bind_text(stock_machine,1,data[0].c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_text(stock_machine,2,data[1].c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_int(stock_machine,3,stoi(data[2]));
	 sqlite3_bind_int(stock_machine,4,stoi(data[3]));
	 sqlite3_bind_int(stock_machine,5,stoi(data[4]));
	 
	 sqlite3_step(stock_machine);
	 sqlite3_finalize(stock_machine);
	 cout<<"Successfully Listed!\n";
 }
 void get_fullstockdata(){
	 stock_name.clear();
	 stock_marketcap.clear();
	 stock_price.clear();
	 stock_units.clear();
	 stock_listing_year.clear();
	 stock_uid.clear();
	 
	 sqlite3*stock_pointer;
	 sqlite3_stmt*stock_machine;
	 sqlite3_open("stock.zip",&stock_pointer);
	 
	 string get_stable="SELECT SID,S_name,S_mcap,S_unit,S_price,S_lyear FROM STOCK_DATA";
	 sqlite3_prepare_v2(stock_pointer,get_stable.c_str(),-1,&stock_machine,nullptr);
	 
	 while(sqlite3_step(stock_machine)==SQLITE_ROW){
		 
		 int sid=sqlite3_column_int(stock_machine,0);stock_uid.push_back(sid);
		 string sn=(const char*)sqlite3_column_text(stock_machine,1);stock_name.push_back(sn);
		 string smcp=(const char*)sqlite3_column_text(stock_machine,2);stock_marketcap.push_back(stoll(smcp));
		 int su=sqlite3_column_int(stock_machine,3);stock_units.push_back(su);
		 int sp=sqlite3_column_int(stock_machine,4);stock_price.push_back(sp);
		 int sly=sqlite3_column_int(stock_machine,5);stock_listing_year.push_back(sly);
		 
	}
	sqlite3_finalize(stock_machine);
	sqlite3_close(stock_pointer);
	
 }
 void update_sharemarket(int index,int price=0,int update_type=1){
	 //update type -price-1 /delisting -0
	 if(update_type==1){
		 sqlite3*stock_pointer;
		 sqlite3_stmt*stock_machine;
		 sqlite3_open("stock.zip",&stock_pointer);
		 
		 string update_stock ="UPDATE STOCK_DATA SET S_mcap=?,S_price=? WHERE SID=?;";
		 sqlite3_prepare_v2(stock_pointer,update_stock.c_str(),-1,&stock_machine,nullptr);
		 long long mcap=price*stock_units[index];
		 stock_marketcap[index]=mcap;
		 
		 sqlite3_bind_text(stock_machine,1,to_string(mcap).c_str(),-1,SQLITE_TRANSIENT);
		 sqlite3_bind_int(stock_machine,2,price);
		 sqlite3_bind_int(stock_machine,3,stock_uid[index]);
		 
		 sqlite3_step(stock_machine);
		 sqlite3_finalize(stock_machine);
		 sqlite3_close(stock_pointer);
		 
	 }
	 else if(update_type==0){
		 sqlite3*stock_pointer;
		 sqlite3_stmt*stock_machine;
		 sqlite3_open("stock.zip",&stock_pointer);
		 
		 string delete_stock="DELETE FROM STOCK_DATA WHERE SID=?;";
		 sqlite3_prepare_v2(stock_pointer,delete_stock.c_str(),-1,&stock_machine,nullptr);
		 sqlite3_bind_int(stock_machine,1,stock_uid[index]);
		 
		 sqlite3_step(stock_machine);
		 sqlite3_finalize(stock_machine);
		 sqlite3_close(stock_pointer);
		 cout<<"Successfully delisted!\n";
		 
	 }
	 
 }
 void start_volume(int index){
	 srand(time(0));
	 
	 float*p=&stock_price[index];
	 getstockformat();
	 int i=0;
	 while(i<100){
		 float move=(((rand()%11)-5)/100.0f);
		 float change=stock_price[index]*move;
		 *p=stock_price[index]+=change;
		 update_sharemarket(index,stock_price[index],1);
		 get_fullstockdata();
		 getstockdet(index,move);
		 if(*p==0){
			 update_sharemarket(index,0,0);
			 cout<<"Ye Company ab loot chuki hain barbad ho chuki hain!\n";
			 break;
		}
		i++;
	 }
 }
 void getstockformat(){
	 cout<<left<<setw(20)<<"NAME"
	     <<left<<setw(20)<<"MARKET CAP"
	     <<left<<setw(20)<<"SHARE UNIT"
	     <<left<<setw(20)<<"SHARE PRICE"
	     <<left<<setw(20)<<"YEAR"
	     <<left<<setw(20)<<"SID"
	     <<left<<setw(1)<<"MARKET CHANGES"<<endl;
 }
 void getstockdet(int index,float a=0){
	 cout<<left<<setw(20)<<stock_name[index]
	     <<left<<setw(20)<<stock_marketcap[index]
	     <<left<<setw(20)<<stock_units[index]
	     <<left<<setw(20)<<stock_price[index]
	     <<left<<setw(20)<<stock_listing_year[index]
	     <<left<<setw(20)<<stock_uid[index]
	     <<left<<setw(1)<<a<<"%"<<endl;
 }
 int getstockindex(){
	 int sid;cout<<"Enter Stock SID:";cin>>sid;cin.ignore();
	 for(int i=0;i<(int)stock_name.size();i++){
		 if(sid==stock_uid[i])return i;
	 }
	 cout<<"Kutte aisa koi stock nahi hain!\n";
	 return -1;
 }
 void getallstocklist(){
	 getstockformat();
	 for(int i=0;i<(int)stock_name.size();i++){
		 getstockdet(i);
	 }
 }
	 
 
};
class BROKER{
private:
 STOCKMODULE STML;
 int sindex=-1;
 
public:
 void star(){
	 STML.get_fullstockdata();
	 int choice;
	 while(true){
		 cout<<"\n[[[[[[[[[[[[ PAGAL SHARE MARKET ]]]]]]]]]]]]\n";
		 cout<<"Registerd in SEBI\n";
		 cout<<"Our Partner:-Pagal bank of india\n";
		 cout<<"\n1.New Listing\n2.ALL stock\n3.Check Market\n4.Search Stock\n0.Exit\n";
		 cout<<"\nEnter choice:";cin>>choice;cin.ignore();
		 
		 if(choice==1)STML.listing_stock();
		 else if(choice==2)STML.getallstocklist();
		 else if(choice==4){
			 sindex=STML.getstockindex();
			 if(sindex!=-1){
				 STML.getstockformat();
				 STML.getstockdet(sindex);
			  }
		 }
		 else if(choice==3){
			 sindex=STML.getstockindex();
			 if(sindex!=-1)STML.start_volume(sindex);
		 }
		 else if(choice==0)break;
		 else cout<<"\nAndha broker aisa koi option nahi hain!\n";
	}
 }
};
class USER_STOCK:public STOCKMODULE{
private:
 vector<string>user_name;
 vector<long long>user_dacn;
 vector<float>user_wallet;
 vector<int>UID;
 vector<int>pin;
 
 vector<vector<string>>sharename;
 vector<vector<int>>shareunit;
 vector<vector<int>>shareid;
 
 int lindex=-1;
 int pindex=-1;
 int sindex=-1;
public:
 void user_signup(){
	 string data[3];
	 cout<<"Enter Name:";getline(cin,data[0]);user_name.push_back(data[0]);
	 srand(time (0));
	 long long dacn=rand()%10000000000;
	 cout<<"Your Demat A/c no:"<<dacn<<endl;user_dacn.push_back(dacn);
	 cout<<"Enter Amont:";getline(cin,data[1]);user_wallet.push_back(stof(data[1]));
	 cout<<"Your UID:"<<(int)user_name.size()<<endl;UID.push_back((int)user_name.size());
	 cout<<"SET PIN:";getline(cin,data[2]);pin.push_back(stoi(data[2]));
	 
	 sqlite3*udb;
	 sqlite3_stmt*insert_machine;
	 sqlite3_open("user.xdb",&udb);
	 
	 string user_table=
	 "CREATE TABLE IF NOT EXISTS user_data("
	 "UID INTEGER PRIMARY KEY AUTOINCREMENT,"
	 "name TEXT,"
	 "dcn TEXT,"
	 "amt INT,"
	 "pin INT);";
	 sqlite3_exec(udb,user_table.c_str(),nullptr,nullptr,nullptr);
	 
	 string user_itable="INSERT INTO user_data(name,dcn,amt,pin)VALUES(?,?,?,?);";
	 sqlite3_prepare_v2(udb,user_itable.c_str(),-1,&insert_machine,nullptr);
	 sqlite3_bind_text(insert_machine,1,data[0].c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_text(insert_machine,2,to_string(dacn).c_str(),-1,SQLITE_TRANSIENT);
	 sqlite3_bind_int(insert_machine,3,stoi(data[1]));
	 sqlite3_bind_int(insert_machine,4,stoi(data[2]));
	 sqlite3_step(insert_machine);
	 sqlite3_finalize(insert_machine);
	 cout<<"Successfully Registerd!\n";
 }
 void get_userdata(){
	 user_name.clear();
	 user_wallet.clear();
	 UID.clear();
	 pin.clear();
	 user_dacn.clear();
	 
	 sqlite3*udb;
	 sqlite3_stmt*read_machine;
	 sqlite3_open("user.xdb",&udb);
	 string get_data="SELECT UID,name,dcn,amt,pin FROM user_data";
	 sqlite3_prepare_v2(udb,get_data.c_str(),-1,&read_machine,nullptr);
	 
	 while(sqlite3_step(read_machine)==SQLITE_ROW){
		 
		 int id=sqlite3_column_int(read_machine,0);UID.push_back(id);
		 string name=(const char*)sqlite3_column_text(read_machine,1);user_name.push_back(name);
		 string dacn=(const char*)sqlite3_column_text(read_machine,2);user_dacn.push_back(stoll(dacn));
		 int amt=sqlite3_column_int(read_machine,3);user_wallet.push_back(amt);
		 int p=sqlite3_column_int(read_machine,4);pin.push_back(p);
	}
	sqlite3_finalize(read_machine);
	sqlite3_close(udb);
	
 }
 void update_user_data(int index,int newbal){
	 sqlite3*udb;
	 sqlite3_stmt*update_machine;
	 sqlite3_open("user.xdb",&udb);
	 
	 string update_table="UPDATE user_data SET amt=? WHERE UID=?;";
	 sqlite3_prepare_v2(udb,update_table.c_str(),-1,&update_machine,nullptr);
	 sqlite3_bind_int(update_machine,1,newbal);
	 sqlite3_bind_int(update_machine,2,UID[index]);
	 sqlite3_step(update_machine);
	 sqlite3_finalize(update_machine);
	 sqlite3_close(udb);
	 
	 get_userdata();
	 
	 
 }
 void buyshare(int shareindex,int userindex,int unit){
	 sharename[userindex].push_back(stock_name[shareindex]);
	 shareunit[userindex].push_back(unit);
	 shareid[userindex].push_back(stock_uid[shareindex]);
	 
	 ofstream sharedata("share.data",ios::app);
	 sharedata<<UID[userindex]<<"|"<<stock_name[shareindex]<<"|"<<unit<<"|"<<stock_uid[shareindex]<<endl;
	 
	 cout<<"Successfully added shares!\n";
	 
 }
 void getsharedata(){
	 sharename.clear();sharename.resize(100);
	 shareunit.clear();shareunit.resize(100);
	 shareid.clear();shareid.resize(100);
	 ifstream osd("share.data");
	 string a,b,c,d;
	 while(getline(osd,a,'|') && getline(osd,b,'|') && getline(osd,c,'|') && getline(osd,d)){
		 sharename[stoi(a)].push_back({b});
		 shareunit[stoi(a)].push_back({stoi(c)});
		 shareid[stoi(a)].push_back({stoi(d)});
	  
	}
	osd.close();
 }
 void sellshare(int shareindex,int userindex){
	 for(int i=0;i<(int)sharename[userindex].size();i++){
		 if(shareid[userindex][i]==stock_uid[shareindex]){
			 user_wallet[userindex]+=shareunit[userindex][i]*stock_price[shareindex];
			 
			 shareid[userindex].erase(shareid[userindex].begin()+i);
			 sharename[userindex].erase(sharename[userindex].begin()+i);
			 shareunit[userindex].erase(shareunit[userindex].begin()+i);
			 update_sharefile();
			 getsharedata();
			 
			 cout<<"Sucessfully selled!\n";
		 }
		 
	 }
 }
 void update_sharefile(){
	 ofstream ups("share.data");
	 for(int i=0;i<(int)sharename.size();i++){
		 for(int j=0;j<(int)sharename[i].size();j++){
			 ups<<UID[i]<<"|"<<sharename[i][j]<<"|"<<shareunit[i][j]<<"|"<<stock_uid[j]<<endl;
		 }
	}
	ups.close();
 }
 int pin_verification(int index){
	 int p;cout<<"Enter PIN:";cin>>p;cin.ignore();
	 if(pin[index]==p){
		 return 1;
	 }else{
		 cout<<"Kutte/kutti galat pin dalte ho!\n";
		 return -1;
	 }
 }
 int login(){
	 long long ui;cout<<"Enter Demat/UID :";cin>>ui;cin.ignore();
	 for(int i=0;i<(int)user_name.size();i++){
		 if(ui==user_dacn[i]||ui==UID[i]){
			 return i;
		 }
	 }
	 cout<<"kutte aisa koi Account nahi hain!\n";
	 return -1;
 }
 void buy_shares(int uindex){
	 sindex=getstockindex();
	 if(sindex!=-1){
		 int u;cout<<"Enter Unit:";cin>>u;cin.ignore();
		 int amt=stock_price[sindex]*u;
		 cout<<"Your Total Bill:"<<amt<<endl;
		 pindex=pin_verification(uindex);
		 if(pindex!=-1){
			 if(amt<=user_wallet[uindex]){
				 user_wallet[uindex]-=amt;
				 buyshare(sindex,uindex,u);
				 update_user_data(uindex,user_wallet[uindex]);
				 cout<<"successfully buyed!\n";
			 }else cout<<"kutte teri aukat se bhar hain!\n";
		 }
	}
 }
 void deposite(int index){
	 int amt;cout<<"Enter Amount:";cin>>amt;
	 user_wallet[index]+=amt*(0.95);
	 update_user_data(index,user_wallet[index]);
	 cout<<"Successfully Deposite!\n";
	 cout<<"Transation Id:"<<&amt<<endl;
 }
 void getinvestment(int index){
	 for(int i=0;i<(int)sharename.size();i++){
		 get_share_data(index,i);
		 
	 }
 }
 void get_share_data(int index,int i){
	 cout<<left<<setw(30)<<sharename[index][i]
	     <<left<<setw(30)<<shareunit[index][i]
	     <<left<<setw(30)<<shareid[index][i]<<endl;
 }
 void start(){
	 get_fullstockdata();
	 get_userdata();
	 getsharedata();
	 int choice;
	 while(true){
		 cout<<"\n[[[[[[[[[[[[[[[[[[ PAGAL EXCHANGE LIMITED ]]]]]]]]]]]]]]]]]]\n";
		 cout<<"Registed in SEBI & FIU \n";
		 cout<<"\n1.Create Account\n2.Login\n0.Exit\n";
		 cout<<"Enter choice:";cin>>choice;cin.ignore();
		 if(choice==1)user_signup();
		 else if(choice==2){
			 lindex=login();
			 if(lindex!=-1){
				 pindex=pin_verification(lindex);
				 if(pindex!=-1){ 
	    			 while(true){
			    		 cout<<"\n1.Buy Share\n2.Sell share\n3.View holding\n4.DEposite\n5.Check Market\n6.search stock\n7.All stock\n8.Check Balance\n0.Exit\n";
			    		 cout<<"Enter choice:";cin>>choice;cin.ignore();
					 
			    		 if(choice==1)buy_shares(lindex);
			    		 else if(choice==2){
							 sindex=getstockindex();
							 if(sindex!=-1)sellshare(sindex,lindex);
							 
						 }
						 else if(choice==3)getinvestment(lindex);
						 else if(choice==4)deposite(lindex);
						 else if(choice==5){
							 sindex=getstockindex();
							 if(sindex!=-1){
						    	 start_volume(sindex);
						     }
						 }
						 else if(choice==6){
							 sindex=getstockindex();
							 if(sindex!=-1)getstockdet(sindex);
						 }
						 else if(choice==7)getallstocklist();
						 else if(choice==8)cout<<"Your Balance:"<<user_wallet[lindex]<<endl;
						 else if(choice==0)break;
						 else cout<<"O O andhe user aisa option nahi hain!\n";
					}
				}
			}
		}
	}
 }
 
 
};




int main(){
	USER_STOCK boot;
	boot.start();
	
}

	 
	 
	 
	 
