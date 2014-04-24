/*
 * tracking.c
 * 
 * Copyright 2013 Russell Bryan <russell@godel>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include "images.h"
#include "particles.h"
#include "ident.h"
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include "config.h"

//Remember to note that particles may overlap and several particles
//can potentially be linked to the same successor.

void link_parts(partlist * partsa, partlist * partsb)
{
    int i, j;
    double best = 0, distance = 0;
    particle *best_part, *next;
    particle *current, *check;
#ifndef USE_RADIUS_THRESH
    double size_diff;
    size_diff = SIZE_DIFF_LIMIT / 100.00;
#endif
    current = partsa->spart;
    for (i = 0; i < partsa->size; i++, current = next) {
	check = partsb->spart;
	best_part = NULL;
	best = DBL_MAX;
	for (j = 0; j < partsb->size; j++, check = check->nextParticle) {
	    distance = dist(current->x, current->y, check->x, check->y);
#ifndef	USE_RADIUS_THRESH
	    if (distance < best
		&& abs(current->size - check->size) / current->size <
		size_diff) {
#endif
#ifdef USE_RADIUS_THRESH
		if (distance < best && distance > current->radius * 0.05
		    && distance < current->radius * 1.1) {
#endif
		    best_part = check;
		    best = distance;
		} else {
		    continue;
		}

	    }
	    next = current->nextParticle;
	    if (best_part == NULL) {
		freeParticle(popParticle(partsa, current));
		i--;
		continue;
	    } else {
		current->nextFrame = best_part;
	    }


	}

    }
