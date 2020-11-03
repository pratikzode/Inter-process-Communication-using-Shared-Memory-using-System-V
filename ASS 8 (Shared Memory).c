#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct sharedMemory
{
	int status;
	char data[10][100];
	int n;
}sharedMemory;

int main()
{
	int shmID, i, j;
	key_t key;
	char ch;
	sharedMemory *shm_Ptr;
	key=ftok(".",'A');
	printf("\nGenerated Key Value: %d", key);
	shmID=shmget(key,sizeof(sharedMemory),IPC_CREAT|0666);
	
	if(shmID<0)
	{
		printf("\nShared Memory is not created!!\n");
		exit(-1);
	}
	else
	{
		printf("\nShared Memory is created\n");
		printf("\nShared memory ID: %d",shmID);
	
		shm_Ptr=(sharedMemory*)shmat(shmID, NULL, 0);
		if(shm_Ptr>=0)
			printf("\nShared Memory is Attached!!\n");
		else
		{
			printf("\nShared memory is  not attached");
			exit(-1);
		}
		shm_Ptr->status=-1;	//Data not filled in shared memory
		printf("\nData is not filled in Shared Memory!!\n");
		printf("\nEnter the Number of Data Fields required: ");
		scanf("%d", &(shm_Ptr->n)); 
		printf("\nEnter Data:\n");
		for(j=0;j<(shm_Ptr->n);j++)
		{
			printf("\nField %d: ",j+1);
			i=0;
			ch=getchar();
			while(ch!='#')
			{
				shm_Ptr->data[j][i++]=ch;
				ch=getchar();
			}
			shm_Ptr->data[j][i]='\0';
		}
				
		printf("Entered Data:\n\n");
		for(j=0; j<(shm_Ptr->n);j++)
		{
			printf("\nField %d: %s ",j+1, shm_Ptr->data[j]);
		}
		
		shm_Ptr->status=0;	//Data filled in Shared Memory
		
		while(shm_Ptr->status!=1)	//till the data is not read by client from shared memory
		{
			printf("\nClient is Reading Data From Shared Memory\n");
			sleep(2);
		}
		shmdt(shm_Ptr);
		
		shmctl(shmID,IPC_RMID,0);
		printf("\n\nTerminated!!");
		return 0;
	}
}
CLIENT PROCESS:
#include<sys/shm.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct sharedMemory
{
	int status;
	char data[10][100];
	int n;
}sharedMemory;

int main()
{
	int shmID, i, j;
	key_t key;
	char ch;
	sharedMemory *shm_Ptr;
	key=ftok(".",'A');
	printf("\nGenerated Key Value: %d", key);
	shmID=shmget(key,sizeof(sharedMemory),IPC_CREAT|0666);
	
	if(shmID<0)
	{
		printf("\nShared Memory is not created!!\n");
		exit(-1);
	}
	else
	{
		printf("\nShared Memory is created\n");
		printf("\nShared memory ID: %d",shmID);
		
		shm_Ptr=(sharedMemory*)shmat(shmID, NULL, 0);
		if(shm_Ptr>=0)
			printf("\nShared Memory is Attached!!\n");
		else
		{
			printf("\nShared memory is  not attached");
			exit(-1);
		}
		
		while(shm_Ptr->status==-1)			//data not filled on shared memory3
		{
			printf("\nData is  not filled by server");
			sleep(2);
		}
		printf("\n\nClient is Reading  the Data from Shared Memory!!]\n");
		
		for(j=0; j<(shm_Ptr->n);j++)
		{
			printf("\nField %d: %s ",j+1, shm_Ptr->data[j]);
		}
		shm_Ptr->status=1;				//data read by client
		
		shmdt(shm_Ptr);
		printf("\n\nTerminated");
		return 0; 
	}
}

