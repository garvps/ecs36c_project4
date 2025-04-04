# Multimap C++ Project

This project implements a custom **self-balancing red-black multimap** in C++, along with a **CFS (Completely Fair Scheduler)** simulation that uses the multimap to manage tasks. Unit tests are also included to validate the multimap's functionality.

---

## Project Structure

- `multimap.h`: Implementation of a templated red-black **Multimap** supporting multiple values per key.
- `test_multimap.cc`: Google Test cases to validate multimap functionality.
- `cfs_sched.cc`: Simulates a Linux-style Completely Fair Scheduler using the multimap to schedule tasks based on virtual runtime.

---

## Multimap Features (`multimap.h`)

- **Multiple values per key** using `std::vector<V>`.
- **Self-balancing** red-black tree implementation.
- **Core operations**:
  - `Insert(key, value)` – Add values to a key.
  - `Remove(key)` – Remove the first value; if no values remain, removes the key.
  - `Get(key)` – Returns the first value associated with the key.
  - `Contains(key)`, `Min()`, `Max()`, `Size()`, and `Print()`.

---

## Scheduler Overview (`cfs_sched.cc`)

- Tasks are added with ID, start time, and duration.
- Uses `Multimap<unsigned int, Task*>` to track tasks by virtual runtime.
- Mimics Linux’s CFS behavior: picks the task with the lowest virtual runtime each tick.
- Handles task start, execution, and completion across time steps.

---

## Running Tests (`test_multimap.cc`)

Make sure you have [Google Test](https://github.com/google/googletest) installed.

To compile the test suite:

```bash
g++ -std=c++17 -lgtest -lgtest_main -pthread test_multimap.cc -o test_multimap
./test_multimap
