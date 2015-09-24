
In this implementation, a dictionary entry looks like this:

int offsetToNextWord; // name length + 4
int flags; // bit 1 = IMMEDIATE
int xt; // address of the start the of code
int nameLength;
int name[];

To-do:
- Separate the dictionary entries from the code.

Notes:
- The dictionary words are at the TOP of the memory space
- They grow down towards the code space
- The code space starts at 1000
- Memory below 1000 is reserved by the system
- The code is machine code, in that an I_CALL
-    instruction is used to execute the definition the words. That is to make
-    it much easier to disassemble the system.
