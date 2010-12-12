#include "exclamation.c"

int
main(int argc, char ** argv)
{
   // Space for the children
   struct Exclamation children[STEPCHILDREN];
   // The parent
   struct Exclamation parent;

   // The handle
   int handle;
 
   parent.height = 160;
   parent.width = 20;
   parent.bottom = -79;
   parent.left = -8;

   // Recurse one step
   fractal_step(&parent, children);

   handle = start_drawing();
  
   draw_children(STEPCHILDREN, children);

   stop_drawing(handle);

   return 0;
}
