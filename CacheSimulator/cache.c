 #include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct line{//each set will contain line data structure. The line structure stores the tag for the addresses being stored in the cache,a valid tag used for determining if the block is empty, and a "used" field used to determine when the address was last accessed so the LRU replacement works as intended
    unsigned long int tag;
    int valid;
    unsigned long int used;
}line;
struct line** cache;

int miss;
int hit;
int reads;
int writes;
unsigned long int count;


line** createcache(int setnum,int assoc)//fills in the cache as an array as an array of line pointers
{
  int i,j;
  cache=(line**)malloc(setnum*sizeof(line*));//allocates the memory to create the sets
  for(i=0;i<setnum;i++)
  {
    cache[i]=(line*)malloc((assoc)*sizeof(line));//allocates memory for each of the sets based on the associativity number. the amount of memory allocated is the associativity multiplied the memory requirement for each line.The malloc is a line* since the array holds line pointers
  }

  for(i=0;i<setnum;i++)
  {
    for(j=0;j<assoc;j++)
    {
         cache[i][j].valid=0;

    }
  }
return cache;
}

void readlru(unsigned long int tagindex,unsigned long int setindex,int assoc){//reads the designated address

int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0)//valid tag is 0 so it misses
	{
    	miss++;
	    reads++;
    	count++;
    	cache[setindex][i].valid=1;
    	cache[setindex][i].tag=tagindex;
    	cache[setindex][i].used=count;
    	return;
	}
	else
	{

		if(cache[setindex][i].tag==tagindex)//tag matches so its a hit
		{
			hit++;
			count++;
			cache[setindex][i].used=count;
			return;
		}
		if(i==(assoc-1))//tag doesnt match so when it hits the end of the set it updates as a miss and will remove the least recently accessed address
		{
			miss++;
			reads++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			    if(cache[setindex][j].used<=cache[setindex][min].used)
			    {
			    	min=j;
			    }	
			}
			cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        return;
		}
	}
}
return;
}


void writelru(unsigned long int tagindex,unsigned long int setindex,int assoc){

int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0)//the valid tag is 0 so it is a miss  and then it changes the valid tag to 1 and changes the tag of the location to the designated tagindex
	{
	miss++;
	writes++;
	reads++;
	count++;
	cache[setindex][i].valid=1;
	cache[setindex][i].tag=tagindex;
	cache[setindex][i].used=count;
	return;
	}
	else
	{
	    if(cache[setindex][i].tag==tagindex){//the valid tag is 1 and the tag matches the tagindex so it is a hit and updates the used element so it keeps track of the most recent accessed memory
			hit++;
			writes++;
			count++;
			cache[setindex][i].used=count;
			return;
		}
			
		if(i==(assoc-1))
		{
			miss++;
			reads++;
			writes++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			
			    if(cache[setindex][j].used<=cache[setindex][min].used)
			    {
			    	min=j;
			    }	
			}
			cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        return;
		}
	}
}
return;
}





void prefetchwrite(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0)
	{
    	reads++;
    	count++;
    	cache[setindex][i].valid=1;
    	cache[setindex][i].tag=tagindex;
    	cache[setindex][i].used=count;
	
	return;
	}
	else
	{
		if(cache[setindex][i].tag==tagindex)
		{
			
			return;
		}
			
		if(i==(assoc-1))
		{
			
			reads++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			    if(cache[setindex][j].used<=cache[setindex][min].used)
		    	{
			    	min=j;
		    	}	
			}
			cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        return;
		}
	}
}
return;
}



void prefetchwritelru(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int lrutagind,unsigned long int lrusetind){
	
int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0)
	{
	    miss++;
    	reads++;
    	writes++;
    	count++;
    	cache[setindex][i].valid=1;
    	cache[setindex][i].tag=tagindex;
    	cache[setindex][i].used=count;
    	prefetchwrite(lrutagind,lrusetind,assoc);
	return;
	}
	else
	{
		if(cache[setindex][i].tag==tagindex){
			hit++;
			writes++;
			count++;
			cache[setindex][i].used=count;
			return;
		}
			
		if(i==(assoc-1))
		{
			miss++;
			reads++;
			writes++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			
		    	if(cache[setindex][j].used<=cache[setindex][min].used)
		    	{
			    	min=j;
		    	}	
	    	}
	    	cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        prefetchwrite(lrutagind, lrusetind,assoc);
	        return;
    	}
	}
}

return;
}






void prefetchread(unsigned long int tagindex,unsigned long int setindex,int assoc){
	
	int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0)
	{
	    reads++;
    	count++;
    	cache[setindex][i].valid=1;
    	cache[setindex][i].tag=tagindex;
    	cache[setindex][i].used=count;
	return;
	}
	else
	{
		if(cache[setindex][i].tag==tagindex)
		{
			
			return;
		}
			
		if(i==(assoc-1))
		{
			reads++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			
		    	if(cache[setindex][j].used<=cache[setindex][min].used)
		    	{
			    	min=j;
			    }	
			}
			cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        return;
		}
	}
		
		
}
}


void prefetchreadlru(unsigned long int tagindex,unsigned long int setindex,int assoc,unsigned long int lrutagind,unsigned long int lrusetind){
	
	
int i,j,min;

for(i=0;i<assoc;i++)
{
	if(cache[setindex][i].valid==0){
	miss++;
	reads++;
	count++;
	cache[setindex][i].valid=1;
	cache[setindex][i].tag=tagindex;
	cache[setindex][i].used=count;
	
	prefetchread(lrutagind,lrusetind,assoc);
	return;
	}
	else
	{
		if(cache[setindex][i].tag==tagindex){
			hit++;
			count++;
			cache[setindex][i].used=count;
			return;
		}
			
		if(i==(assoc-1))
		{
			miss++;
			reads++;
			min=0;
			for(j=0;j<assoc;j++)
			{
			    if(cache[setindex][j].used<=cache[setindex][min].used)
			    {
			    	min=j;
			    }	
			}
			cache[setindex][min].valid=1;
	        cache[setindex][min].tag=tagindex;
	        count++;
	        cache[setindex][min].used=count;
	        prefetchread(lrutagind,lrusetind,assoc);
	        return;
		}
	}
}
return;
}





void empty(int setnum, int assoc)//resets everything to 0 and empties the cache so the prefetched verson can be runned
{
	int i,j;
	for(i=0;i<setnum;i++)
	{
		for(j=0;j<assoc;j++)
		{
			cache[i][j].tag=0;
			cache[i][j].valid=0;
			cache[i][j].used=0;
	    }
	}
	miss=0;
	hit=0;
	reads=0;
	writes=0;
	count=0;
}

int main(int argc, char** argv)
{
   int cachesize=atoi(argv[1]);
   int blocksize=atoi(argv[4]);
   int assoc;
   int setnum;
   int x;//used for holding associativity when read from the text file
   int b;//used for bit operations, acts like the block offset
   int s;//used for bit operations to get the correct set index
   //combination of b and s with bit operations is used to get the tag index
     char writeorread;
  unsigned long int address;
  unsigned long int lruaddress;
  unsigned long int setmask;//used for calculating the set index through bitwise operations
  unsigned long int tagindex;
  unsigned long int setindex;
  unsigned long int lrutagind;//this is the tag index of the new 
  unsigned long int lrusetind;
   
   FILE* fl;
  fl=fopen(argv[5],"r");

  if(fl==NULL){

    printf("cannot find tracefile with that name\n");
    return 0;

  }


if(argv[2][0]=='d')
{
    assoc=1;
    setnum=cachesize/blocksize;

}else if(argv[2][5]!=':')//full associativity since there is no : to designate a associativity value
{
    setnum=1;
    assoc=cachesize/blocksize;
}else
{//sets the associativity to the designated level and calculates the number of sets
    sscanf(argv[2],"assoc:%d",&x);
    assoc=x;
    setnum=cachesize/blocksize/x;

  }

b=log(blocksize)/log(2);//calcates bits for the index and mask
s=log(setnum)/log(2);

setmask=((1<<s)-1);

cache=createcache(setnum,assoc);//creates the cache with the designated associativity and the number of sets


while(fscanf(fl, "%*x: %c %lx", &writeorread, &address)==2)//runs the unprefetched cache
{
    setindex=(address>>b)&setmask;
    tagindex=address>>(b+s);
    if(writeorread=='R')//reads or writes based on the letter contained in the tracefile line
    {
    	readlru(tagindex,setindex,assoc);
	}
	else if(writeorread=='W')
	{
		writelru(tagindex,setindex,assoc);
	}
}

fclose(fl);


  fl=fopen(argv[5],"r");

  if(fl==NULL){

    printf("cannot find tracefile with that name\n");
    return 0;

  }

 printf("no-prefetch\n");
 printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",reads,writes,hit,miss);

empty(setnum,assoc);//clears the cache and the variables associated with it so that the prefetched version can be run

while(fscanf(fl, "%*x: %c %lx", &writeorread, &address)==2){//runs loop to read and write from the file in the prefetched setting

setindex=(address>>b)&setmask;
tagindex=address>>(b+s);
lruaddress=address+blocksize;
lrusetind=(lruaddress>>b)&setmask;
lrutagind=lruaddress>>(b+s);
    if(writeorread=='R')
    {
	    prefetchreadlru(tagindex,setindex,assoc,lrutagind,lrusetind);
	}else if(writeorread=='W')
    {
		   prefetchwritelru(tagindex,setindex,assoc,lrutagind,lrusetind);
	}
}


 printf("with-prefetch\n");
 printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",reads,writes,hit,miss);
return 0;
}

