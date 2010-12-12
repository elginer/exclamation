#include "exclamation.c"

int
main(int argc, char ** argv)
{

   struct Exclamation parent;

   struct Exclamation *children;

   int handle, size, i, max;

   if (argc < 2)
   {
      fprintf(stderr, "USAGE: exclamation POSITIVE_NUMBER\n");
      exit(EXIT_FAILURE);
   }

   max = atoi(argv[1]);

   parent.height = 20;
   parent.width = 2;
   parent.bottom = -10;
   parent.left = -1;

   handle = start_drawing();

   // Fractal zoom
   for (i = 0; i < max; i++)
   {

//      fprintf(stderr, "%dth iteration...\n", i);
//      fprintf(stderr, "parent stats: w: %f h: %f b: %f l: %f\n", parent.width, parent.height, parent.bottom, parent.left);
      // Get the children from the recursion
      children = fractal(&parent, &size);
      
//      fprintf(stderr, "made fractal...\n");
      new_frame();
      draw_children(size, children);

//      fprintf(stderr, "drawn %d children...\n", size);
      
      free(children);
//      fprintf(stderr, "freed the fractal of %d children at %p...\n", size, children);

      parent.height = parent.height * 1.1;
      parent.width = parent.width * 1.1;
      parent.bottom = parent.bottom * 1.1;
      parent.left = parent.left * 1.1;


   }

   stop_drawing(handle);

   return 0;

}
