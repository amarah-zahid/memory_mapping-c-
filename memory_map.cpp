#include <dirent.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<cstring>
#include<string>
#include<cerrno>
#include<cstdio>
#include<cstdlib>
using namespace std;
class memory{
public:
//if the file is not created this function will create the file and
//writecontents in it.
//if the file exists the new content will overwrite the old content.

int write_file(){
string filename;
cout<<"enter filename you want to write in: "<<endl;
cin>>filename;
cin.ignore();
int fd=open(filename.c_str(),O_RDWR | O_CREAT, 0666);
if(fd==-1){
cerr<<"error opening file: "<<strerror(errno)<<endl;
return 1;
}
struct stat fileinfo;
if(fstat(fd,&fileinfo)==-1){
cerr<<"error getting file information"<<strerror(errno)<<endl;
close(fd);
return 1;
}
size_t filesize= fileinfo.st_size;
cout<<"size of the file: "<<filesize<<endl;
if(filesize==0){
const size_t initialsize=128;
if(ftruncate(fd,initialsize)==-1)
{
cerr<<"error setting filesize: "<<strerror(errno)<<endl;
close(fd);
return 1;
}

filesize=initialsize;
cout<<"file created and initialized with size: "<<filesize<<" bytes"<<endl;
}
void* map_addr=mmap(NULL, filesize,PROT_READ| PROT_WRITE, MAP_SHARED, fd,0);
if(map_addr== MAP_FAILED){
cerr<<"error mapping file: "<<strerror(errno)<<endl;
close(fd);
return 1;
}

char* map_data=static_cast<char*>(map_addr);
//const char* mess="hello memory mapping!";
string newcontent;
cout<<"enter the data you want to write in the file: "<<endl;
getline(cin, newcontent);
size_t mess_len=newcontent.size();
if(mess_len>filesize){
cout<<"message too large for mapped region, resizing file for new content "<<endl;
fd=open(filename.c_str(), O_RDWR);
if(ftruncate(fd,mess_len)==-1)
{
cerr<<"error resizing file for new content"<<endl;
close(fd);
munmap(map_addr,mess_len);
return 1;
}
close(fd);
munmap(map_addr,mess_len);
filesize=mess_len;
map_addr=mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, open(filename.c_str(),O_RDWR), 0);
if(map_addr==MAP_FAILED){
cerr<<"error mapping file"<<endl;
return 1;
}
map_data=static_cast<char*>(map_addr);
}
memcpy(map_data,newcontent.c_str(),mess_len);
cout<<"size of the memory allocated for the file: "<< mess_len<<endl;
cout<<"data in mapped memory: "<<map_data<<endl;
if(munmap(map_addr, filesize)==-1){
cerr<<"error unmapping file: "<<strerror(errno)<<endl;
}
close(fd);

cout<<"memory mapping and writing completed"<<endl;
return 0;
}



int read_file()
{
string filename;
cout<<"enter filename you want to open for reading" <<endl;
cin>>filename;
int fd=open(filename.c_str(), O_RDONLY);
if(fd==-1)
{
cerr<<"error opening file: "<<strerror(errno)<<endl;
return 1;
}
struct stat file_stat;
if(fstat(fd, &file_stat)==-1)
{
cerr<<"error getting  file size : "<<strerror(errno)<<endl;
close(fd);
return 1;
}
size_t file_size=file_stat.st_size;
if(file_size==0){
cout<<"file is empty"<<endl;
close(fd);
return 0;
}
void* map_addr=mmap(NULL, file_size,PROT_READ,MAP_SHARED,fd,0);
if(map_addr==MAP_FAILED){
cerr<<"error mapping  file: "<<strerror(errno)<<endl;
close(fd);
return 1;
}
char* map_data= static_cast<char*>(map_addr);
string file_content(map_data,file_size);
cout<<"data written in file: \n" <<file_content<<endl;
if(munmap(map_addr, file_size)==-1){
cerr<<"error unmapping file: "<<strerror(errno)<<endl;
}
close(fd);

cout<<"memory mapping and reading completed"<<endl;
return 0;
}
int update_file()
{
string filename;
cout<<"enter filename you want to write in: "<<endl;
cin>>filename;
cin.ignore();
int fd=open(filename.c_str(),O_RDWR);
if(fd==-1){
cerr<<"error opening file: "<<strerror(errno)<<endl;
return 1;
}
struct stat filestat;
if(fstat(fd,&filestat)==-1){
cout<<"error getting filesize"<<endl;
close(fd);
return 1;
}
size_t currentsize=filestat.st_size;
string newcontent;
cout<<"enter the data you want to update in the file: "<<endl;
getline(cin, newcontent);
string contentwrite=(currentsize > 0 ? "\n" : "" )+ newcontent;
size_t newsize=currentsize + contentwrite.size();
if(ftruncate(fd,newsize)==-1){
cout<<"error resizing file"<<endl;
close(fd);
return 1;
}
void* map_addr=mmap(NULL, newsize, PROT_READ| PROT_WRITE, MAP_SHARED, fd,0);
if(map_addr== MAP_FAILED){
cerr<<"error mapping file: "<<strerror(errno)<<endl;
close(fd);
return 1;
}

char* data=static_cast<char*>(map_addr);
memcpy(data+ currentsize, contentwrite.c_str(), contentwrite.size());
cout<<"data updated in file: "<<newcontent<<endl;
if(msync(map_addr,newsize,MS_SYNC)==-1)
{
cerr<<"error syncing memory"<<endl;
munmap(map_addr,newsize);
return 1;
}
if(munmap(map_addr, newsize)==-1){
cerr<<"error unmapping file: "<<strerror(errno)<<endl;
return 1;
}

close(fd);
 cout<<"updation completed in file"<<endl;
return 0;
}
int delete_file(){
string filename;
cout<<"enter the filename you want to delete: "<<endl;
cin>>filename;
if(remove(filename.c_str())!=0)
{
cout<<"error deleting file"<<endl;
}
else{
cout<<"file deleted successfully"<<endl;
}
}
int rename_file() {
    string old_filename, new_filename;

    cout << "Enter the current filename to rename: ";
    cin >> old_filename;

    cout << "Enter the new filename: ";
    cin >> new_filename;

   
    if (rename(old_filename.c_str(), new_filename.c_str()) != 0) {
        cerr << "Error renaming file: " << strerror(errno) << endl;
        return 1; 
    }

    cout << "File renamed successfully from '" << old_filename << "' to '" << new_filename << "'" << endl;
    return 0; 
}
int list_file_dir() {
    string dirpath;
    cout << "Enter the directory path to list files: " << endl;
    cin >> dirpath;

    DIR* dir;
    struct dirent* entry;

    
    dir = opendir(dirpath.c_str());
    if (dir == NULL) {
        cerr << "Error opening directory: " << strerror(errno) << endl;
        return 1; 
    }

    cout << "Files in directory '" << dirpath << "':" << endl;
   
    while ((entry = readdir(dir)) != NULL) {
        
        if (entry->d_name[0] != '.') {
            cout << entry->d_name << endl;
        }
    }

    closedir(dir); 
    return 0; 
}
int search_str(){
	 char command[512]; 

 
    string pattern, directory;
    cout << "Enter the string pattern to search for:\n";
    cin>>pattern;
   
 cout << "Enter the directory path to search in:\n";
    getline(cin, directory);
cin.ignore();
    snprintf(command, sizeof(command), "grep -r \"%s\" \"%s\"", pattern.c_str(), directory.c_str());

   
    int result = system(command);
    
    
    if (result == -1) {
        cerr << "Error executing command." << endl;
    }

    return 0;
}
};

int main()
{
int n;
memory m;
cout<<"press 1 for writing operation"<<endl;
cout<<"press 2 for reading operation"<<endl;
cout<<"press 3 for updating file"<<endl;
cout<<"press 4 for deleting a file"<<endl;
cout<<"press 5 for renaming a file"<<endl;
cout<<"press 6 for listing files of a specific location"<<endl;
cout<<"press 7 for searching a string among files of a location"<<endl;
cin>>n;
if(n==1)
{
m.write_file();
}

if(n==2)
{
m.read_file();
}
if(n==3)
{
m.update_file();
}
if(n==4)
{
m.delete_file();
}
if(n==5){
	m.rename_file();
}
if(n==6){
	m.list_file_dir();
}
if(n==7)
{
	m.search_str();
}
return 0;
}


