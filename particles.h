/*
 * particles.h
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

struct vec2d
{
  int x, y;
};

typedef struct vec2d vec2d;


struct pt
{
  int id;
  int x, y;
  struct pt *prevPt;
  struct pt *nextPt;
};

typedef struct pt pt;

#pragma pack(0)
struct particle
{
  int size, x, y;
  double radius;
  struct pt *spt, *ept;		//start point and end point
  int id;
  struct particle *nextFrame;
  struct particle *prevParticle;
  struct particle *nextParticle;
};
#pragma pack()
typedef struct particle particle;

struct partlist
{
  int size;
  struct particle *spart;
  struct particle *epart;

};

typedef struct partlist partlist;


extern void freePartlist (partlist * plist);
extern particle *newParticle (int id);
extern pt *newPt (int id, int x, int y);
extern pt *jump (pt * start, int n);
extern void pushPt (pt * point, particle * par);
extern pt *popPt (pt * point, particle * par);
extern particle *popParticle (partlist * plist, particle * part);
extern void freeParticle (particle * par);
extern void pushParticle (partlist * plist, particle * part);
