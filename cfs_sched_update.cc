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
        vector<Task*> temp_tasks;

        while (!arrival_queue.empty() && arrival_queue.front()->start_time == tick) {
            Task* new_task = arrival_queue.front();
            arrival_queue.pop();
            new_task->vruntime = min_vruntime;
            temp_tasks.push_back(new_task);
        }

        sort(temp_tasks.begin(), temp_tasks.end(), [](Task* a, Task* b) {
            return a->id < b->id;
        });

        for (Task* t : temp_tasks) {
            task_tree.Insert(t->vruntime, t);
        }
    }

    void scheduleTask() {
        if (!(task_tree.Size() == 0)) {
            unsigned int best_task_key = task_tree.Min();  // Min() returns the smallest key
            Task* best_task = task_tree.Get(best_task_key); // Get() retrieves the task associated with that key

            if (best_task != nullptr) {
                if (running_task == nullptr || best_task_key < running_task->vruntime) {
                    if (running_task != nullptr) {
                        task_tree.Insert(running_task->vruntime, running_task);
                    }
                    running_task = best_task;
                    task_tree.Remove(best_task_key);  // Remove using the key
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
