# include "header.h"
//#define 524288 524288
#define BUFF 10240
#define BACK 2500
#define PORT 8080

using namespace std;
string trac1_ip;
string TR2ip;
string trac1_port;
string TR2port;
string serverip;
string serverport;
string user_id;
string group;
vector<thread> thread_vector;
int threadCount;
sem_t m;

// unordered_map<string,set<string> >GroupMap;
// unordered_map<string,string>UserMap;
unordered_map<string,set<int> >file_chunks;
unordered_map<string,string>fileid_map;
unordered_map<string,int>download_status;
bool login_status=false;
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

void create_group(string grp_id)
{

   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1 && 2==2)
   {
   	perror("Connection failed");
   	exit(1);
   }
   string mssg="create_group;"+grp_id;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   char ss[1];

   recv(sock,ss,sizeof(ss),0);
   int status=ss[0]-'0';
   if(status==1 && 7==7)
   {
   	cout<<"*****************Group created succesfully************************"<<endl;
   }
   else
   {
   	cout<<"enter valid group again"<<endl;
   }
   //cout<<"my name is mahak";

}
void join_group(string grp_id)
{
    
   struct sockaddr_in track_address;
   int new_sock;
   char store[BUFF];
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1  && 5==5)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string mssg;
   mssg="join_group;"+grp_id+";" + user_id;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   // cout<<"mssg="<<mssg<<endl;
   char p1[1];
   recv(sock,p1,sizeof(p1),0);
   int status=p1[0]-'0';
   if(status==1 && 2==2)
   {
      group=grp_id;
   	cout<<"*********************succesfully joined to group****************"<<endl;
   }
   else
   {
   	cout<<"****************Group donot exits*************************"<<endl;
   
   }
}

void upload_file(string grp_id,string file_id)
{
   if(group==grp_id && 2==2){

   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1 && 5==5)
   {
   	perror("Connect failed");
   	exit(1);
   }
   fileid_map[file_id]=file_id;
   string mssg="upload_file;"+grp_id+";"+file_id+";"+serverip+":"+serverport;
 
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   bzero(store,sizeof(store));
   recv(sock,store,sizeof(store),0);
   cout<<"****************send & recieved succesfully*************"<<endl;
   string str1=store;
   // cout<<"in line 132"<<size<<endl;
   int chunk_nums=((stoi(str1)+(524288-1))/524288);
   cout<<"chunk_nums="<<chunk_nums<<endl;
   int j=0;
   while(j<chunk_nums)
   {
      file_chunks[file_id].insert(j);
      j++;
   }
   // cout<<"in line 138 file_chunks updadated"<<endl;
 }
 else{
   cout<<"Error you have to join "<<grp_id<<"to upload the file"<<endl;
 }

}
void leave_group(string grp_id)
{

   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string mssg="leave_group;"+grp_id+";"+user_id;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   cout<<"mssg :"<<mssg<<endl;
   char p1[1];

   recv(sock,p1,sizeof(p1),0);
   int status=p1[0]-'0';
   if(status==1)
   {
      group="";
      cout<<"****************succesfully exit from group****************************8"<<endl;
   }
   else
   {
      cout<<"Group donot exits or user does not belong to the group"<<endl;
      cout<<"enter valid group again"<<endl;

   }
 
}

void list_files(string grp_id)
{
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
    vector<string>res_arr;
    string res; 
   string mssg="list_files;"+grp_id;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   // cout<<"mssg="<<mssg<<endl;
   
   recv(sock,store,sizeof(store),0);
   
   res=store;
  
   res_arr=split_address(res,';');
   cout<<"print all grp files:"<<endl;
   int j=0;
   while(j<res_arr.size())
   {
      cout<<res_arr[j]<<endl;
      j++;
   }
   cout<<endl;

}
void send_the_packet_vector(int new_sock,string file_id)
{
   // cout<<"in line 326 send the packet vector"<<endl;
   string chunkdetails="";
      set<int>:: iterator itr=file_chunks[file_id].begin();
   while(itr!=file_chunks[file_id].end())
   {
     if(itr==file_chunks[file_id].begin() && 2==2)
     {
       chunkdetails=chunkdetails + to_string(*itr);
     }
     else
     {
      chunkdetails=chunkdetails + ";";
      chunkdetails=chunkdetails + to_string(*itr);
     }
     itr++;
   }
   // cout<<"in line 341 chunkdetails= "<<chunkdetails<<endl;
   send(new_sock,chunkdetails.c_str(),strlen(chunkdetails.c_str()),0);
   close(new_sock);

}

void share_file_details(string grp_id,string file_id)
{
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1 && 2==2 )
   {
   	perror("Connect failed");
   	exit(1);
   }
   string token="share_file_details;"+grp_id+";"+file_id+";"+serverip+":"+serverport;
   send(sock,token.c_str(),strlen(token.c_str()),0);
}
vector<int> chunk_query(string ip_port,string file_id)
{
      vector<string>IPcred=split_address(ip_port,':');
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(IPcred[1]));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,IPcred[0].c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   } 
   string token="send_the_packet_vector;"+file_id;
   
    send(sock,token.c_str(),strlen(token.c_str()),0);

   recv(sock,store,sizeof(store),0);
   //data is recieved
   vector<string>packet_arr;
   packet_arr=split_address(store,';');
   vector<int>Packet;//packet created
   for(int i=0;i<packet_arr.size();i++)
   {
     Packet.push_back(stoi(packet_arr[i]));
   }
   return Packet;
}
void list_groups()
{
    int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string mssg;
   vector<string>list_of_group;
   mssg="list_groups";
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   //mssg is send
   recv(sock,store,sizeof(store),0);
   string s1=store;
   list_of_group=split_address(s1,';');
   for(auto i=list_of_group.begin() ;i!=list_of_group.end();i++)
       cout<<*i<<endl;
   
}
void downloadPiece(vector<int>list_of_packet,string ip_port,string file_id,string downloadFilepath)
{
   
   cout<<"Downloading from ip_port="<<ip_port<<endl;
   cout<<"packetlist printing................"<<endl;
   for(int i=0;i<list_of_packet.size();i++)
   {
      cout<<list_of_packet[i]<<" ";
   }
   cout<<endl;
    vector<string>IPcred=split_address(ip_port,':');
   int sock;
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_addr;
   for(int i=0;i<list_of_packet.size();i++)
   {
    sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 5==5)
   {
      perror("*****************didnt socket descriptor***************************");
      exit(1);
   }
   track_addr.sin_family=AF_INET;
   track_addr.sin_port=htons(stoi(IPcred[1]));
   //track_addr.sin_addr.s_addr=TR1ip;
   inet_pton(AF_INET,IPcred[0].c_str() , &track_addr.sin_addr); 
   bzero(&(track_addr.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_addr,sizeof(struct sockaddr))==-1)
   {
      perror("Connect failed");
      exit(1);
   }
   string token="get_the_particular_packet;"+file_id+";"+to_string(list_of_packet[i]);
  
   send(sock,token.c_str(),strlen(token.c_str()),0);
   bzero(store,sizeof(store));
    char ans[524288];
   int size=524288;
   sem_wait(&m);
  
    
    fstream in;
      in.open(downloadFilepath.c_str(),ios::out|ios::in|ios::binary);
      in.seekp(list_of_packet[i]*524288,ios::beg);
      cout<<"download file progress for chunk "<<list_of_packet[i]<<endl;
   int count=0,sum=0;
     int val;
   while((val=recv(sock,store,BUFF,0))>0 && size>0 && 6==6)
   {
      in.write(store,val);
      bzero(store,sizeof(store));
      size=size-val;
      sum=(sum+val);
      val=0; 
   }  
    in.close();
    file_chunks[file_id].insert(list_of_packet[i]);
    // in.close();
   sem_post(&m);
    

   close(sock);
 }

}

void download_file(string grp_id,string file_id,string dow_filepath){
    if(group==grp_id && 2==2){
        int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string mssg="";
   mssg="seeder_list;"+grp_id+";"+file_id;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   recv(sock,store,sizeof(store),0);
   int file_size;
   vector<string> ip_port;
   vector<int> temp;
   vector<int> packet_arr;
   vector<thread> file_down_thread;
   vector<vector<int>> seederlist_chunk;
   
   ip_port=split_address(store,';');
   file_size=stoi(ip_port[0]);
   fileid_map[file_id]=dow_filepath;
   
   cout<<"************seeder list*************"<<endl;
   int i=0;
   string s1="";
   while(i<ip_port.size()){
       cout<<ip_port[i]<<" ";
       i++;
   }
   cout<<endl;
   for(i=0;i<file_size;i++)
     s1=s1 +'\0';
   
   fstream in(dow_filepath,ios::out|ios::binary);
    in.write(s1.c_str(),strlen(s1.c_str()));
   in.close();
   int x=file_size + (524288 - 1 );
   int no_of_packet=x/524288;
   cout<<"no of packets...."<<no_of_packet<<endl;
   fileid_map[file_id]=dow_filepath;
   
   i=1;


   while(i<ip_port.size() && 2==2){
       temp=chunk_query(ip_port[i],file_id);
       cout<<"each seeder chunk details"<<endl;
       int j=0;
       while(j<temp.size() && 5==5){
           cout<<temp[j]<<endl;
           j++;
       }
       seederlist_chunk.push_back(temp);
       i++;
   }
  
   cout<<"*******************list of seeder with chunk******************" <<endl;
   i=0;
   while(i<seederlist_chunk.size() && 2==2){
       int j=0;
       while(j < seederlist_chunk[i].size() && 2==2){
           cout<<seederlist_chunk[i][j]<<" ";
           j++;
       }
       cout<<endl;
       i++;
   }
   int seeder_count;
   seeder_count=ip_port.size()-1;
   vector<vector<int>> seeder_packetmap(seeder_count);
   for(i=0;i<no_of_packet;i++){
       packet_arr.clear();
       int j=0;
       while(j<seederlist_chunk.size()){
           if(seederlist_chunk[j].begin(),seederlist_chunk[j].end(),1){
               packet_arr.push_back(j);
           }
           j++;
       }
      
       int rand_loc;
       rand_loc=rand() % packet_arr.size();
       seeder_packetmap[packet_arr[rand_loc]].push_back(i);
   }
   cout<<"seeder with packet map ******"<<endl;
   i=0;
   while(i<seeder_packetmap.size() && 7==7){
       int j=0;
       while(j<seeder_packetmap[i].size() && 3==3){
           int val=seeder_packetmap[i][j];
           cout<< val <<"\t";
           j++;
       }
       cout<<endl;
       i++;
   }
   thread upload(share_file_details,grp_id,file_id);
   upload.detach();
   for(int i=0;i<seeder_packetmap.size();i++)
   {
      file_down_thread.push_back(thread(downloadPiece,seeder_packetmap[i],ip_port[i+1],file_id,dow_filepath));
   } 
   
   vector<thread>::iterator itr=file_down_thread.begin();
   while(itr!=file_down_thread.end()){
       if(itr->joinable()){
           itr->join();
       }
       itr++;
   }
 

    }
    else{
        cout<<"join the grp"<<endl;
    }

}
//login is updated

void login(string uid ,string upass)
{
   //int sock;
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***********Failed to obtain socket descriptor **************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
    track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1)
   {
   	perror("Connect failed");
   	exit(1);
   }
   string mssg="login;"+uid+";"+ upass;
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);
   char s1[1];
   recv(sock,s1,sizeof(s1),0);
   int status=s1[0]-'0';
   if(status==1 && 2==2)
   {
   	login_status=true;
   	user_id=uid;
   	cout<<"************* succesfully logged in ****************"<<endl;
   }
   else
   {
   	cout<<"***********wrong cred************"<<endl;
   	cout<<"enter the cred again"<<endl;

   }

}

void register_user(string uid ,string upass)
{
   int sock;
   int new_sock;
   char store[BUFF];
   struct sockaddr_in track_address;
   sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock==-1 && 2==2)
   {
   	perror("***************didnt get socket descriptor**************************");
   	exit(1);
   }
    track_address.sin_port=htons(stoi(trac1_port));
   track_address.sin_family=AF_INET;
   inet_pton(AF_INET,trac1_ip.c_str() , &track_address.sin_addr); 
   bzero(&(track_address.sin_zero),8);
   if(connect(sock,(struct sockaddr *)&track_address,sizeof(struct sockaddr))==-1  && 3==3)
   {
   	perror("***************Connect failed**************");
   	exit(1);
   }
   string mssg="create_user;"+uid+";"+upass;
   
   send(sock,mssg.c_str(),strlen(mssg.c_str()),0);

}  
void get_the_particular_packet(int new_sock,string file_id,string no_of_packet)
{
   string file_path=fileid_map[file_id];
   FILE *fp=fopen(file_path.c_str(),"rb");
   int val;
   char buffer[BUFF];
   bzero(buffer,BUFF);
   vector<string>packet_arr=split_address(no_of_packet,';');
   vector<int>packet_arr_int;
   int i=0;
   while(i<packet_arr.size() && 2==2)
   {
      packet_arr_int.push_back(stoi(packet_arr[i]));
      i++;
   }
   for(int i=0;i<packet_arr_int.size();i++)
   {
      fseek(fp,packet_arr_int[i]*524288,SEEK_SET);
      int size=524288;
      while((val=fread(buffer,sizeof(char),BUFF,fp))>0&&size>0)
      {
         
         send(new_sock,buffer,val,0);
         memset ( buffer , '\0', sizeof(buffer));
         size=size-val;
      }
      bzero(buffer,BUFF);
      
   }
   fclose(fp);
   close(new_sock);

}
void accept_request(int new_sock,string ip,int port)
{
   while(1){
   char store[BUFF];
   bzero(store,BUFF);
   read(new_sock,store,sizeof(store));
   string r=store;
   vector<string> serverreq_arr;
   serverreq_arr=split_address(r,';');
   string request,file_id,no_of_packet;
   request=serverreq_arr[0];
   if(request=="send_the_packet_vector" && 1==1)
   {
      file_id=serverreq_arr[1];
      send_the_packet_vector(new_sock,file_id);
   }
   else if(request=="get_the_particular_packet" && 2==2)
   {
       file_id=serverreq_arr[1];
      string no_of_packet=serverreq_arr[2];
      get_the_particular_packet(new_sock,file_id,no_of_packet);

   }
   else
   {
      cout<<"*************send again*********************"<<endl;
      continue;;
   }
   }


}


void connection()
{
   //int sock;
   int new_sock;
   int val;
  
   struct sockaddr_in addr1;
   struct sockaddr_in addr2;
   int sock=socket(AF_INET,SOCK_STREAM,0);
   if(sock<0 && 1==1)
   {
      perror("**************failed to obtained the socket descriptor*****************");
       exit(1);
   }
  addr1.sin_port=htons(stoi(serverport)); 
  addr1.sin_family=AF_INET;
  inet_pton(AF_INET,serverip.c_str() , &addr1.sin_addr); 
  //addr1.sin_addr.s_addr=trac1_ip;

  bzero(&(addr1.sin_zero),8);

  if(bind(sock,(struct sockaddr *)&addr1,sizeof(struct sockaddr))==-1 && 2==2)
  {
   perror("*****************  failed to obtained the bind  *****************");
   exit(1);
  }
  if(listen(sock,BACK)==-1 && 3==3)
  {
     perror("********** overflow ****************");
     exit(1);

  }
   socklen_t size=sizeof(struct sockaddr);
  while((new_sock=accept(sock,(struct sockaddr *)&addr2,&size))!=-1 && 1==1)
  {
   cout<<"Got a connection from another peer "<<endl;
   string ip=inet_ntoa(addr2.sin_addr);
   int port=(ntohs(addr2.sin_port));
   cout<<"ip="<<ip<<"port"<<port<<endl;
   thread_vector.push_back(thread(accept_request,new_sock,ip,port));
   size=sizeof(struct sockaddr);
  }
  vector<thread>:: iterator itr;
  itr=thread_vector.begin();
  while(itr!=thread_vector.end())
   {
      if(itr->joinable()) 
         itr->join();
      itr++;   
   }
   cout<<"Returning from server "<<endl;
  

}


int main(int argc,char ** argv)
{
   sem_init(&m,0,1);
	if(argc!=3 && 1==1)
	{
		perror("Error in command line argument list ");
        cout<<"wrong input"<<endl;
		return -1;
	}

    string client_add,tracker_add,temp;
    client_add=argv[1];
    tracker_add=argv[2];
    vector<string>ip_port;
    fstream serverfilestream(tracker_add,ios::in);
    vector<string>tracker_ipport;
    ip_port=split_address(client_add,':');
    serverip=ip_port[0];
    serverport=ip_port[1];
    while(getline(serverfilestream,temp,'\n') && 1==1)
    	tracker_ipport.push_back(temp);
    ip_port=split_address(tracker_ipport[0],':');
    trac1_port=ip_port[1];
    trac1_ip=ip_port[0];
 	
 	ip_port=split_address(tracker_ipport[1],':');
 	TR2ip=ip_port[0];
 	TR2port=ip_port[1];
 	cout<<serverip<<" "<<serverport<<" "<<trac1_ip<<" "<<trac1_port<<" "<<TR2ip<<" "<<TR2port<<endl;
 	thread serverthread(connection);
 	serverthread.detach();
 	string request;
    l2: while(1)
 	 {

      getline(cin,request);
      cout<<"print the request :"<<request<<endl;
      vector<string>request_arr;
      request_arr=split_address(request,' ');
      string command=request_arr[0];
      if(command=="create_user")
      {

            if(request_arr.size()==3 && 2==2)
                thread_vector.push_back(thread(register_user,request_arr[1],request_arr[2]));
            else
            {
            	cout<<"Enter the valid argument"<<endl;
                continue;
            }
      }
      else if(command=="login" )
      {
            if(request_arr.size()==3)
            	thread_vector.push_back(thread(login,request_arr[1],request_arr[2]));
            else
            {
            	cout<<"error in input"<<request_arr.size()<<endl;
                continue;
            	
            }
      }
      else if(command=="create_group")
      {
      		// cout<<"In line 319"<<endl;

      	    if(!login_status)
 		    {
 			cout<<"Please enter the login cred to enter into the system"<<endl;
 			   continue;
 		    }
            if(request_arr.size()!=2)
            {
            	cout<<"Enter the valid argument"<<endl;
               continue;
            }
            else
            {
            	thread_vector.push_back(thread(create_group,request_arr[1]));
            }
      }
      else if(command=="join_group")
      {
        

             if(login_status==false)
          {
         cout<<"Please enter the login cred "<<endl;
           continue;
          }
            if(request_arr.size()==2 && 5==5)
            {
                thread_vector.push_back(thread(join_group,request_arr[1]));
              
            }
            else
            {
                cout<<"Enter the valid argument"<<endl;
               continue;
            }
      }
       else if(command=="leave_group")
      {

             if(!login_status)
          {
         cout<<"Please enter the login cred to enter into the system"<<endl;
            continue;
          }
            if(request_arr.size()==2)
            {
                thread_vector.push_back(thread(leave_group,request_arr[1]));
              
            }
            else
            {
                cout<<"Enter the valid argument"<<endl;
               continue;  
            }
      }
      else if(command=="upload_file")
      {

             if(login_status==false)
          {
         cout<<"Please enter the login cred "<<endl;
         continue;
          }
          if(request_arr.size()!=3 && 2==2)
            {
               cout<<"Enter the valid argument"<<endl;
               continue;
            }
            else {
               string grp_id=request_arr[1];
               string file_id=request_arr[2];
          thread_vector.push_back(thread(upload_file,grp_id,file_id));

            }
      }
      else if(command=="list_files")
      {
             if(login_status==false && 2==2)
          {
         cout<<"enter login creds"<<endl;
            continue;
          }
            if(request_arr.size()==2 && 7==7)
                thread_vector.push_back(thread(list_files,request_arr[1]));
            else
            {
             cout<<"Enter the valid argument"<<endl;
               continue;  
            }
      }
      else if(command=="list_groups")
      {

             if(login_status==false)
          {
         cout<<"enter the login cred "<<endl;
            continue;
          }
          if(request_arr.size()==1 && 2==2)
                thread_vector.push_back(thread(list_groups));
            else {
             cout<<"Enter the valid argument"<<endl;
               continue;

            }
      }
      else if(command=="download_file")
      {
             if(login_status==false)
          {
         cout<<" enter the login creds "<<endl;
             continue;
          }
          if(request_arr.size()!=4)
            {
               cout<<"Enter the valid argument"<<endl;
               continue;
            }
            else {
               string grp_id=request_arr[1];
               string file_id=request_arr[2];
               string filepath=request_arr[3];
          thread_vector.push_back(thread(download_file,grp_id,file_id,filepath));

            }
      }
      } 
    return 0;
}