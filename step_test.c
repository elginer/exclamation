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

   // The parameters for the children
   float width, height, xGap, ySlot;
 
   parent.height = 160;
   parent.width = 20;
   parent.bottom = -79;
   parent.left = -8;

   // Get the paramters for a fractal step
   child_params(&parent,
         &width, &height,
         &xGap, &ySlot);

   // Recurse one step
   fractal_step(width, height, 
         parent.left, parent.bottom, 
         xGap, ySlot,
         children);

   handle = start_drawing();
  
   draw_children(STEPCHILDREN, children);

   stop_drawing(handle);

   return 0;
}
