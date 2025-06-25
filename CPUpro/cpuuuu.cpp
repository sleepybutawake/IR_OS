#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// ���̿��ƿ�ṹ��
struct PCB {
    string name;
    int arrtime;
    int needtime;
    int starttime;
    int endtime;
    int runtime;
    int remaintime;
    int priority;

    PCB(string n, int at, int nt, int p)
        : name(n), arrtime(at), needtime(nt), starttime(0), endtime(0), runtime(0), remaintime(nt), priority(p) {
    }
};

// ��ͷ
void header() {
    cout << setw(8) << "������" << setw(10) << "����ʱ��" << setw(10) << "����ʱ��"
        << setw(10) << "��ʼʱ��" << setw(10) << "����ʱ��" << setw(10) << "����ʱ��"
        << setw(10) << "ʣ��ʱ��" << setw(8) << "���ȼ�" << endl;
}

// ������Ϣ
void printProcess(const PCB& pcb) {
    cout << setw(8) << pcb.name << setw(10) << pcb.arrtime << setw(10) << pcb.needtime
        << setw(10) << pcb.starttime << setw(10) << pcb.endtime << setw(10) << pcb.runtime
        << setw(10) << pcb.remaintime << setw(8) << pcb.priority << endl;
}

// ��������
void printReadyQueue(const vector<PCB>& readyQ) 
{
    cout << "��������״̬��" << endl;
    header();
    for (const auto& pcb : readyQ) 
    {
        printProcess(pcb);
    }
}

// ��ӡ����ɽ���
void Fprocesses(const vector<PCB>& finishedProcesses) {
    cout << "����ɵĽ��̣�" << endl;
    header();
    for (const auto& pcb : finishedProcesses) {
        printProcess(pcb);
    }
}

// ƽ����תʱ��
double averageTTime(const vector<PCB>& finishedP) {
    int totalTurnaroundTime = 0;
    for (const auto& pcb : finishedP) {
        totalTurnaroundTime += pcb.endtime - pcb.arrtime;
    }
    return static_cast<double>(totalTurnaroundTime) / finishedP.size();
}

// ����ƽ���ȴ�ʱ��
double averageWTime(const vector<PCB>& finishedP) {
    int totalWaitingTime = 0;
    for (const auto& pcb : finishedP) {
        totalWaitingTime += pcb.endtime - pcb.arrtime - pcb.needtime;
    }
    return static_cast<double>(totalWaitingTime) / finishedP.size();
}

// ������Ӧʱ��
double RTime(const vector<PCB>& finishedP) {
    int totalResponseTime = 0;
    for (const auto& pcb : finishedP) {
        totalResponseTime += pcb.starttime - pcb.arrtime;
    }
    return static_cast<double>(totalResponseTime) / finishedP.size();
}

// FCFS�㷨
void fcfs(vector<PCB> processes) {
    cout << "FCFS:" << endl;
    vector<PCB> readyQ;
    vector<PCB> finishedProcesses;
    int currentTime = 0;
    int busyTime = 0;
    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b) {
        return a.arrtime < b.arrtime;
        });

    while (!processes.empty() || !readyQ.empty()) 
    {
        // ������Ľ��̼����������
        for (auto it = processes.begin(); it != processes.end();) 
        {
            if (it->arrtime <= currentTime) 
            {
                readyQ.push_back(*it);
                it = processes.erase(it);
            }
            else 
            {
                ++it;
            }
        }

        if (!readyQ.empty()) {
            PCB& currentProcess = readyQ.front();
            if (currentProcess.runtime == 0) {
                currentProcess.starttime = currentTime;
            }
            currentProcess.runtime++;
            currentProcess.remaintime--;

            cout << "ʱ�䣺" << currentTime << endl;
            cout << "�����еĽ��̣�" << endl;
            header();
            printProcess(currentProcess);

            if (currentProcess.remaintime == 0) {
                currentProcess.endtime = currentTime + 1;
                finishedProcesses.push_back(currentProcess);
                readyQ.erase(readyQ.begin());
                Fprocesses(finishedProcesses);
            }
            else {
                Fprocesses(finishedProcesses);
            }

            printReadyQueue(readyQ);
            cout << endl;
        }

        currentTime++;
    }

    double throughput = static_cast<double>(finishedProcesses.size()) / currentTime;
    double avgTurnaroundTime = averageTTime(finishedProcesses);
    double avgWaitingTime = averageWTime(finishedProcesses);
    double responseTime = RTime(finishedProcesses);

    cout << "CPU Utilization: 100%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Response time: " << responseTime << endl;
    cout << endl;
}

// �����ҵ���ȣ�SJF�������㷨
void sjf(vector<PCB> processes) {
    cout << "           SJF           " << endl;
    vector<PCB> readyQueue;
    vector<PCB> finishedProcesses;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b) {
        return a.arrtime < b.arrtime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        // �����������
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrtime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            auto shortestJob = min_element(readyQueue.begin(), readyQueue.end(), [](const PCB& a, const PCB& b) {
                return a.needtime < b.needtime;
                });
            PCB& currentProcess = *shortestJob;
            if (currentProcess.runtime == 0) {
                currentProcess.starttime = currentTime;
            }
            currentProcess.runtime++;
            currentProcess.remaintime--;

            cout << "ʱ�䣺" << currentTime << endl;
            cout << "�����еĽ��̣�" << endl;
            header();
            printProcess(currentProcess);

            if (currentProcess.remaintime == 0) {
                currentProcess.endtime = currentTime + 1;
                finishedProcesses.push_back(currentProcess);
                readyQueue.erase(shortestJob);
                Fprocesses(finishedProcesses);
            }
            else {
                Fprocesses(finishedProcesses);
            }

            printReadyQueue(readyQueue);
            cout << endl;
        }

        currentTime++;
    }

    double throughput = static_cast<double>(finishedProcesses.size()) / currentTime;
    double avgTurnaroundTime = averageTTime(finishedProcesses);
    double avgWaitingTime = averageWTime(finishedProcesses);
    double responseTime = RTime(finishedProcesses);

    cout << "CPU Utilization: 100%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Response time: " << responseTime << endl;
    cout << endl;
}

// ���ʣ��ʱ�����ȣ�SRTF�������㷨
void srtf(vector<PCB> processes) {
    cout << "SRTF��" << endl;
    vector<PCB> readyQueue;
    vector<PCB> finishedProcesses;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b) {
        return a.arrtime < b.arrtime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        // ������Ľ��̼����������
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrtime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            auto shortestRemainTimeJob = min_element(readyQueue.begin(), readyQueue.end(), [](const PCB& a, const PCB& b) {
                return a.remaintime < b.remaintime;
                });
            PCB& currentProcess = *shortestRemainTimeJob;
            if (currentProcess.runtime == 0) {
                currentProcess.starttime = currentTime;
            }
            currentProcess.runtime++;
            currentProcess.remaintime--;

            cout << "ʱ�䣺" << currentTime << endl;
            cout << "�����еĽ��̣�" << endl;
            header();
            printProcess(currentProcess);

            if (currentProcess.remaintime == 0) {
                currentProcess.endtime = currentTime + 1;
                finishedProcesses.push_back(currentProcess);
                readyQueue.erase(shortestRemainTimeJob);
                Fprocesses(finishedProcesses);
            }
            else {
                Fprocesses(finishedProcesses);
            }

            printReadyQueue(readyQueue);
            cout << endl;
        }

        currentTime++;
    }

    double throughput = static_cast<double>(finishedProcesses.size()) / currentTime;
    double avgTurnaroundTime = averageTTime(finishedProcesses);
    double avgWaitingTime = averageWTime(finishedProcesses);
    double responseTime = RTime(finishedProcesses);

    cout << "CPU Utilization: 100%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Response time: " << responseTime << endl;
    cout << endl;
}

// ���ȼ������㷨
void priority(vector<PCB> processes) {
    cout << "���ȼ������㷨��" << endl;
    vector<PCB> readyQueue;
    vector<PCB> finishedProcesses;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b) {
        return a.arrtime < b.arrtime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        // ������Ľ��̼����������
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrtime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            auto highestPriorityJob = min_element(readyQueue.begin(), readyQueue.end(), [](const PCB& a, const PCB& b) {
                return a.priority < b.priority;
                });
            PCB& currentProcess = *highestPriorityJob;
            if (currentProcess.runtime == 0) {
                currentProcess.starttime = currentTime;
            }
            currentProcess.runtime++;
            currentProcess.remaintime--;

            cout << "ʱ�䣺" << currentTime << endl;
            cout << "�����еĽ��̣�" << endl;
            header();
            printProcess(currentProcess);

            if (currentProcess.remaintime == 0) {
                currentProcess.endtime = currentTime + 1;
                finishedProcesses.push_back(currentProcess);
                readyQueue.erase(highestPriorityJob);
                Fprocesses(finishedProcesses);
            }
            else {
                Fprocesses(finishedProcesses);
            }

            printReadyQueue(readyQueue);
            cout << endl;
        }

        currentTime++;
    }

    double throughput = static_cast<double>(finishedProcesses.size()) / currentTime;
    double avgTurnaroundTime = averageTTime(finishedProcesses);
    double avgWaitingTime = averageWTime(finishedProcesses);
    double responseTime = RTime(finishedProcesses);

    cout << "CPU Utilization: 100%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Response time: " << responseTime << endl;
    cout << endl;
}

// ʱ��Ƭ��ת��RR�������㷨
void rr(vector<PCB> processes, int timeSlice) {
    cout << "RR:" << endl;
    vector<PCB> readyQueue;
    vector<PCB> finishedProcesses;
    int currentTime = 0;

    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b) {
        return a.arrtime < b.arrtime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        // ������Ľ��̼����������
        for (auto it = processes.begin(); it != processes.end();) {
            if (it->arrtime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            PCB& currentProcess = readyQueue.front();
            if (currentProcess.runtime == 0) {
                currentProcess.starttime = currentTime;
            }

            int executedTime = 0;
            while (executedTime < timeSlice && currentProcess.remaintime > 0) {
                currentProcess.runtime++;
                currentProcess.remaintime--;
                executedTime++;
                currentTime++;

                cout << "ʱ�䣺" << currentTime - 1 << endl;
                cout << "�����еĽ��̣�" << endl;
                header();
                printProcess(currentProcess);

                if (currentProcess.remaintime == 0) {
                    currentProcess.endtime = currentTime;
                    finishedProcesses.push_back(currentProcess);
                    readyQueue.erase(readyQueue.begin());
                    Fprocesses(finishedProcesses);
                }
                else {
                    Fprocesses(finishedProcesses);
                }

                printReadyQueue(readyQueue);
                cout << endl;
            }

            if (currentProcess.remaintime > 0) {
                readyQueue.push_back(currentProcess);
                readyQueue.erase(readyQueue.begin());
            }
        }
        else {
            currentTime++;
        }
    }

    double throughput = static_cast<double>(finishedProcesses.size()) / currentTime;
    double avgTurnaroundTime = averageTTime(finishedProcesses);
    double avgWaitingTime = averageWTime(finishedProcesses);
    double responseTime = RTime(finishedProcesses);

    cout << "CPU Utilization: 100%" << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Response time: " << responseTime << endl;
    cout << endl;
}

int main() {
    vector<PCB> processes = {
        PCB("B", 2, 2, 1),
        PCB("A", 0, 8, 1),
        PCB("C", 5, 9, 2),
        PCB("D", 5, 5, 1),
        PCB("E", 12, 6, 4),
        PCB("F", 15, 10, 2)
    };

    fcfs(processes);
    sjf(processes);
    srtf(processes);
    priority(processes);
    rr(processes, 4);

    return 0;
}