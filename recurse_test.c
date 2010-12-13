#include "exclamation.c"

int
main(int argc, char ** argv)
{

   struct Exclamation parent;

   struct Exclamation *children;

   int handle, size;

   parent.height = 200;
   parent.width = 20;
   parent.bottom = -100;
   parent.left = -10;

   // Get the children from the recursion
   children = fractal(&parent, &size);

   handle = start_drawing();

   draw_children(size, children);

   free(children);

   stop_drawing(handle);

   return 0;

}
