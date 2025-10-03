#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct{
	int r, g, b, val;
} pixel;

pixel **alloc_image(int h, int w);
void free_image(int h, pixel **image);
void load_file(char name[], FILE *file, int *ok);
void skip_comm(FILE *file);
void values(FILE *file, char type[], int *w, int *h, int *max);
void load_image(FILE *file, char type[], int h, int w, pixel **image);
void load_image_rgb(FILE *file, char type[], int h, int w, pixel **image);
void select_all(int h, int w, int *x1, int *y1, int *x2, int *y2, int ok);
void sel_val(int h, int w, int *x1, int *y1, int *x2, int *y2, int cy1, int ok);
void rotate(int angle, int *h, int *w, int x1, int y1, int *x2, int *y2,
			int ok, pixel ***ptr);
void equalize(int h, int w, int ok, char type[], pixel **image);
pixel **crop(int *h, int *w, int *x1, int *y1, int *x2, int *y2, pixel **image);
void apply(int h, int w, int x1, int y1, int x2, int y2, char type[2],
		   pixel **image, int ok);
void histogram(int h, int w, int ok, char type[], pixel **image);
void check_ascii(char ch, char aux[], int *ascii);
void save(char new_file[101], pixel **image, int h, int w, int max,
		  char type[], int ascii, int ok);

#endif
