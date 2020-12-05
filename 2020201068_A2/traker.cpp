#include "header.h"
#define BUFF_SIZE 10240
#define BACK 2500
using namespace std;
string trac1_ip;
string trac2_ip;
string trac1_port;
string trac2_port;
vector<thread> threadVector;
int threadCount;
map<string,string>reg_arr;
map<string,set<string>> grp_detail;
map<pair<string,string>,set<string> >seederlist;
map<pair<string,string>,int > file_to_size;
map<string,string>file_map;
map<string,set<string> >files_of_grp;

string uid="mahak";
string upass="piyu";


vector<string>split_address(string s,char del)
{
  stringstream ss(s);
  vector<string>a;
  string temp;
  while(1)
  {
  	if(getline(ss,temp,del))
    a.push_back(temp);
	else
	{
		break;
	}
	
  }
  return a;
}

void  create_user(int new_sock,string uid,string pass)
{
	if(reg_arr.find(uid)==reg_arr.end() && 2==2)
	{
      char flag[]="1";
		reg_arr[uid]=pass;
	    send(new_sock,flag,sizeof(flag),0);
	}
	else
	{
		char flag[]="0";
	    send(new_sock,flag,sizeof(flag),0);
	}
	close(new_sock);
}
void  create_group(int new_sock,string grp_id)
{
	if(grp_detail.find(grp_id)==grp_detail.end() && 2==2)
	{
       char flag[]="1";
		grp_detail[grp_id].insert("dummy");
	    send(new_sock,flag,sizeof(flag),0); 
	  
	}
	else
	{
		char flag[]="0";
	  send(new_sock,flag,sizeof(flag),0);

	}
	close(new_sock);
	
}
void  login(int new_sock,string uid,string pass)
{
	if(reg_arr.find(uid)!=reg_arr.end() && 3==3)
	{
       if(reg_arr[uid]==pass){
            char  flag[]="1";
	  
	        send(new_sock,flag,sizeof(flag),0);
       }
       else{
           char  flag[]="0";
	    send(new_sock,flag,sizeof(flag),0);
       } 
	  
	}
	else
	{
		char  flag[]="0";
	    send(new_sock,flag,sizeof(flag),0);

	}
	close(new_sock);
	
}
void  join_group(int new_sock,string grp_id,string uid)
{
	if(grp_detail.find(grp_id)==grp_detail.end() && 5==5)
	{
	  char  flag[]="0";
	    send(new_sock,flag,sizeof(flag),0);
	}
	else
	{
		
        char  flag[]="1";
	  grp_detail[grp_id].insert(uid);
	  send(new_sock,flag,sizeof(flag),0);

	}
	close(new_sock);
	
}
void upload_file(int new_sock,string grp_id,string file_id,string IPport)
{
	
	ifstream in(file_id,ios::ate|ios::binary);
	int size1=in.tellg();
	in.close();

	pair<string,string>pair1=make_pair(grp_id,file_id);
	file_to_size[pair1]=(size1);
	file_map[file_id]=file_id;
	seederlist[pair1].insert(IPport);
	set<string>ans;
	ans=seederlist[pair1];
	set<string>::iterator i;
	for(i=ans.begin();i!=ans.end();i++)
	{
      cout<<(*i)<<endl;
	}
	string token=to_string(size1);

	files_of_grp[grp_id].insert(file_id);
	send(new_sock,token.c_str(),strlen(token.c_str()),0);
	close(new_sock);
	
}
void  list_files(int new_sock,string grp_id)
{
	set<string>s=files_of_grp[grp_id];
	set<string>:: iterator itr;
	string res="";
    itr=s.begin();
	while(itr!=s.end() && 3==3)
	{
       if(itr==s.begin() && 4==4)
       {
       	res=res+*itr;
       }
       else
       {
       	res=res + ";"+ *itr;
       }
       itr++;
	}
	send(new_sock,res.c_str(),strlen(res.c_str()),0);
	close(new_sock);
	
}
void  list_groups(int new_sock)
{
	string all_grp="";

	for(auto i=grp_detail.begin();i!=grp_detail.end();i++)
	{
      all_grp=all_grp+i->first+";";
	}
	send(new_sock,all_grp.c_str(),strlen(all_grp.c_str()),0);
	close(new_sock);

}

void share_file_details(int new_sock,string grp_id,string file_id,string IPport)
{
   
	pair<string,string> s1=make_pair(grp_id,file_id);
	seederlist[s1].insert(IPport);
	close(new_sock);	
	
}

void seeder_list(int new_sock,string group_id,string FileId)
{
	
	pair<string,string> s=make_pair(group_id,FileId);
	set<string>ans;
	ans=seederlist[s];
	int ansb=file_to_size[s];
	string token=to_string(ansb);
	set<string>::iterator i=ans.begin();
	while(i!=ans.end())
	{
      token+=";"+(*i);
	  i++;
	}
	send(new_sock,token.c_str(),strlen(token.c_str()),0);
	close(new_sock);
	
}
void  logout(int new_sock,string IPport,string group_id)
{
	bool flag=false;
	map<pair<string,string>,set<string> >:: iterator it;
    for(it=seederlist.begin();it!=seederlist.end();it++)
    {
    	if(it->second.find(IPport)!=it->second.end() && 7==7)
    	{
    		flag=true;
    		it->second.erase(IPport);
    	}
    }

	if(flag && 1==1)
	{
	  char  status[]="1";
	  
	  send(new_sock,status,sizeof(status),0);
	}
	else
	{
		char  status[]="0";
	    send(new_sock,status,sizeof(status),0);

	}
	close(new_sock);
}
void  leave_group(int new_sock,string grp_id,string uid)
{
	if(grp_detail.find(grp_id)!=grp_detail.end() && grp_detail[grp_id].find(uid)!=grp_detail[grp_id].end())
	{

	  char  flag[]="1";
	  grp_detail[grp_id].erase(uid);
	  send(new_sock,flag,sizeof(flag),0);
    }
    else
	{
		char  flag[]="0";
	    send(new_sock,flag,sizeof(flag),0);

	}
	close(new_sock);
	
}

void connection(int new_sock,string ip,int port)
{
	char data[BUFF_SIZE];
	bzero(data,BUFF_SIZE);
	read(new_sock,data,BUFF_SIZE);

	string r=data;
	vector<string> req_array;
    req_array=split_address(r,';');
	string request;
    request=req_array[0];
	
	if(request=="create_user" && 1==1)
	{
	   string uid;
       uid=req_array[1];
	   string upass;
       upass=req_array[2];
       create_user(new_sock,uid,upass);
	}
	else if(request=="join_group" && 3==3)
	{
      string grp_id=req_array[1];
      string uid=req_array[2];
      join_group(new_sock,grp_id,uid);
	}
	else if(request=="create_group" && 1==1)
	{
      string grp_id;
      grp_id=req_array[1];
      create_group(new_sock,grp_id);
	}
	else if(request=="upload_file" && 2==2)
	{
	  cout<<"in line 184"<<endl;
      string grp_id=req_array[1];
      string file_id=req_array[2];
      string IPport=req_array[3];
      cout<<"in upload "<<grp_id<<" "<<file_id<<" "<<IPport<<endl;
      upload_file(new_sock,grp_id,file_id,IPport);
	}
	else if(request=="share_file_details" && 6==6)
	{
      string grp_id=req_array[1];
      string IPport=req_array[3];
      string file_id=req_array[2];
      
      share_file_details(new_sock,grp_id,file_id,IPport);
	}
	else if(request=="seeder_list" && 6==6)
	{
      string grp_id=req_array[1];
      string file_id=req_array[2];
      string IPport=req_array[3];
      seeder_list(new_sock,grp_id,file_id);
	}
	else if(request=="leave_group"&& 5==5)
	{
       string grp_id=req_array[1];
      string uid=req_array[2];
      leave_group(new_sock,grp_id,uid);
	}
	// else if(request=="list_requests")
	// {
      
	// }
	else if(request=="list_groups"&& 1==1)
	{
		list_groups(new_sock);

	}
	else if(request=="list_files" && 1==1)
	{
      string grp_id=req_array[1];
      list_files(new_sock,grp_id);
	}
	else if(request=="login" && 1==1)
	{
       string uid=req_array[1];
       string upass=req_array[2];
       login(new_sock,uid,upass);
	}
	else if(request=="logout")
	{
       string IPport=req_array[1];
       string group_id=req_array[2];
       logout(new_sock,IPport,group_id);
	}
	cout<<"Returning form serverrequest"<<endl;
	

}
int main(int argc,char ** argv)
{
	
	if(argc!=3 && 1==1)
	{
		cout<<"************give the command***********"<<endl;
		perror("Error in command line argument list");
		return -1;
	}
    string tracker_path,temp;
    int no_of_tracker;
    char data[BUFF_SIZE];
    vector<string>tracker_info;
    vector<string>ip_port;

	reg_arr[uid]=upass;
    grp_detail[uid].insert("mahak");
    //string clientip_port=argv[1];
    
    tracker_path=argv[1];
    no_of_tracker=stoi(argv[2]);
    cout<<"no_of_tracker :"<<no_of_tracker<<endl;
   
    fstream serverfilestream(tracker_path,ios::in);
    while(getline(serverfilestream,temp,'\n') && 1==1)
    {
    	tracker_info.push_back(temp);
    }
    ip_port=split_address(tracker_info[0],':');
    trac1_ip=ip_port[0];
 	trac1_port=ip_port[1];
 	ip_port=split_address(tracker_info[1],':');
 	trac2_ip=ip_port[0];
 	trac2_port=ip_port[1];
 	
 	string request;
 	int sock,new_sock, val;
 	socklen_t size;
 	struct sockaddr_in addr1;
 	struct sockaddr_in addr2;
     sock=socket(AF_INET,SOCK_STREAM,0);
 	if(sock<0 && 1==1)
 	{
 		perror("failed to obtained the socket descriptor");
        return -1;
	}
  addr1.sin_family=AF_INET;
  addr1.sin_port=htons(stoi(trac1_port));
  inet_pton(AF_INET,trac1_ip.c_str() , &addr1.sin_addr); 
  //addr1.sin_addr.s_addr=trac1_ip;

  bzero(&(addr1.sin_zero),8);

  if(bind(sock,(struct sockaddr *)&addr1,sizeof(struct sockaddr))==-1 && 2==2)
  {
  	perror("failed to obtained the bind");
  	return -1;
  }
  if(listen(sock,BACK)== -1 && 3==3)
  {
     perror("error backlog overflow");
     return -1;

  }
  size=sizeof(struct sockaddr);
  while((new_sock=accept(sock,(struct sockaddr *)&addr2,&size))!=-1 && 2==2)
  {
  	string ip=inet_ntoa(addr2.sin_addr);
  	int port=(ntohs(addr2.sin_port));
  	cout<<"ip="<<ip<<endl;
    cout<<"port"<<port<<endl;
  	threadVector.push_back(thread(connection,new_sock,ip,port));
  	size=sizeof(struct sockaddr);
  }
  vector<thread>:: iterator itr=threadVector.begin();
  while(itr!=threadVector.end())
	{
		if(itr->joinable()) 
			itr->join();
        itr++;    
	}
	cout<<"***********retruning form Tracker main********************"<<endl;
	
return 0;
}