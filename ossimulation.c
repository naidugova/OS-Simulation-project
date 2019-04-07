/*
          CPU schedules N processes which arrive at different time intervals and each process is
allocated the CPU for a specific user input time unit, processes are scheduled using a preemptive
round robin scheduling algorithm. Each process must be assigned a numerical priority, with a
higher number indicating a higher relative priority. In addition to the processes one task has
priority 0. The length of a time quantum is T units, where T is the custom time considered as
time quantum for processing. If a process is preempted by a higher-priority process, the
preempted process is placed at the end of the queue. Design a scheduler so that the task with
priority 0 does not starve for resources and gets the CPU at some time unit to execute. Also
compute waiting time, turn around.
*/
#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>

//Defining a structure for process
struct process
{
	int processNumber;
	int priority;
	int arrivalTime;
	int burstTime;
	int turnAroundTime;
	int waitingTime;
    int completionTime;
};

//static variable for maintaining no of completed processes
int completed_processes=0;
int queued_processes=0;
int current_process=-1;
//global variables
int numberOfProcesses=0,timeQuantum=0; 
int N = 1;
int totalTime=0;
int aging=0;
int aging_time=-1;
float avg_turn_around_time=0;
float avg_waiting_time=0;



int main()
{
	printf("=============================================================================\n");
	printf("___________________________________________CPU SCHEDULER_____________________\n\n");
    printf("\t\t\t\t\tOS SIMULATION BASED-1\n\n");
    
    //Taking input of no of processes and time quantum
    printf("\nEnter the number of processes :  ");
    scanf("%d",&numberOfProcesses);
    printf("\nEnter the time quantum to be allocated :  ");
    scanf("%d",&timeQuantum);
    
    N+=numberOfProcesses;
    
    //Declaring array of structures structures
    struct process available[N];
    struct process completed[N];
    struct process queue[N];

    
    //Taking input about processes from user
    //declaring loop variable
    int i=0;
    for( i ; i<N ; ++i )
    {
    	
    	if(i==0)
    	{
    		printf("\nTask with 0 priority\n");
    		available[i].priority=0;
    		available[i].processNumber=i;
    		printf("\nEnter arrivalTime  :");
    		scanf("%d",&available[i].arrivalTime);
    		printf("\nEnter burstTime  :");
    		scanf("%d",&available[i].burstTime);

    		//Initializing other values in structure
    		available[i].turnAroundTime=0;
    		available[i].completionTime=0;
    		available[i].waitingTime=0;

    	}
    	else
    	{
    		printf("\nprocess %d : \n",i);
    		available[i].processNumber=i;
            printf("\nEnter the priority  :");
    		scanf("%d",&available[i].priority);
    		printf("\nEnter arrivalTime  :");
    		scanf("%d",&available[i].arrivalTime);
    		printf("\nEnter burstTime  :");
    		scanf("%d",&available[i].burstTime);

    		//Initializing other values in structure
    		available[i].turnAroundTime=0;
    		available[i].completionTime=0;
    		available[i].waitingTime=0;
    	
    	}
    }

    
    //Print the entered processes
    printf("\n\n\t\t\t\t The processes are \n");
    printf("\n| Process number| Arrival time  |   Priority    |  Burst time  |\n");
    printf("-------------------------------------------------------------------------\n");
    for (i=0; i < N; ++i)
    {
    	printf("|\t %d \t|\t %d \t|\t %d \t|\t %d     |\n ",available[i].processNumber,available[i].arrivalTime,available[i].priority,available[i].burstTime);
    }


    //calculating total time of execution

    i=0;//reset the loop variable
    int maxat=0;//maximum arrival time among processes
    int maxatp=0;//Process having maximum arrival time
    for (i; i<N ; ++i)
    {
    	if(available[i].arrivalTime>maxat)
    	{
    		maxat=available[i].arrivalTime;
    		maxatp=i;
		}
    }

    //calculating how many times aging concept should be applied
    aging=((available[maxatp].arrivalTime-available[0].arrivalTime)/7)-1;
    if (aging==-1 | aging==0)
    {
    	printf("\nThe aging is done 0 times");
    }
    if(aging!=-1 & aging!=0)
    {
    	printf("\nThe aging is done %d times",aging);
    	aging_time=available[0].arrivalTime+7;
    }
       
    //checking a condition for total time calculation 
    i=0;
    for (i; i<N ; ++i)
    {
    	totalTime += available[i].burstTime;
    }
    
    if(totalTime<(maxat+available[maxatp].burstTime))
       totalTime=maxat+available[maxatp].burstTime;

    printf("\nTotal time of execution is : %d \n",totalTime);
    
    
    //looping over total time
    for ( i = 0; i <=totalTime; ++i)
    {
        printf("\nAfter time %d second \n",i);
        int j=0;
    	//checking for aging
    	if (aging>0)
    	{
    		if (aging_time==i)
    		{
    			printf("\nIncreasing priority of processes in queue by 2 to prevent starving \n");
    			for (j; j < queued_processes; ++j)
    			{
    				queue[j].priority += 2;
    			}
    			--aging;
    			aging_time += 7;
    		}
    	}
    	
    	//If current process is completed the adding it to completed array
    	if (current_process!=-1)
    	{
    	if(queue[current_process].burstTime==0)
    	{
    		int mainpro;
    		printf("\nProcess %d is completed\n",queue[current_process].processNumber);
    		for (j = 0; j < N; ++j)
    		{
    			if(queue[current_process].processNumber==available[j].processNumber)
    				mainpro=j;
    		}
    		queue[current_process].completionTime=i;
    		queue[current_process].turnAroundTime=i-queue[current_process].arrivalTime;
    		queue[current_process].waitingTime=queue[current_process].turnAroundTime - available[mainpro].burstTime;
    		completed[completed_processes]=queue[current_process];
    		queue[current_process].burstTime = available[mainpro].burstTime;
    		++completed_processes;
    		//Removing current process from queue
    	    j=current_process;
    	    for (j; j < queued_processes; ++j)
    	    {
    		    queue[j]=queue[j+1];
    	    }
    	    --queued_processes;

    	}
    	else
    	{

    		if (i==timeQuantum)
    		{
    			printf("\nTime quantum expired \n");
    			j=current_process;
    			struct process sample;
    			sample=queue[j];
    	        for (j; j < queued_processes; ++j)
    	        {
    	        	
    		        queue[j]=queue[j+1];
    	        }
    	        queue[j-1]=sample;
    		}
    	}
        }

    	//checking processes at time i and adding to queue
    
        
    	for (j=0;j<N; ++j)
    	{
    		
    		if (available[j].arrivalTime==i)
    		{
    			printf("\nAdding process %d to queue",j);
    			queue[queued_processes]=available[j];
    			++queued_processes;
    		}
    	}

    	//finding process to be executed from queue

    	for(j=0;j<queued_processes;++j)
    	{
            int maxpr=0;
    		if(queue[j].priority>=maxpr)
    		{
    			maxpr=queue[j].priority;
    			current_process=j;
    		}
           
    	}

    	

    	//Executing current process for one second
    	if(queue[current_process].burstTime!=0)
        printf("\nDecreasing burst time of %d process by 1 \n",queue[current_process].processNumber );
    	queue[current_process].burstTime-=1;

    }
    
    
    printf("\n\n\t\t\t\t The completed processes are \n");
    printf("\n| Process number| Arrival time  |   Priority    |  Burst time  |   Completion time  |  Turnaroundtime  |   Waiting time    | \n");
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    for (i=0; i < N; ++i)
    {
    	static int total_turn_around_time=0;
    	static int total_waiting_time=0;
    	printf("|\t%2d  \t|\t%2d  \t|\t%2d  \t|\t%2d     |\t  %2d        | \t %2d            | \t %2d        |\n ",completed[i].processNumber,completed[i].arrivalTime,completed[i].priority,completed[i].burstTime,completed[i].completionTime,completed[i].turnAroundTime,completed[i].waitingTime);
        total_turn_around_time += completed[i].turnAroundTime;
        total_waiting_time += completed[i].waitingTime;
        if(i==(N-1))
        {
        	avg_turn_around_time = (float) total_turn_around_time/N;
        	avg_waiting_time = (float) total_waiting_time/N;
        }

    }

    printf("\n The average turn around time of all processes is : %.3f \n",avg_turn_around_time );
    printf("\n The average waiting time of all processes is : %.3f \n", avg_waiting_time);



	return 0;
}
