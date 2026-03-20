# DSA — Sort an Array of 0s, 1s, and 2s (Dutch National Flag Algorithm)

A focused C++ implementation of **Dijkstra's Dutch National Flag algorithm** — one of the most elegant and widely cited three-pointer algorithms in computer science. Given an array containing only `0`s, `1`s, and `2`s, it sorts the array in a **single pass** using **O(1) extra space**. This is a canonical problem in technical interviews (LeetCode #75) and a masterclass in how carefully chosen pointer invariants can eliminate multiple passes over data.

---

## Problem Statement

Given an array containing only the values `0`, `1`, and `2`, sort it in-place so all `0`s come first, followed by all `1`s, then all `2`s.

**Example Input:**
```
Enter number of elements: 9
Enter elements (0, 1 or 2 only): 2 0 1 2 1 0 0 1 2
```

**Example Output:**
```
Sorted: 0 0 0 1 1 1 2 2 2
```

---

## The Code

```cpp
#include<bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int nums[300];
    cout << "Enter elements (0, 1 or 2 only): ";
    for (int i = 0; i < n; i++) cin >> nums[i];

    int low = 0, mid = 0, high = n - 1;

    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low], nums[mid]);
            low++; mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high]);
            high--;
        }
    }

    cout << "Sorted: ";
    for (int i = 0; i < n; i++) cout << nums[i] << " ";
    cout << endl;

    return 0;
}
```

---

## Why "Dutch National Flag"?

Edsger W. Dijkstra named this problem after the **Dutch national flag** — three horizontal bands of red, white, and blue. The problem maps directly: partition an array of three distinct values into three contiguous groups, just as the flag has three contiguous color bands. Dijkstra formalized the algorithm and its invariants in his 1976 monograph *A Discipline of Programming* — it remains one of the cleanest examples of invariant-driven algorithm design.

```
Dutch Flag:          Array analogy:
┌─────────┐          ┌───────────────────────────┐
│  Red    │          │  0  0  0  0               │
├─────────┤    →     ├───────────────────────────┤
│  White  │          │  1  1  1  1  1            │
├─────────┤          ├───────────────────────────┤
│  Blue   │          │  2  2  2                  │
└─────────┘          └───────────────────────────┘
```

---

## The Three Pointers — Roles and Invariants

The algorithm maintains three pointers at all times:

| Pointer | Role | Invariant maintained |
|---------|------|----------------------|
| `low` | Left boundary — next position for a `0` | `nums[0..low-1]` are all `0` |
| `mid` | Current element under examination | `nums[low..mid-1]` are all `1` |
| `high` | Right boundary — next position for a `2` | `nums[high+1..n-1]` are all `2` |

The **unsorted region** at any point is `nums[mid..high]`. The algorithm terminates when `mid > high` — the unsorted region is empty.

---

## The Three Cases — Step by Step

### Case 1: `nums[mid] == 0`
```cpp
swap(nums[low], nums[mid]);
low++; mid++;
```
`0` belongs at the left. Swap it with `nums[low]` (the next available `0` slot). Advance both `low` and `mid` — the swapped-in element at `mid` is known to be `1` (it came from the `[low..mid-1]` region which is all `1`s), so `mid` can safely advance.

### Case 2: `nums[mid] == 1`
```cpp
mid++;
```
`1` belongs in the middle — it is already in the right region. Simply advance `mid`.

### Case 3: `nums[mid] == 2`
```cpp
swap(nums[mid], nums[high]);
high--;
```
`2` belongs at the right. Swap it with `nums[high]` (the next available `2` slot). Advance `high` inward. **`mid` is NOT incremented** — the swapped-in element from `nums[high]` is unknown and must be examined next.

---

## The Critical Asymmetry — Why `mid` Does Not Advance After Swapping a `2`

This is the most important detail in the entire algorithm. The handling of `0` and `2` look symmetric — both swap — but their pointer updates are not:

```cpp
// Case 0: swap with low
swap(nums[low], nums[mid]);
low++; mid++;      ← BOTH advance

// Case 2: swap with high
swap(nums[mid], nums[high]);
high--;            ← only high advances, mid STAYS
```

**Why?**

- When swapping a `0` with `nums[low]`: `nums[low]` came from the `[low..mid-1]` region, which is **all `1`s** by invariant. So after the swap, `nums[mid]` is a `1` — already sorted. `mid` can safely advance.

- When swapping a `2` with `nums[high]`: `nums[high]` came from the **unsorted `[mid..high]` region**. Its value is completely unknown — it could be `0`, `1`, or `2`. `mid` must **stay in place** to examine the newly swapped-in element on the next iteration.

Advancing `mid` after swapping a `2` would skip an unexamined element and produce an incorrect sort. This asymmetry is the invariant-preservation mechanism that makes the algorithm correct.

---

## Step-by-Step Trace

**Input:** `nums = {2, 0, 1, 2, 1, 0}`, `n = 6`

**Initial state:** `low=0, mid=0, high=5`

```
Index:  0  1  2  3  4  5
Array: [2, 0, 1, 2, 1, 0]
        ↑              ↑
       low,mid        high
```

| Step | `nums[mid]` | Action | `low` | `mid` | `high` | Array |
|------|-------------|--------|-------|-------|--------|-------|
| 1    | 2 | swap(mid,high), high-- | 0 | 0 | 4 | `[0, 0, 1, 2, 1, 2]` |
| 2    | 0 | swap(low,mid), low++, mid++ | 1 | 1 | 4 | `[0, 0, 1, 2, 1, 2]` |
| 3    | 0 | swap(low,mid), low++, mid++ | 2 | 2 | 4 | `[0, 0, 1, 2, 1, 2]` |
| 4    | 1 | mid++ | 2 | 3 | 4 | `[0, 0, 1, 2, 1, 2]` |
| 5    | 2 | swap(mid,high), high-- | 2 | 3 | 3 | `[0, 0, 1, 1, 2, 2]` |
| 6    | 1 | mid++ | 2 | 4 | 3 | `[0, 0, 1, 1, 2, 2]` |
| Exit | mid(4) > high(3) → loop ends | | | | | |

**Output:** `Sorted: 0 0 1 1 2 2` ✅

---

## Complexity Analysis

| Metric | Complexity |
|--------|------------|
| Time   | **O(n)** — single pass; every element examined at most once |
| Space  | **O(1)** — three integer pointers; no auxiliary array |

This is **optimal** — any correct sort of this array must examine every element at least once, making O(n) the lower bound. The Dutch National Flag algorithm achieves this lower bound in a single pass.

---

## The Fixed Buffer — `int nums[300]`

Unlike most programs in this series which use a VLA (`int arr[n]`), this code uses a fixed-size buffer:

```cpp
int nums[300];
```

This pre-allocates 300 integers on the stack regardless of the actual input size `n`. It is not a VLA — it compiles on any standard-compliant C++ compiler including MSVC.

**Implications:**
- If `n > 300`, the input loop writes beyond the allocated buffer — **buffer overflow**, undefined behavior ⚠️
- Memory for 300 integers is always allocated even if `n = 3`
- For portable, safe code: `std::vector<int> nums(n)` allocates exactly `n` elements

---

## The Input Constraint — No Validation

The prompt says `"Enter elements (0, 1 or 2 only)"` but the code performs no validation:

```cpp
for (int i = 0; i < n; i++) cin >> nums[i];
// no check that nums[i] is 0, 1, or 2
```

If a value other than `0`, `1`, or `2` is entered (e.g., `5`), it falls into the `else` branch (treated as `2`) and is swapped to the high end — producing a silently wrong sorted output with the invalid value present.

**Fix:**
```cpp
for (int i = 0; i < n; i++) {
    cin >> nums[i];
    if (nums[i] < 0 || nums[i] > 2) {
        cout << "Invalid input. Only 0, 1, 2 allowed." << endl;
        return 1;
    }
}
```

---

## Why Not Just Use `std::sort`?

`std::sort` would work in O(n log n). The Dutch National Flag algorithm beats it — O(n), single pass — by exploiting the domain constraint: **only three distinct values exist**. General-purpose sorting algorithms don't exploit this; this algorithm does.

Similarly, **counting sort** would work in O(n) by counting occurrences of `0`, `1`, `2` and reconstructing:

```cpp
int c0=0, c1=0, c2=0;
for(int i=0;i<n;i++) {
    if(nums[i]==0) c0++;
    else if(nums[i]==1) c1++;
    else c2++;
}
// refill array: c0 zeros, c1 ones, c2 twos
```

| Approach | Time | Space | Passes | In-place? |
|----------|------|-------|--------|-----------|
| `std::sort` | O(n log n) | O(1) | Multiple | ✅ |
| Counting sort | O(n) | O(1) | **2** | ✅ |
| **Dutch National Flag (this repo)** | **O(n)** | **O(1)** | **1** | **✅** |

The Dutch National Flag is the only approach that achieves O(n) time, O(1) space, and a **single pass** simultaneously.

---

## Edge Cases

| Scenario | Behavior |
|----------|----------|
| All `0`s `{0,0,0}` | `mid` advances, `low` advances — all swaps are self-swaps ✅ |
| All `1`s `{1,1,1}` | Only `mid++` triggers — array untouched ✅ |
| All `2`s `{2,2,2}` | `high` shrinks to meet `mid` — no movement of elements needed ✅ |
| Single element | One iteration — trivially sorted ✅ |
| Already sorted `{0,1,2}` | Each pointer advances once — O(1) work ✅ |
| Reverse sorted `{2,1,0}` | Correctly sorted in one pass ✅ |
| `n = 0` | `mid=0 > high=-1` immediately — loop skipped ✅ |
| `n > 300` | ⚠️ Buffer overflow — undefined behavior |
| Value outside {0,1,2} | ⚠️ Treated as `2` — silently wrong output |

---

## Repository Structure

```
DSA-Dutch-National-Flag/
│
├── dutch_flag.cpp      # Main C++ implementation
└── README.md           # Project documentation
```

---

## How to Compile and Run

**Prerequisites:** GCC / G++

```bash
# Clone the repository
git clone https://github.com/rishita-ops/DSA-Dutch-National-Flag.git
cd DSA-Dutch-National-Flag

# Compile
g++ dutch_flag.cpp -o dutch_flag

# Run
./dutch_flag
```

**On Windows:**
```bash
g++ dutch_flag.cpp -o dutch_flag.exe
dutch_flag.exe
```

---

## Key Concepts Covered

- **Dutch National Flag algorithm** — Dijkstra's three-pointer invariant-based sort
- **Pointer invariants** — each pointer maintains a strict guarantee about the region it bounds
- **The `mid`-asymmetry** — why `mid` advances for `0` but not for `2` — the core correctness mechanism
- **Single-pass O(n) sort** — achieving optimal time and space simultaneously
- **Fixed-size buffer** — `int nums[300]` vs VLA vs `std::vector` — portability and overflow implications
- **Domain-specific optimization** — exploiting "only 3 values" to beat general-purpose O(n log n) sort
- **Counting sort comparison** — two passes vs one pass; both O(n), different trade-offs
- **`bits/stdc++.h`** — only `<iostream>` and `<algorithm>` (for `swap`) needed here

---

## Why This Problem Matters in DSA

| Problem / Concept | Connection |
|-------------------|------------|
| **LeetCode #75** (Sort Colors) | This exact problem — canonical reference implementation |
| **LeetCode #283** (Move Zeroes) | Two-pointer partition — simplified version of the DNF left partition |
| **LeetCode #905** (Sort Array by Parity) | Two-pointer partition of even/odd — same two-region DNF idea |
| **QuickSort partition step** | The Lomuto and Hoare partition schemes are direct relatives of this three-pointer approach |
| **Three-way QuickSort** | DNF is the partition step of three-way QuickSort — handles duplicate pivots efficiently |
| **LeetCode #148** (Sort List) | Sorting with restricted values in a linked list — same domain-constraint thinking |
| **Invariant-based algorithm design** | The DNF algorithm is the textbook example of writing correct algorithms by maintaining loop invariants |

The Dutch National Flag algorithm is not just a sorting trick — it is the partition subroutine of **three-way QuickSort**, which handles arrays with many duplicate elements far more efficiently than standard QuickSort. Understanding the invariants here makes the QuickSort partition step intuitive rather than mysterious.

---

## Contributing

Contributions are welcome. Consider adding:
- A **counting sort version** for direct comparison (2 passes, same O(n) time)
- **Input validation** — reject values outside {0, 1, 2}
- A **`std::vector` version** replacing the fixed buffer for safety
- A version extended to **four or more distinct values**
- Implementations in Python, Java, or JavaScript

```bash
git checkout -b feature/your-feature
git commit -m "Add: your feature description"
git push origin feature/your-feature
# Then open a Pull Request
```

---

## License

This project is open-source and available under the [MIT License](LICENSE).

---

*Part of a structured DSA practice series — fundamentals, done right.*
