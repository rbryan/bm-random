/*
 * config.h
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







//Leave this defined if you want to descriminate particles by radius
//rather than surface area.
#define USE_RADIUS_THRESH

#ifdef USE_RADIUS_THRESH
#define RAD_MAX				10
#define RAD_MIN				4
#endif

#ifndef USE_RADIUS_THRESH
#define MAX_PART_SIZE		200
#define MIN_PART_SIZE		50
#define SIZE_DIFF_LIMIT		10
#endif
