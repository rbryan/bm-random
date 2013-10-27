/*
 * images.c
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
#include <unistd.h>
#include <stdint.h>
#include <libgen.h>
#include <Imlib2.h>
#include "images.h"


//Written by Russell Bryan//////////////////////////////////////////////

void freeMatrix(matrix *mat)
{
    int i;
    for(i=0; i<mat->width; i++)
    {

		free(mat->vals[i]);
		
    }
	free(mat->vals);
    free(mat);
}

void freeImage(image *img)
{
    freeMatrix(img->red);
    freeMatrix(img->green);
    freeMatrix(img->blue);
    free(img);
}
////////////////////////////////////////////////////////////////////////

matrix *newMatrix(int width, int height)
{
    int i;
    matrix *mat = (matrix *)calloc(1, sizeof(matrix));


    mat->vals = (uint8_t **)calloc(width, sizeof(uint8_t *));
    mat->width = width;
    mat->height = height;


    for(i = 0; i < width; i++)
    {
        mat->vals[i] = (uint8_t*)calloc(height, sizeof(uint8_t));
    }
    return mat;
}


image *loadImage(const char *filename)
{
    Imlib_Image img;
    image *result;
    int i, j;
    int width, height;


    if((result = (image*)calloc(1, sizeof(image)))==NULL)
    {
        fprintf(stderr,"Herpaderp, out of memory");
        exit(1);
    }



    if((img = imlib_load_image(filename))==NULL){
		return NULL;
	}
	


    imlib_context_set_image(img);


    width = imlib_image_get_width();
    height = imlib_image_get_height();


    result->width = width;
    result->height = height;


    result->red = newMatrix(width, height);
    result->green = newMatrix(width, height);
    result->blue = newMatrix(width, height);


    for(i = 0; i < width; i++)
    {
        for(j = 0; j < height; j++)
        {
            Imlib_Color col;
            imlib_image_query_pixel(i, j, &col);


            result->red->vals[i][j] = col.red;
            result->green->vals[i][j] = col.green;
            result->blue->vals[i][j] = col.blue;
        }
    }

	imlib_free_image();
    return result;
}


void saveImage(image *img, const char *filename)
{
    Imlib_Image out;
    int i, j;


    out = imlib_create_image(img->width, img->height);
    imlib_context_set_image(out);


    for(i = 0; i < img->width; i++)
    {
        for(j = 0; j < img->height; j++)
        {
            int red = (int)img->red->vals[i][j];
            int green = (int)img->green->vals[i][j];
            int blue = (int)img->blue->vals[i][j];


            red = red <= 255 ? red : 255;
            green = green <= 255 ? green : 255;
            blue = blue <= 255 ? blue : 255;


            imlib_context_set_color(red, green, blue, 255);
            imlib_image_draw_line(i, j, i, j, 0);
        }
    }


    imlib_save_image(filename);
    imlib_free_image();
}


/* 
 * matrix *avgGreyscale(image *img)
 * 
 * This function takes and image and returns a matrix that contains
 * an unweighted average of the rgb values of each pixel.
 * 
 */

matrix *avgGreyscale(image *img){
	int i,j;
	matrix *mat;
	int r,g,b;
	int width,height;
	
	width = img->width;
	height = img->height;
	
	mat = newMatrix(width,height);
	
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			r = (int)img->red->vals[i][j];
			g = (int)img->green->vals[i][j];
			b = (int)img->blue->vals[i][j];
			
			mat->vals[i][j] = (r+g+b)/3;
		}
	}
	
	return mat;
	
}

/*
 * matrix *gradient(matrix *mat)
 * 
 * This function takes in a pointer to a matrix and returns two matrices.
 * The first contains the gradient matrix and the second contains
 * the directions of each gradient vector in the first.
 *  
 */
 
matrix *gradient(matrix *mat){
	matrix *grad;
	int *pix,current=0;
	int i,j,k,current_dir=0;
	int width,height;
	


	width = mat->width;
	height = mat->height;

	grad = newMatrix(width,height);
	
	
	if((pix = calloc(8,sizeof(int))) == NULL){
		fprintf(stderr, "Out of memory error.\n");
		exit(1);
	}
	
	/* 
	 * I am removing the edge pixels of the picture so that we don't go out 
	 * of bounds. It's faster and they aren't needed anyway. I plan on 
	 * throwing away particles that are that close to the edge.
	 */

	for(i=1; i<width-1; i++){
		for(j=1; j<height-1; j++){
			
			pix[0] = (int)mat->vals[i-1][j-1];
			pix[1] = (int)mat->vals[i][j-1];
			pix[2] = (int)mat->vals[i+1][j-1];
			pix[3] = (int)mat->vals[i-1][j];
			pix[4] = (int)mat->vals[i+1][j];
			pix[5] = (int)mat->vals[i-1][j+1];
			pix[6] = (int)mat->vals[i][j+1];
			pix[7] = (int)mat->vals[i+1][j+1];
			
			current = 0;
			//This is used to find the direction of maximum increase.
			for(k=0; k<8; k++){
				if(abs(pix[k]) > current){
					current = pix[k];
					current_dir = k;
				}
			}
	
			/*Try this with and without the assumption that diagonal
			 * pixels are sqrt(2) away instead of 1.*/
			grad->vals[i][j] = (abs(pix[current_dir]-((int)mat->vals[i][j])));
			
			
			
		}
		
	}
	free(pix);
	freeMatrix(mat);
	return grad;
}

void threshold(matrix *mat, matrix *dest, uint8_t min, uint8_t max){
	int i,j;
	
	for(i=0; i<mat->width; i++){
		for(j=0; j<mat->height; j++){
			if((mat->vals[i][j] > min) && (mat->vals[i][j] < max)){
				dest->vals[i][j] = 255;
			}else{
				dest->vals[i][j] = 0;
			}
		}
	}	
}

void fill_matrix(matrix * mat, uint8_t value){
	int i,j;
	
	for(i=0; i<mat->width; i++){
		for(j=0; j<mat->height; j++){
			mat->vals[i][j] = value;
		}
	}
	
}

