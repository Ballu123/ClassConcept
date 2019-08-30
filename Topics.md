Rvalue Reference and Move Semantics:

Move semantics means the class will transfer ownership of the object rather than making a copy.
https://www.bogotobogo.com/cplusplus/C11/5_C11_Move_Semantics_Rvalue_Reference.php
https://mbevin.wordpress.com/2012/11/20/move-semantics/

Shared Pointer Implementation:

https://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c
https://www.acodersjourney.com/implementing-smart-pointer-using-reference-counting/

Weak Pointer:

A weak_ptr is created as a copy of shared_ptr. It provides access to an object that is owned by one or more shared_ptr instances,
but does not participate in reference counting. The existence or destruction of weak_ptr has no effect on the shared_ptr 
or its other copies. It is required in some cases to break circular references between shared_ptr instances.
https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2/



