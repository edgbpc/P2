to create executable:
make or make frontEnd

To run program:
frontEnd <testfile>
frontEnd < <testFile>.<fileExt>

To enter free form text entering mode
frontEnd

My program does print to screen a mapping of the nodes and the elements (if retained during parsing). I could see via the debugger that my trees are building as I intended but i have a hard time visualing what that looks like in the mapping so I am not entirely certain the printing is correct.  I can see all the various elments that I'd expect to see.

I found the most challenging part of this was knowing when to call a new token.  It was a bit tricky but in the end all the "Good Programs" pass and all the "Bad Programs" fail where they should fail.

I also tested against *, /, and % to ensure those functioned as well as ( <expr> ) since these cases were not included in the "Good Programs"

I am not entirely sure that i saved the proper elements so any feedback in this area for P3 would be appreciated.  Adding additional elements saved is relatively easy and perhaps I should have just saved them all.


