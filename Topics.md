Rvalue Reference and Move Semantics:
https://www.bogotobogo.com/cplusplus/C11/5_C11_Move_Semantics_Rvalue_Reference.php

Weak Pointer:
https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2/

A weak_ptr is created as a copy of shared_ptr. It provides access to an object that is owned by one or more shared_ptr instances,
but does not participate in reference counting. The existence or destruction of weak_ptr has no effect on the shared_ptr 
or its other copies. It is required in some cases to break circular references between shared_ptr instances.
