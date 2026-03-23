# 🍝 Philosophers — 42 Project | 125/125

![Score](https://img.shields.io/badge/Score-125%2F125-brightgreen)
![Language](https://img.shields.io/badge/Language-C-blue)
![42](https://img.shields.io/badge/42-Project-black)

## 📌 Project Goal

**Philosophers** is a concurrency project from the 42 curriculum based on the classic **Dining Philosophers Problem** (Dijkstra, 1965).

The goal is to simulate `N` philosophers sitting around a circular table, alternating between three states:
- 🤔 **Thinking**
- 🍴 **Eating** (requires picking up both the left and right fork)
- 😴 **Sleeping**

Your program must ensure that **no philosopher starves to death**, while correctly managing concurrent access to shared resources (forks) using **threads** and **mutexes**.

---

## 🧠 Key Concepts

### Threads (`pthread`)
Each philosopher is an independent **POSIX thread** running concurrently. All threads share the same memory space, which means shared data must be carefully protected.

| Function | Description |
|---|---|
| `pthread_create` | Spawn a new thread |
| `pthread_join` | Wait for a thread to finish |
| `pthread_detach` | Detach a thread (no need to join) |

### Mutexes (`pthread_mutex_t`)
A **mutex** (mutual exclusion lock) ensures that only one thread accesses a critical resource at a time — in this case, a **fork**.

| Function | Description |
|---|---|
| `pthread_mutex_init` | Initialize a mutex |
| `pthread_mutex_lock` | Acquire the lock (blocking) |
| `pthread_mutex_unlock` | Release the lock |
| `pthread_mutex_destroy` | Clean up the mutex |

### The Core Race Condition
Without protection, two philosophers could pick up the same fork simultaneously. Mutexes prevent this — each fork is protected by its own mutex.

### Deadlock
If every philosopher picks up their **left fork** simultaneously, all will wait forever for their **right fork** — this is a **deadlock**. Your program must avoid this scenario.

### Data Races
Accessing shared state (like `last_meal_time` or `is_dead`) from multiple threads without synchronization leads to **undefined behavior**. Every shared variable must be read/written under a mutex.

---

## 🚀 Usage

### Compilation
```bash
make
```

### Running
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Arguments

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` (ms) | A philosopher dies if they haven't started eating within this time since their last meal (or since the start) |
| `time_to_eat` (ms) | Time it takes to eat |
| `time_to_sleep` (ms) | Time spent sleeping |
| `[number_of_times_each_philosopher_must_eat]` | Optional: simulation ends when all philosophers have eaten this many times |

### Examples
```bash
# Basic simulation, no one should die
./philo 5 800 200 200

# Should never die, ends when all eat 7 times
./philo 5 800 200 200 7

# One philosopher — special edge case
./philo 1 800 200 200

# A philosopher must die
./philo 4 310 200 100
```

---

## 🏗️ Implementation Logic

### Overall Architecture

```
main()
 ├── Parse & validate arguments
 ├── Initialize forks (mutexes) + philosopher structs
 ├── Spawn N philosopher threads
 ├── Spawn 1 monitor thread
 ├── Join all threads
 └── Clean up (destroy mutexes)
```

### Philosopher Thread Loop
```
while simulation is running:
    pick up left fork  → lock left mutex
    pick up right fork → lock right mutex
    eat                → update last_meal_time, increment meal_count
    put down forks     → unlock both mutexes
    sleep
    think
```

### Monitor Thread
A dedicated **monitor thread** continuously checks:
1. Has any philosopher exceeded `time_to_die` since their last meal?
2. Have all philosophers eaten `n` times? (optional stopping condition)

When a death or completion is detected, a shared `simulation_over` flag is set, causing all threads to exit gracefully.

### Deadlock Prevention Strategy
Use **fork ordering**: even-numbered philosophers pick up their right fork first, odd-numbered pick up their left fork first. This breaks the circular wait condition.

Alternatively, add a small `usleep` delay for even philosophers before picking up their first fork — both approaches work.

### Timestamp & Logging
All messages must be printed with a precise **millisecond timestamp** relative to the simulation start:
```
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```
A **print mutex** ensures log lines are never interleaved.

---

## ⚠️ Common Mistakes & Tricky Points

- **Death detection timing**: The monitor must check frequently (~every 1ms) and print the death message **within 10ms** of the actual death — evaluators test this.
- **No print after death**: Use a mutex-protected `simulation_over` flag checked before every `printf`. Once set, nothing else should be logged.
- **Single philosopher edge case**: With 1 fork, the philosopher picks it up, waits forever for the second, and dies. Handle this without deadlocking.
- **Protect `last_meal_time`**: It's written by the philosopher thread and read by the monitor — always access it under a mutex to avoid data races.
- **`usleep` is imprecise**: It can overshoot on loaded systems. Use a loop that calls `usleep(500)` repeatedly and checks elapsed time yourself.
- **Even/odd staggering**: Without it, all philosophers act in lockstep. Add a small initial delay for even-numbered philosophers to desynchronize them.
- **Always destroy mutexes**: Clean up every initialized mutex, even on error paths.

---

## ✅ Evaluation Checklist

- [ ] No philosopher dies when they shouldn't
- [ ] A philosopher dies when they should (e.g. `4 310 200 100`)
- [ ] Single philosopher case handled correctly
- [ ] Death message printed within 10ms of actual death
- [ ] No data races (run with `valgrind --tool=helgrind` or `ThreadSanitizer`)
- [ ] No memory leaks
- [ ] Simulation stops correctly with optional 5th argument
- [ ] Logs never interleave or print after a death

---

## 🛠 Useful Debug Commands

```bash
# Check for data races
gcc -fsanitize=thread -g ... && ./philo 5 800 200 200

# Check memory
valgrind --leak-check=full ./philo 5 800 200 200

# Stress test death timing
./philo 4 310 200 100   # Should die
./philo 5 610 200 200   # Should not die
```
