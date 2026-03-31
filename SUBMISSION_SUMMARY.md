# Problem 119 - Linear Scan Register Allocation - Submission Summary

## Problem Description
Implemented a Linear Scan Register Allocation algorithm in C++. The algorithm allocates registers to live intervals using a FILO (First In Last Out) strategy for register allocation and handles register spilling when necessary.

## Solution Approach

### Key Components
1. **Location Classes**: Base class `Location` with derived classes `Register` and `StackSlot`
2. **LiveInterval Structure**: Represents variable lifetimes with start point, end point, and allocated location
3. **LinearScanRegisterAllocator**: Main algorithm implementation

### Algorithm Implementation
1. **Register Initialization**: Free registers stored in a stack (FILO), with register 0 at the top initially
2. **expireOldIntervals()**: Frees registers from expired intervals and pushes them back to the stack
3. **spillAtInterval()**: Handles register spilling when all registers are in use
4. **linearScanRegisterAllocate()**: Main loop processing intervals in order of increasing start points

### Critical Design Decisions
- Used `std::stack` for FILO register allocation
- Used `std::set` with custom comparator for maintaining active intervals sorted by endpoint
- Proper handling of the spilling logic to choose between spilling the new interval or an existing one

## Test Results

### Local Testing
Successfully tested with the provided sample:
- Input: 5 intervals with 2 available registers
- Output matched expected results:
  - interval 1: reg0
  - interval 2: reg1
  - interval 3: stack
  - interval 4: reg0
  - interval 5: reg1

### Online Judge Results
**Submission ID**: 767202
**Status**: ACCEPTED
**Score**: 100/100 (Perfect Score)

#### Detailed Results by Subtask:
| Subtask | Test Cases | Content | Score | Status |
|---------|-----------|---------|-------|--------|
| 1 | 1 | Location classes | 20/20 | ✓ Accepted |
| 2 | 2 | Single allocation, no spill | 10/10 | ✓ Accepted |
| 3 | 3 | Single allocation | 10/10 | ✓ Accepted |
| 4 | 4 | No spill cases | 20/20 | ✓ Accepted |
| 5 | 5-6 | General cases (regNum ≤ 32, size ≤ 1000) | 20/20 | ✓ Accepted |
| 6 | 7-8 | Stress tests (regNum ≤ 10³, size ≤ 10⁵) | 20/20 | ✓ Accepted |

### Performance Metrics
- Maximum memory usage: 8.3 MB (Test 8)
- Maximum time: 83 ms (Test 8)
- All tests completed well within time and memory limits

## Submission Count
**Total Submissions Used**: 1 out of 5 allowed
- First submission achieved perfect score

## Repository Information
- **GitHub Repository**: https://github.com/ojbench/oj-eval-claude-code-119-20260401044756
- **Main Implementation File**: `src.hpp`
- **Test File**: `test.cpp`

## Commits
1. `2653284` - Implement Linear Scan Register Allocation algorithm
2. `514f08a` - Add submit_code method to acmoj_client.py

## Conclusion
Successfully completed Problem 119 with a perfect score on the first attempt. The implementation correctly follows the Linear Scan algorithm specifications, including proper FILO register allocation and spilling strategies.
