/*
 * particles.c
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
#include <stdlib.h>
#include <stdint.h>
#include "images.h"

struct vec2d {
    int x, y;
};

typedef struct vec2d vec2d;

struct pt {
    int id;
    int x, y;
    struct pt *prevPt;
    struct pt *nextPt;
};

typedef struct pt pt;

#pragma pack(0)
struct particle {
    int size, x, y;
    double radius;
    struct pt *spt, *ept;	//start point and end point
    int id;
    struct particle *nextFrame;
    struct particle *prevParticle;
    struct particle *nextParticle;
};
#pragma pack()
typedef struct particle particle;

struct partlist {
    int size;
    struct particle *spart;
    struct particle *epart;

};

typedef struct partlist partlist;

void pushParticle(partlist * plist, particle * part)
{
    if (plist->size > 0) {
	plist->epart->nextParticle = part;
	part->prevParticle = plist->epart;
	plist->epart = part;

    } else if (plist->size == 0) {
	plist->spart = part;
	plist->epart = part;

    }
    plist->size++;
}



particle *popParticle(partlist * plist, particle * part)
{
    if (plist->size == 1) {
	plist->spart = NULL;
	plist->epart = NULL;
	plist->size--;
	return part;
    } else if (plist->spart == part) {
	part->nextParticle->prevParticle = NULL;
	plist->spart = part->nextParticle;
	plist->size--;
	return part;
    } else if (plist->epart == part) {

	part->prevParticle->nextParticle = NULL;
	plist->epart = part->prevParticle;
	plist->size--;
	return part;
    } else {
	part->nextParticle->prevParticle = part->prevParticle;
	part->prevParticle->nextParticle = part->nextParticle;
	plist->size--;
	return part;
    }

}

particle *newParticle(int id)
{
    particle *newpart;

    if ((newpart = (particle *) calloc(1, sizeof(particle))) == NULL) {
	fprintf(stderr, "proc.c: newParticle(): Out of memory error.\n");
	exit(1);
    }

    newpart->id = id;
    return newpart;
}

pt *newPt(int id, int x, int y)
{
    pt *newpt;

    if ((newpt = (pt *) calloc(1, sizeof(pt))) == NULL) {
	fprintf(stderr, "proc.c: newPt(): Out of memory error.\n");
	exit(1);
    }
    newpt->id = id;
    newpt->x = x;
    newpt->y = y;
    return newpt;
}

//As of yet this function remains unused...

pt *jump(pt * start, int n)
{
    pt *current;
    int i;

    current = start;

    if (n > 0) {
	for (i = 0; i < n; i++) {
	    if ((current = (pt *) current->nextPt) == NULL) {
		fprintf(stderr,
			"proc.c: jump(): Pointer out of bounds: pt Jump: start address: %lud distance: %d\n",
			(uint64_t) start, n);
		return NULL;
	    }
	}

    } else if (n < 0) {
	for (i = 0; i > n; i--) {
	    if ((current = (pt *) current->prevPt) == NULL) {
		fprintf(stderr,
			"proc.c: jump(): Pointer out of bounds: pt Jump: start address: %lud distance: %d\n",
			(uint64_t) start, n);
		return NULL;
	    }
	}

    } else {
	return start;
    }

    return current;
}



void pushPt(pt * point, struct particle *par)
{
    par->ept->nextPt = point;
    point->prevPt = par->ept;
    par->ept = point;
}



pt *popPt(pt * point, particle * par)
{
    if (par->spt == par->ept && par->size == 1) {
	par->spt = NULL;
	par->ept = NULL;
	par->size--;
	return point;
    } else if (par->ept == point) {
	par->ept = point->prevPt;
	par->size--;
	return point;
    } else {
	if (!(point->prevPt == NULL)) {
	    point->prevPt->nextPt = point->nextPt;
	} else {
	    par->spt = point->nextPt;
	}
	if (!(point->nextPt == NULL)) {
	    point->prevPt->nextPt = point->nextPt;
	} else {
	    par->ept = point->prevPt;
	}
	par->size--;
	return point;
    }
}




void freeParticle(particle * par)
{
    if (!(par->ept == NULL && par->spt == NULL)) {
	while (par->size > 0) {

	    free(popPt(par->ept, par));

	}
    }
    free(par);
}

void freePartlist(partlist * plist)
{
    int i;
    particle *part, *next;
    part = plist->spart;
    for (i = 0; i < plist->size; i++, part = next) {
	next = part->nextParticle;
	freeParticle(part);
    }
    free(plist);
}
