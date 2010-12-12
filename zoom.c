/*
* Exclamation.  Copyright John Morrice 2010.
*
*
* This file is part of Exclamation.
*
* Exclamation is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Exclamation is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Exclamation.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "exclamation.c"

int
main(int argc, char ** argv)
{

   struct Exclamation parent;

   struct Exclamation *children;

   int handle, size, i, max;

   if (argc < 2)
   {
      fprintf(stderr, "USAGE: exclamation NUMBER_OF_FRAMES\n");
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
