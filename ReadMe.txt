
In this implementation, a dictionary entry looks like this:

int offsetToPreviousWord;
int flags; (bit 1 = IMMEDIATE)
int nameLength;
int name[];
int backPointerToWordStart;
int definition[];

Notes:
- The CORE words are at the TOP of the memory space
- The user dictionary starts at 1000
- The code in the user dictionary is more like machine code, in that an I_CALL
-    instruction is used to execute the definition the user words. That is to make
-    it much easier to disassemble the system.

I am considering this ...
- the word addresses are offsets and can be positive or negative
- the purpose here is to make the dictionary easily relocatable
