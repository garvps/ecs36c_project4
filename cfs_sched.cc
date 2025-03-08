#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>
#include "multimap.h"

using namespace std;

class Task {
public:
    char id;
    unsigned int start_time;
    unsigned int duration;
    unsigned int runtime;
    unsigned int vruntime;

    Task(char id, unsigned int start_time, unsigned int duration)
        : id(id), start_time(start_time), duration(duration), runtime(0), vruntime(0) {}

    void run() {
        runtime++;
        vruntime++;
    }

    bool isComplete() const {
        return runtime >= duration;
    }
};

class CFSScheduler {
private:
    Multimap<unsigned int, Task*> task_tree;
    queue<Task*> arrival_queue;
    unsigned int min_vruntime;
    unsigned int tick;
    Task* running_task;

public:
    CFSScheduler() : min_vruntime(0), tick(0), running_task(nullptr) {}

    void addTask(Task* task) {
        arrival_queue.push(task);
    }

    void run() {
        while (!(task_tree.Size() == 0) || !arrival_queue.empty() || running_task != nullptr) {
            processArrivals();
            scheduleTask();
            tick++;
        }
    }

void processArrivals() {
    vector<Task*> arrivingTasks;

     while (!arrival_queue.empty() && arrival_queue.front()->start_time == tick) {
        Task* currentTask = arrival_queue.front();
        arrival_queue.pop();
        currentTask->vruntime = min_vruntime;
        arrivingTasks.push_back(currentTask);
    }

    sort(arrivingTasks.begin(), arrivingTasks.end(), [](const Task* t1, const Task* t2) {
        return t1->id < t2->id;
    });

    for (Task* task : arrivingTasks) {
        task_tree.Insert(task->vruntime, task);
    }
}


    void scheduleTask() {
        if (!(task_tree.Size() == 0)) {
            unsigned int best_task_key = task_tree.Min();
            Task* best_task = task_tree.Get(best_task_key);

            if (best_task != nullptr) {
                if (running_task == nullptr || best_task_key < running_task->vruntime) {
                    if (running_task != nullptr) {
                        task_tree.Insert(running_task->vruntime, running_task);
                    }
                    running_task = best_task;
                    task_tree.Remove(best_task_key);
                    min_vruntime = running_task->vruntime;
                }
            }
        }

        if (running_task == nullptr) {
            cout << tick << " [0]: _" << endl;
            return;
        }

        running_task->run();

        cout << tick << " [" << task_tree.Size() + 1 << "]: " << running_task->id;
        if (running_task->isComplete()) {
            cout << "*";
            delete running_task;
            running_task = nullptr;
        }
        cout << endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <task_file.dat>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: cannot open file " << argv[1] << endl;
        return 1;
    }

    CFSScheduler scheduler;
    char id;
    unsigned int start, duration;

    while (file >> id >> start >> duration) {
        scheduler.addTask(new Task(id, start, duration));
    }

    file.close();
    scheduler.run();
    return 0;
}
