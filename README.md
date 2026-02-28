# Serialization of linked list with random references
This program is aimed to perform linked list construction from file data, and serialize/deserialize it.

## Task
```cpp
struct ListNode { // ListNode модифицировать нельзя
    ListNode* prev = nullptr; // указатель на предыдущий элемент или nullptr
    ListNode* next = nullptr;
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr` 
    std::string data; // произвольные пользовательские данные 
};
```
Implement serialization/deserialization of linked list of `ListNode`-s, where:
- `prev` and `next` point to the previous/next element of the list;
- `rand` points to an arbitrary element of the same list or nullptr;
- Each element contains the string `data`.

It is necessary to:
1. Read the text description of the list from the file inlet.in;
2. Build a linked list `ListNode*` head from this data;
3. Serialize the list into binary file outlet.out.

## Input file format
```cpp
<data>;<rand_index>
```
Where:
- `<data>` is a string (may contain spaces, special characters, and UTF-8 encoding);
- `<rand_index>` is the index of the node pointed to by `rand`, or -1 if `rand` == nullptr;

Example:
```cpp
apple;2
banana;-1
carrot;1
```

## Output file
Binary file containing a serialized representation of linked list.

## Limitations
- Maximum number of nodes: $10^6$;
- Data can be up to $1000$ characters long.