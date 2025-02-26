# memory_mapping-c-
**What is Memory Mapping?**
Memory mapping is a process used in computers to assign memory addresses to different hardware devices, files, or data in RAM. It allows the CPU to access data quickly without worrying about the actual physical location of the data.
**How Does File Memory Mapping Work?**
The process of file memory mapping involves the following steps:
**Open the File**: The operating system opens the file and creates a link between the file and memory.
**Create a Memory Map:** A portion of the virtual memory is reserved, and the file is mapped to this area.
**Load Data on Demand:** Instead of loading the whole file, only the required parts are brought into RAM when accessed.
**Read|Write Directly**:The program can now access the file as if it were an array in memory. Any modifications made are reflected in the file.
**Close the File**: Once done, the memory mapping is closed, and the changes (if any) are written back to the file.
