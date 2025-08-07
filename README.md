# Wu-Manber Skip List Implementation

A C++ implementation of the Wu-Manber algorithm using skip lists for efficient multi-pattern string matching.

## Features

- **Skip List Based Storage**: Replaces traditional hash tables with skip lists for O(log n) search/insert complexity
- **Prefix Hashing**: Uses optimized hash function for pattern prefixes
- **Dynamic Optimization**: Auto-adjusts skip list parameters based on pattern count
- **Thread Safety**: Designed for concurrent access (though not fully thread-safe in current implementation)

## Class Structure

### `Node<V>`
- Template class representing skip list nodes
- Stores:
  - `hash_key`: Precomputed hash of pattern prefix
  - `value`: The full pattern string
  - `node_level`: Node's height in skip list
- Maintains forward pointers for O(log n) traversal

### `WuManberSkipList`
Core class implementing the Wu-Manber algorithm with skip lists.

#### Key Methods:
- `insert_pattern()`: Adds new patterns to the skip list
- `search_exact_match()`: Checks for exact pattern matches
- `optimize_for_wumanber()`: Dynamically adjusts skip list parameters

## Performance Characteristics

| Operation       | Complexity | Notes                          |
|-----------------|------------|--------------------------------|
| Pattern Insert  | O(log n)   | Includes hash computation      |
| Exact Match     | O(log n)   | Worst case when hash collides  |
| Memory Usage    | O(n)       | Slightly higher than hash table|

## Configuration Options

- `max_level`: Controls maximum skip list height (default 16)
- `prefix_length`: Wu-Manber prefix length (default 3)
- `probability`: Skip list growth probability (auto-adjusted)

## Dependencies

- C++11 or later
- Standard Library headers:
  - `<vector>`, `<string>`, `<functional>`
  - `<cstdlib>`, `<iostream>`, `<cmath>`, `<cstring>`

## Build Instructions
```bash
g++ -std=c++11 your_program.cpp -o wumanber
```

## Limitations

1. Currently stores full pattern strings (memory intensive for long patterns)
2. Not thread-safe in current implementation
3. Fixed prefix length (configure at initialization)

## Future Improvements

- [ ] Add support for pattern deletion
- [ ] Implement thread-safe version
- [ ] Add batch insert operations
- [ ] Optimize memory usage for long patterns