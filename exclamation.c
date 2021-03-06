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

#include <stdio.h>
#include <stdlib.h>
#include <plot.h>

// The number of vertical slots for children in a fractal parent.
#define VSLOTS 8
// The number of horizontal slots for children in a fractal child.
#define HSLOTS 3
// Number of children used in a single step
#define STEPCHILDREN ((VSLOTS - 1) * HSLOTS)

// An exclamation mark.
struct Exclamation
{
	float height;
	float width;
	float bottom;
	float left;
};

// Can we recurse on this Exclamation?
// Returns 1 if possible, 0 otherwise.
int
can_recurse(struct Exclamation *candidate)
{
	struct Exclamation fract = *candidate;
	// fprintf(stderr, "can_recurse\n");
	return (fract.width > 1.0 || fract.height > 1.0);
}

// A horizontal step while recursing
void
step_width(struct Exclamation *children, 
		float bottom, float left, 
		float width, float height, float gap)
{
	// Iterative variables.
	float current_left;
	int i;
	struct Exclamation * child;


	//  fprintf(stderr, "step_width\n");

	// fprintf(stderr, "drawing width...\n");
	current_left = left;
	for (i = 0; i < HSLOTS; i++)
	{
		child = children + i;
		child->width = width;
		child->height = height;
		child->bottom = bottom;
		child->left = current_left;
		current_left += gap + width;
		// fprintf(stderr, "child at %fx%f\n", child->left, child->bottom);
	}
	// fprintf(stderr, "step_width complete...\n");
}

// Recurse one step
// The children array must contain STEPCHILDREN struct Exclamation
void
fractal_step(float width, float height, 
		float left, float bottom,
		float xGap, float ySlot,
		struct Exclamation *children)
{
	// Variables for iteration
	float current_bottom;
	int i;

	// Write the bottom part of the exclamation mark.
	step_width(children, bottom, left, width, height, xGap);

	// fprintf(stderr, "drawn bottom children\n");

	// Write the top part of the exclamation mark.
	current_bottom = bottom + (ySlot * 2); 

	for (i = 3; i < STEPCHILDREN; i += HSLOTS)
	{
		step_width(children + i, current_bottom, left, width, height, xGap);
		current_bottom += ySlot;
	}
}

// Write the new child parameters to the pointers
void
child_params(struct Exclamation *parent, 
		float *width, float *height,
		float *xGap, float *ySlot)
{
	struct Exclamation fract = *parent;

	// The value in the ySlot.
	float ySlotV;

	ySlotV = fract.height / VSLOTS;
	*ySlot = ySlotV; 

	// There are 2 gaps of 35% of the overall width
	*xGap = fract.width * 0.35;
	*width = fract.width * 0.1;

	// There is a gap of 0.1 of the height of the children between each child
	*height = ySlotV * 0.9;

}

// Allocate space
struct Exclamation *
xZone(size_t count)
{
	struct Exclamation * new_ptr;

	new_ptr = (struct Exclamation *) malloc(count * sizeof(struct Exclamation));

	//   fprintf(stderr, "Allocated space for %u Exclamations...\n", count);

	if (!new_ptr)
	{
		fprintf(stderr, "FATAL: Could not allocate space for %lu Exclamations :(\n", count);
		exit(EXIT_FAILURE);
	}
	return new_ptr;

}

// Reallocate space
struct Exclamation *
rezone(struct Exclamation * fractals, size_t next)
{
	struct Exclamation * new_ptr;

	new_ptr = (struct Exclamation *) realloc(fractals, next * sizeof(struct Exclamation));

	if (!new_ptr)
	{
		fprintf(stderr, "FATAL: Could not re-allocate space for %lu Exclamations :(\n", next);
		exit(EXIT_FAILURE);
	}

	return new_ptr;
}

// Is the exclamation visible?
int
is_visible(struct Exclamation * surprisep)
{

	// Surprise!
	struct Exclamation surprise = *surprisep;

	// Most right point, and heighest point
	float right, top;

	right = surprise.left + surprise.width;
	top = surprise.bottom + surprise.height;

	return surprise.left <= 250 && right >= -250 && surprise.bottom <= 250 && top >= -250;

}

// Take an array of exclamations, the size of the array, and a pointer
// Return a new array of exclamations, formed of those in the input array
// who are actually visible.
// Write the size of the new array to the pointer.
struct Exclamation *
filter_visible(struct Exclamation *unbounded, int size, int *next_size)
{
	// Assume all are visible
	struct Exclamation * visible;

	// How many visible exclamations have we found?
	int found;

	// An iterative variable
	int i;

	visible = xZone(size);


	// Find those exclamations that are visible
	for (i = 0, found = 0; i < size; i++)
	{
		if (is_visible(unbounded + i))
		{
			visible[found] = unbounded[i];
			found ++;
		}
	}

//	 fprintf(stderr, "of %d exclamations total\n", size);
// 	fprintf(stderr, "found %d visible exclamations\n", found);

	// If none were found we zoomed into empty space
	if (!found)
	{
		fprintf(stderr, "Stopped early after zooming into empty space.\n");
		exit(0);
	}

	// Resize the array
	visible = rezone(visible, found); 
	*next_size = found;
	return visible;
}

// Generate the fractal.  Write the resultant number of children to size.
struct Exclamation *
fractal(struct Exclamation *parent, int *size)
{
	// The parental generation
	struct Exclamation *parents;
	// The new generation
	struct Exclamation *children;

	// Number of parents and children
	int parentCount, childCount;
	// Iterative variables
	int i;
	struct Exclamation current_parent;

	// The parameters for the children
	float width, height, xGap, ySlot;

	//  fprintf(stderr, "fractal\n");

	// We start with one parent
	parentCount = 1;
	// Allocate memory for our one parent.
	parents = xZone(1);

	//   fprintf(stderr, "allocated space for one parent at %p\n", parents);

	*parents = *parent;

	// Create new generations until the parents are too small.
	while (can_recurse(parents))
	{
		childCount = parentCount * STEPCHILDREN;
		children = xZone(childCount);

		//      fprintf(stderr, "allocated %d children at %p\n", childCount, children);

		// Get the parameters for the children
		// Get the paramters for a fractal step
		child_params(parents,
				&width, &height, 
				&xGap, &ySlot);

		// Create all the children
		for (i = 0; i < parentCount; i ++)
		{
			current_parent = parents[i];

			fractal_step(width, height,
					current_parent.left, current_parent.bottom,
					xGap, ySlot,
					children + (i * STEPCHILDREN));
		}

		//      fprintf(stderr, "freeing %d parents at %p...\n", parentCount, parents);
		free(parents);
		//      fprintf(stderr, "parents freed.\n");
		parents = filter_visible(children, childCount, &parentCount);
	}

	// The last parents are the children.
	*size = parentCount;
	return parents;

}

// Draw a point at this position
void
draw_point(int x, int y)
{
	//   fprintf(stderr, "drawing box from %dx%d to %dx%d\n", x, y, x + 1, y + 1);
	pl_box(x, y, x + 1, y + 1);
}

// Draw a bunch of the fractal children
void
draw_children(int count, struct Exclamation * children)
{
	// The current child
	struct Exclamation child;

	// fprintf(stderr, "draw_children\n");

	// Iterative variable;
	int i;

	for (i = 0; i < count; i++)
	{
		child = children[i];
		draw_point(child.left, child.bottom);
	} 

}

// Start a new frame
void
new_frame()
{
	// fprintf(stderr, "new_frame\n");
	pl_erase();
}

// Start drawing the GIF.
int
start_drawing()
{
	int handle;

	pl_parampl("BITMAPSIZE", "500x500");
	pl_parampl("BG_COLOR", "white");
	pl_parampl("GIF_ITERATIONS", "65535");
	pl_parampl("GIF_DELAY", "3");

	handle = pl_newpl("gif", stdin, stdout, stderr);
	pl_selectpl(handle);
	pl_openpl();
	pl_space(-250, -250, 249, 249);
	pl_pencolorname("black");
	pl_linewidth(1);
	pl_fillcolorname("black");
	pl_filltype(1);
	return handle;
}

// Stop drawing
void
stop_drawing(int handle)
{
	pl_closepl();
	pl_selectpl(0);
	pl_deletepl(handle);
}
