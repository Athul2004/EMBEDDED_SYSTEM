## 🔹 ``malloc()`` (Memory Allocation)

- Allocates a single block of memory of given size (in bytes).
- The contents are uninitialized (garbage values).
- Faster than calloc since it doesn’t set memory to zero.

Syntax:
```c
ptr = (int*) malloc(n * sizeof(int));
```

## 🔹 ``calloc()`` (Contiguous Allocation)

- Allocates multiple blocks (array) of memory, each of the same size.
- The memory is initialized to 0.
- Slightly slower because it clears memory to zero.
```c
ptr = (int*) calloc(n, sizeof(int));
```

| Feature        | `malloc()`                        | `calloc()`                                          |
| -------------- | --------------------------------- | --------------------------------------------------- |
| Initialization | Garbage values (uninitialized)    | All values set to **0**                             |
| Parameters     | Takes **1** argument (total size) | Takes **2** arguments (no. of blocks, size of each) |
| Speed          | Faster (no initialization)        | Slightly slower (due to zeroing)                    |
| Use case       | When you’ll assign values later   | When you want a zero-initialized array              |

