#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Structure to represent a process
typedef struct {
    int pid;            //Process ID
    int arrival_time;   //Time when the process arrives
    int burst_time;     //Time the process requires to execute
    int priority;       //Priority of the process (not used in this implementation)
} Process;

//Function to read process data from "processes.txt" file
void read_processes(Process processes[], int *count) {
    FILE *file = fopen("processes.txt", "r");  //Open file in read mode
    if (!file) {
        printf("Error opening file\n");
        *count = 0;  //Set count to 0 if file fails to open
        return;
    }

    fscanf(file, "%*s %*s %*s %*s");  //Skip the header line (PID Arrival Burst Priority)
    *count = 0;  //Initialize count to 0

    //Read data from file into the array
    while (fscanf(file, "%d %d %d %d", &processes[*count].pid, &processes[*count].arrival_time,
                  &processes[*count].burst_time, &processes[*count].priority) == 4) {
        (*count)++;
    }

    fclose(file);  //Close file after reading
}

//First-Come, First-Served (FCFS) Scheduling Algorithm
void fcfs(Process processes[], int count) {
    int wt[count], tat[count], start_time[count], end_time[count];
    int total_wt = 0, total_tat = 0;
    int completion_time = 0, total_burst_time = 0;

    printf("\nGantt Chart:\n| ");

    //Loop through processes in arrival order
    for (int i = 0; i < count; i++) {
        //If CPU is idle, jump to arrival time
        if (completion_time < processes[i].arrival_time) {
            completion_time = processes[i].arrival_time;
        }

        //Start and end times for Gantt chart
        start_time[i] = completion_time;
        completion_time += processes[i].burst_time;
        end_time[i] = completion_time;

        //Calculate waiting time and turnaround time
        wt[i] = start_time[i] - processes[i].arrival_time;
        tat[i] = completion_time - processes[i].arrival_time;

        //Accumulate totals
        total_wt += wt[i];
        total_tat += tat[i];
        total_burst_time += processes[i].burst_time;

        //Print Gantt chart block
        printf("P%d | ", processes[i].pid);
    }

    //Print timeline under Gantt chart
    printf("\n%d  ", start_time[0]);
    for (int i = 0; i < count; i++) {
        printf("%d  ", end_time[i]);
    }

    //Print table of individual waiting and turnaround times
    printf("\n\nProcess\tWT\tTAT\n");
    for (int i = 0; i < count; i++) {
        printf("P%d\t%d\t%d\n", processes[i].pid, wt[i], tat[i]);
    }

    //CPU utilization calculation
    float cpu_utilization = ((float)total_burst_time / completion_time) * 100;

    //Print averages and utilization
    printf("\nAverage WT: %.2f\n", (float)total_wt / count);
    printf("Average TAT: %.2f\n", (float)total_tat / count);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}

//Shortest Job First (SJF) Scheduling Algorithm - Non-preemptive
void sjf(Process processes[], int count) {
    int wt[count], tat[count], start_time[count], end_time[count];
    bool completed[count];  //Track completed processes
    int total_wt = 0, total_tat = 0;
    int completion_time = 0, completed_count = 0, total_burst_time = 0;

    //Initialize completed array to false
    for (int i = 0; i < count; i++) completed[i] = false;

    printf("\nGantt Chart:\n| ");

    //While not all processes are completed
    while (completed_count < count) {
        int shortest = -1, min_burst = __INT_MAX__;

        //Find shortest job available at current time
        for (int i = 0; i < count; i++) {
            if (!completed[i] && processes[i].arrival_time <= completion_time &&
                processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                shortest = i;
            }
        }

        //If no process has arrived, increment time
        if (shortest == -1) {
            completion_time++;
            continue;
        }

        //Set start and end times
        start_time[shortest] = completion_time;
        completion_time += processes[shortest].burst_time;
        end_time[shortest] = completion_time;

        //Calculate WT and TAT
        tat[shortest] = completion_time - processes[shortest].arrival_time;
        wt[shortest] = tat[shortest] - processes[shortest].burst_time;

        completed[shortest] = true;  //Mark process as completed
        completed_count++;

        //Accumulate totals
        total_wt += wt[shortest];
        total_tat += tat[shortest];
        total_burst_time += processes[shortest].burst_time;

        //Print Gantt chart block
        printf("P%d | ", processes[shortest].pid);
    }

    //Print Gantt timeline
    printf("\n%d  ", start_time[0]);
    for (int i = 0; i < count; i++) {
        printf("%d  ", end_time[i]);
    }

    //Print table of individual waiting and turnaround times
    printf("\n\nProcess\tWT\tTAT\n");
    for (int i = 0; i < count; i++) {
        printf("P%d\t%d\t%d\n", processes[i].pid, wt[i], tat[i]);
    }

    //CPU utilization calculation
    float cpu_utilization = ((float)total_burst_time / completion_time) * 100;

    // Print averages and utilization
    printf("\nAverage WT: %.2f\n", (float)total_wt / count);
    printf("Average TAT: %.2f\n", (float)total_tat / count);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}

//Main function: Entry point of the program
int main() {
    Process processes[10];  //Array to store process data
    int count;

    //Read process data from the file
    read_processes(processes, &count);

    //If no processes were read, exit safely
    if (count == 0) {
        printf("No processes found. Make sure 'processes.txt' exists in the same directory.\n");
        return 1;
    }

    //Prompt user for scheduling choice
    printf("\nChoose Scheduling Algorithm:\n");
    printf("1. First-Come, First-Served (FCFS)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("Enter choice: ");

    int choice;
    scanf("%d", &choice);

    //Run the selected scheduling algorithm
    switch (choice) {
        case 1:
            fcfs(processes, count);
            break;
        case 2:
            sjf(processes, count);
            break;
        default:
            printf("Invalid choice!\n");
    }

    return 0;
}
