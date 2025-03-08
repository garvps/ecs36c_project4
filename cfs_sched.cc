#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "multimap.h"

class Task {

    private:
    // task id
    char id;
    // start time of task
    unsigned int start_time;
    // task duration
    unsigned int duration;
    // task runtime
    unsigned int runtime;
    // task virtual runtime
    unsigned int vruntime;

    public:
    // class constructor
    Task(char id, unsigned int start_time, unsigned int duration)
        : id(id), start_time(start_time), duration(duration), runtime(0), vruntime(0) {}

    // getters for private class variables
    char returnID () {
        return id;
    }

    unsigned int returnStartTime () {
        return start_time;
    }

    unsigned int returnDuration () {
        return duration;
    }

    unsigned int returnRuntime () {
        return runtime;
    }

    unsigned int returnVruntime () {
        return vruntime;
    }

    // methods to update runtimes and check if tasks are completed
    void run() {
        runtime++;
        vruntime++;
    }

    bool isComplete() const {
        return runtime >= duration;
    }

    // setter for updating virtual run time
    void setVruntime (unsigned int new_v_rtime) {
        vruntime = new_v_rtime;
    }
};

class CFSScheduler {

    private:
    // the timeline of tasks ordered by virtual runtime
    Multimap<unsigned int, Task*> timeline;

    // tasks waiting to be added (not yet at their start time)
    std::vector<Task*> pending_tasks;

    // global min virtual runtime
    unsigned int min_vruntime;

    // current tick value
    unsigned int tick;

    // currently running task
    Task* current_task;

    public:
    CFSScheduler() : min_vruntime(0), tick(0), current_task(nullptr) {}

    // add a task to the scheduler
    void addTask(Task* task) {

        pending_tasks.push_back(task);
    }

    // main scheduling function
    void schedule() {

        // sort pending tasks by start time then by id
        std::sort(pending_tasks.begin(), pending_tasks.end(),
            [](Task* a, Task* b) {
                if (a->returnStartTime() == b->returnStartTime())
                    return a->returnID() < b->returnID();
                return a->returnStartTime() < b->returnStartTime();
            });

        // scheduling loop
        while (!pending_tasks.empty() || timeline.Size() > 0 || current_task != nullptr) {
            addTasksAtCurrentTick();
            checkTaskYield();
            getNextTask();
            runCurrentTask();
            tick++;
        }
    }

    private:
    // add tasks that should start at the current tick
    void addTasksAtCurrentTick() {

        std::vector<Task*> tasks_to_add;

        // identify tasks that should start now
        auto it = pending_tasks.begin();
        while (it != pending_tasks.end()) {
            if ((*it)->returnStartTime() == tick) {
                // initialize their vruntime to the current min_vruntime
                (*it)->setVruntime(min_vruntime);
                tasks_to_add.push_back(*it);
                it = pending_tasks.erase(it);
            }

            else if ((*it)->returnStartTime() > tick) {
                // future tasks
                break;
            }

            else {
                it++;
            }
        }

        // sort by id and add to timeline
        std::sort(tasks_to_add.begin(), tasks_to_add.end(),
            [](Task* a, Task* b) { return a->returnID() < b->returnID(); });

        for (Task* task : tasks_to_add)
            timeline.Insert(task->returnVruntime(), task);

    }

    // check if current task should yeild
    void checkTaskYield() {

        if (current_task != nullptr && timeline.Size() > 0) {

            unsigned int min_vruntime_in_timeline = timeline.Min();

            // reprioritize according to shorter vruntime
            if (min_vruntime_in_timeline < current_task->returnVruntime()) {

                timeline.Insert(current_task->returnVruntime(), current_task);
                current_task = nullptr;
            }
        }
    }

    // get next task to run
    void getNextTask() {

        if (current_task == nullptr && timeline.Size() > 0) {
            unsigned int min_vruntime_key = timeline.Min();
            current_task = timeline.Get(min_vruntime_key);
            timeline.Remove(min_vruntime_key);

            // update global min_vruntime
            min_vruntime = current_task->returnVruntime();
        }
    }

    // run current task and report status
    void runCurrentTask() {

        // report scheduling status
        std::cout << tick << " [";

        if (current_task != nullptr) {
            std::cout << timeline.Size() + 1;
        } else {
            std::cout << "0";
        }

        std::cout << "]: ";

        if (current_task == nullptr) {
            std::cout << "_" << std::endl;
            return;
        }

        std::cout << current_task->returnID();

        // run task for one tic
        current_task->run();

        // check if the task is completed
        if (current_task->isComplete()) {
            std::cout << "*";
            delete current_task;
            current_task = nullptr;
        }

        std::cout << std::endl;
    }
};

int main(int argc, char* argv[]) {

    // check for 2 args
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <task_file.dat>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    // raise error if file doesn't exist/can't be opened
    if (!file) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    // scheduler object
    CFSScheduler scheduler;
    char id;
    unsigned int start, duration;

    while (file >> id >> start >> duration) {
        scheduler.addTask(new Task(id, start, duration));
    }

    file.close();
    scheduler.schedule();

    return 0;
}
