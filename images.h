/*
 * images.h
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
 */

#include <stdint.h>


typedef struct
{
    int width, height;
    uint8_t **vals;
} matrix;


typedef struct
{
    int width, height;

    matrix *red;
    matrix *green;
    matrix *blue;
} image;

extern void freeMatrix(matrix *mat);
extern void freeImage(image *img);
extern matrix *newMatrix(int width, int height);
extern image *loadImage(const char *filename);
extern void saveImage(image *img, const char *filename);
extern matrix *avgGreyscale(image *img);
extern matrix *gradient(matrix *mat);
extern void threshold(matrix *mat, matrix *dest, uint8_t min, uint8_t max);
extern void fill_matrix(matrix * mat, uint8_t value);
