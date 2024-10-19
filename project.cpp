#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>

using namespace std;

// Job structure
struct Job {
    int arrivalTime;
    int coresRequired;
    int memoryRequired;
    int executionTime;
};

// Worker Node structure
struct WorkerNode {
    int availableCores;
    int availableMemory;
};

// Job Scheduler class
class JobScheduler {
private:
    queue<Job> jobQueue;
    vector<WorkerNode> workerNodes;
    int currentTime;
    int policy; // 0: FCFS, 1: Smallest Job First, 2: Short Duration Job First
    int nodePolicy; // 0: First Fit, 1: Best Fit, 2: Worst Fit
    double avgCpuUtilization;
    double avgMemoryUtilization;

public:
    JobScheduler(int policy, int nodePolicy) {
        this->policy = policy;
        this->nodePolicy = nodePolicy;
        currentTime = 0;
        for (int i = 0; i < 128; i++) {
            WorkerNode node;
            node.availableCores = 24;
            node.availableMemory = 64;
            workerNodes.push_back(node);
        }
    }

    void addJob(Job job) {
        jobQueue.push(job);
    }

    void scheduleJobs() {
        while (!jobQueue.empty()) {
            Job job = jobQueue.front();
            jobQueue.pop();
            if (job.arrivalTime <= currentTime) {
                allocateWorkerNode(job);
            } else {
                jobQueue.push(job);
                break;
            }
        }
        currentTime++;
    }

    void allocateWorkerNode(Job job) {
        vector<WorkerNode> availableNodes;
        for (WorkerNode node : workerNodes) {
            if (node.availableCores >= job.coresRequired && node.availableMemory >= job.memoryRequired) {
                availableNodes.push_back(node);
            }
        }
        if (!availableNodes.empty()) {
            WorkerNode allocatedNode;
            switch (nodePolicy) {
                case 0: // First Fit
                    allocatedNode = availableNodes[0];
                    break;
                case 1: // Best Fit
                    allocatedNode = *min_element(availableNodes.begin(), availableNodes.end(),
                            [](WorkerNode a, WorkerNode b) {
                                return a.availableCores < b.availableCores ||
                                       (a.availableCores == b.availableCores && a.availableMemory < b.availableMemory);
                            });
                    break;
                case 2: // Worst Fit
                    allocatedNode = *max_element(availableNodes.begin(), availableNodes.end(),
                            [](WorkerNode a, WorkerNode b) {
                                return a.availableCores < b.availableCores ||
                                       (a.availableCores == b.availableCores && a.availableMemory < b.availableMemory);
                            });
                    break;
            }
            allocatedNode.availableCores -= job.coresRequired;
            allocatedNode.availableMemory -= job.memoryRequired;
            // Simulate job execution
            for (int i = 0; i < job.executionTime; i++) {
                // Update CPU and Memory utilization
                // ...
            }
            allocatedNode.availableCores += job.coresRequired;
            allocatedNode.availableMemory += job.memoryRequired;
        } else {
            jobQueue.push(job);
        }
    }

    void printUtilization() {
        // Calculate average CPU and Memory utilization per day
        avgCpuUtilization = 0;
        avgMemoryUtilization = 0;
        for (WorkerNode node : workerNodes) {
            avgCpuUtilization += (24 - node.availableCores) / 24.0;
            avgMemoryUtilization += (64 - node.availableMemory) / 64.0;
        }
        avgCpuUtilization /= 128;
        avgMemoryUtilization /= 128;
        cout << "Average CPU Utilization: " << avgCpuUtilization << endl;
        cout << "Average Memory Utilization: " << avgMemoryUtilization << endl;
    }

    void generateCSV() {
        ofstream csvFile("utilization.csv");
        csvFile << "Policy,Node Policy,Average CPU Utilization,Average Memory Utilization" << endl;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                JobScheduler scheduler(i, j);
                // Add jobs to the scheduler
                Job job1;
                job1.arrivalTime = 0;
                job1.coresRequired = 4;
                job1.memoryRequired = 16;
                job1.executionTime = 2;
                scheduler.addJob(job1);
                // ...
                scheduler.scheduleJobs();
                scheduler.printUtilization();
                            csvFile << i << "," << j << "," << scheduler.avgCpuUtilization << "," << scheduler.avgMemoryUtilization << endl;
            }
        }
        csvFile.close();
    }
};

int main() {
    // Example usage of the Job Scheduler
    JobScheduler scheduler(0, 0); // FCFS, First Fit

    // Add jobs to the scheduler
    Job job1;
    job1.arrivalTime = 0;
    job1.coresRequired = 4;
    job1.memoryRequired = 16;
    job1.executionTime = 2;
    scheduler.addJob(job1);

    Job job2;
    job2.arrivalTime = 0;
    job2.coresRequired = 8;
    job2.memoryRequired = 32;
    job2.executionTime = 3;
    scheduler.addJob(job2);

    Job job3;
    job3.arrivalTime = 1;
    job3.coresRequired = 2;
    job3.memoryRequired = 8;
    job3.executionTime = 1;
    scheduler.addJob(job3);

    // Simulate job scheduling
    for (int hour = 0; hour < 24; hour++) {
        scheduler.scheduleJobs();
    }

    // Print utilization results and generate CSV file
    scheduler.printUtilization();
    scheduler.generateCSV();

    return 0;
}