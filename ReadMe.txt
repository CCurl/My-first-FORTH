
In this implementation, a dictionary entry looks like this:

int entrySize; // name length + 3
int flags; // 1 = IMMEDIATE
int xt; // address of the start the of code
int nameLength;
int name[nameLength];

Notes:
- The dictionary words are at the TOP of the memory space.
- It grows down toward the code space.
- The code starts at 1000, and grows toward the dictionary.
- Memory below 1000 is reserved by the system
- The code is machine code, in that an I_CALL
-    instruction is used to execute the definition a word. That 
-    improves performance and dakes it easier to disassemble the code.
- The code can exist without the dictionary. In that world, words 
-    that interact with the dictionary will (naturally) not work.
