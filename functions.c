#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
	int r, g, b, val;
} pixel;

void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int is_number(char str[])
{
	int i = 0;
	while (str[i]) {
		if (str[i] < '0' || str[i] > '9')
			return 0;
		str++;
	}
	return 1;
}

pixel **alloc_image(int h, int w)
{
	pixel **a;
	a = (pixel **)malloc(h * sizeof(pixel *));
	for (int i = 0; i < h; i++)
		a[i] = (pixel *)malloc(w * sizeof(pixel));
	return a;
}

void free_image(int h, pixel **image)
{
	for (int i = 0; i < h; i++)
		free(image[i]);
	free(image);
}

void load_file(char name[], FILE *file, int *ok)
{
	if (!file) {
		printf("Failed to load %s\n", name);
		*ok = 0;
		return;
	}
	printf("Loaded %s\n", name);
	*ok = 1;
}

void skip_comm(FILE *file)
{
	char first;
	while ((first = fgetc(file)) != EOF) {
		if (first == '\n')
			first = fgetc(file);
		if (first == '#') {//e linie comentariu deci trebuie ignorata
			while ((first = fgetc(file)) != EOF && first != '\n')
				continue;
		} else {
			fseek(file, -1, SEEK_CUR);//ca sa citim putem reciti caract ca nr
			break;
		}
	}
}

void values(FILE *file, char type[], int *w, int *h, int *max)
{
	skip_comm(file);
	fscanf(file, "%d%d", w, h);
	if (type[1] != '1' && type[1] != '4') {
		skip_comm(file);
		fscanf(file, "%d", max);
	}
	skip_comm(file);
}

void load_image(FILE *file, char type[], int h, int w, pixel **image)
{
	if (type[1] == '1' || type[1] == '2') {//fisier text
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++)
				fscanf(file, "%d", &image[i][j].val);
		}
	} else {//fisier binar
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				unsigned char val;
				fread(&val, sizeof(unsigned char), 1, file);
				image[i][j].val = (int)val;
			}
		}
	}
}

void load_image_rgb(FILE *file, char type[], int h, int w, pixel **image)
{
	if (type[1] == '3') {//fisier text
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				fscanf(file, "%d", &image[i][j].r);
				fscanf(file, "%d", &image[i][j].g);
				fscanf(file, "%d", &image[i][j].b);
			}
		}
	} else {//fisier binar
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				unsigned char r, g, b;
				fread(&r, sizeof(unsigned char), 1, file);
				fread(&g, sizeof(unsigned char), 1, file);
				fread(&b, sizeof(unsigned char), 1, file);
				image[i][j].r = (int)r;
				image[i][j].g = (int)g;
				image[i][j].b = (int)b;
			}
		}
	}
}

void select_all(int h, int w, int *x1, int *y1, int *x2, int *y2, int ok)
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	*x1 = 0;
	*y1 = 0;
	*x2 = h;
	*y2 = w;
	printf("Selected ALL\n");
}

void no_coord_errors(int ok)
{
	if (ok == 0)
		printf("No image loaded\n");
	else
		printf("Invalid command\n");
}

void sel_val(int h, int w, int *x1, int *y1, int *x2, int *y2, int cy1, int ok)
{
	int cx1 = 1, cx2 = 1, cy2 = 1;
	char coord[25], aux[25], ch;
	ch = getchar();
	if (ch == '\n') { //nu mai exista coordonata a 2a
		no_coord_errors(ok);
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", coord);
	if (coord[0] == '-') {
		cx1 = -1; //numarul are semnul minus
		strcpy(aux, coord + 1);
		strcpy(coord, aux); //scapam de caracterul minus de la inceput
	}
	if (is_number(coord))
		cx1 *= atoi(coord);
	else {
		printf("Invalid command\n");
		return;
	}

	ch = getchar();
	if (ch == '\n') { //nu mai exista coordonata a 3a
		no_coord_errors(ok);
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", coord);
	if (coord[0] == '-') {
		cy2 = -1; //numarul are semnul minus
		strcpy(aux, coord + 1);
		strcpy(coord, aux); //scapam de caracterul minus de la inceput
	}
	if (is_number(coord))
		cy2 *= atoi(coord);
	else {
		printf("Invalid command\n");
		return;
	}

	ch = getchar();
	if (ch == '\n') { //nu mai exista coordonata a 4a
		no_coord_errors(ok);
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", coord);
	if (coord[0] == '-') {
		cx2 = -1; //numarul are semnul minus
		strcpy(aux, coord + 1);
		strcpy(coord, aux); //scapam de caracterul minus de la inceput
	}
	if (is_number(coord))
		cx2 *= atoi(coord);
	else {
		printf("Invalid command\n");
		return;
	}

	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (cx1 < 0 || cx1 > h || cx2 < 0 || cx2 > h || cy1 < 0 || cy1 > w ||
		cy2 < 0 || cy2 > w || cx1 == cx2 || cy1 == cy2) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (cx1 > cx2)
		swap(&cx1, &cx2);
	if (cy1 > cy2)
		swap(&cy1, &cy2);
	//spre ex, supraf det de (3,1) si (0,2) e la fel cu cea de (0,1) si (3,2)
	printf("Selected %d %d %d %d\n", cy1, cx1, cy2, cx2);
	*x1 = cx1, *y1 = cy1, *x2 = cx2, *y2 = cy2;
}

pixel **crop(int *h, int *w, int *x1, int *y1, int *x2, int *y2, pixel **image)
{
	int auxh = *h;
	*h = *x2 - *x1;
	*w = *y2 - *y1;
	pixel **aux = alloc_image(*h, *w);
	for (int i = 0; i < *h; i++)
		for (int j = 0; j < *w; j++) {
			aux[i][j].r = image[*x1 + i][*y1 + j].r;
			aux[i][j].g = image[*x1 + i][*y1 + j].g;
			aux[i][j].b = image[*x1 + i][*y1 + j].b;
			aux[i][j].val = image[*x1 + i][*y1 + j].val;
		}
	printf("Image cropped\n");
	*x1 = 0;
	*y1 = 0;
	*x2 = *h;
	*y2 = *w;
	free_image(auxh, image);
	return aux;
}

void copy_to_image(int x1, int y1, int x2, int y2, pixel **image, pixel **aux)
{
	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++) {
			image[i][j].r = aux[i - x1][j - y1].r;
			image[i][j].g = aux[i - x1][j - y1].g;
			image[i][j].b = aux[i - x1][j - y1].b;
			image[i][j].val = aux[i - x1][j - y1].val;
		}
}

void rotate_90(int *h, int *w, int x1, int y1, int *x2, int *y2, pixel ***ptr)
{
	pixel **image = *ptr;
	if ((*x2 - x1) != (*y2 - y1)) {
		//selectia nu e patrata, deci e intreaga imagine
		pixel **aux = alloc_image(*w, *h); //se inverseaza dimensiunile
		for (int i = 0; i < *w; i++)
			for (int j = 0; j < *h; j++) {
				aux[i][j].r = image[*h - 1 - j][i].r;
				aux[i][j].g = image[*h - 1 - j][i].g;
				aux[i][j].b = image[*h - 1 - j][i].b;
				aux[i][j].val = image[*h - 1 - j][i].val;
			}
		free_image(*h, image);
		swap(h, w);
		swap(x2, y2);
		*ptr = aux;
	} else { //selectia e patrata sau toata imaginea, care e patrata
		int n = *x2 - x1;
		pixel **aux = alloc_image(n, n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				aux[i][j].r = image[*x2 - 1 - j][y1 + i].r;
				aux[i][j].g = image[*x2 - 1 - j][y1 + i].g;
				aux[i][j].b = image[*x2 - 1 - j][y1 + i].b;
				aux[i][j].val = image[*x2 - 1 - j][y1 + i].val;
			}
		copy_to_image(x1, y1, *x2, *y2, image, aux);
		free_image(n, aux);
	}
}

void rotate(int angle, int *h, int *w, int x1, int y1, int *x2, int *y2,
			int ok, pixel ***ptr)
{
	pixel **image = *ptr;
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if ((*x2 - x1) != (*y2 - y1) &&
		(x1 != 0 || y1 != 0 || *x2 != *h || *y2 != *w)) {
		//selectia nu e patrata si nici intreaga imagine
		printf("The selection must be square\n");
		return;
	} else if (angle % 90 || angle > 360 || angle < -360) {
		printf("Unsupported rotation angle\n");
		return;
	} else if (angle == 360 || angle == -360 || angle == 0) {
		//bucata de imagine selectata va ajunge tot in pozitia originala
		//deci va ramane la fel
		printf("Rotated %d\n", angle);
		return;
	} else if (angle == 90 || angle == -270) {
		//rotirea cu aceste unghiuri va aduce imaginea in aceeasi pozitie
		rotate_90(h, w, x1, y1, x2, y2, &image);
		*ptr = image;
		printf("Rotated %d\n", angle);
		return;
	} else if (angle == 180 || angle == -180) {
		//rotirea cu aceste unghiuri va aduce imaginea in aceeasi pozitie
		rotate_90(h, w, x1, y1, x2, y2, &image);
		rotate_90(h, w, x1, y1, x2, y2, &image);
		//rotirea imaginii de 2 ori cu cate 90 grade = rotirea cu 180
		*ptr = image;
		printf("Rotated %d\n", angle);
		return;
	} else if (angle == 270 || angle == -90) {
		//rotirea cu aceste unghiuri va aduce imaginea in aceeasi pozitie
		rotate_90(h, w, x1, y1, x2, y2, &image);
		rotate_90(h, w, x1, y1, x2, y2, &image);
		rotate_90(h, w, x1, y1, x2, y2, &image);
		//rotirea imaginii de 3 ori cu cate 90 grade = rotirea cu 270
		*ptr = image;
		printf("Rotated %d\n", angle);
		return;
	}
}

void check_ascii(char ch, char aux[], int *ascii)
{
	while (ch == ' ')
		ch = getchar();
	if (ch != '\n') {
		ungetc(ch, stdin);
		scanf("%s", aux);
		if (strcmp(aux, "ascii") == 0)
			*ascii = 1;
	}
}

void save(char new_file[101], pixel **image, int h, int w, int max,
		  char type[], int ascii, int ok)
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	FILE *newfile = fopen(new_file, ascii ? "w" : "wb");
	if (ascii && type[1] > '3')
		type[1] -= 3;
	else if (ascii == 0 && type[1] < '4')
		type[1] += 3;
	fprintf(newfile, "%s\n%d %d\n", type, w, h);
	if (type[1] != '1' && type[1] != '4')
		fprintf(newfile, "%d\n", max);
	if (ascii) {
		if (type[1] == '3' || type[1] == '6') {
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					fprintf(newfile, "%d ", image[i][j].r);
					fprintf(newfile, "%d ", image[i][j].g);
					fprintf(newfile, "%d ", image[i][j].b);
				}
				fprintf(newfile, "\n");
			}
		} else {
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++)
					fprintf(newfile, "%d ", image[i][j].val);
				fprintf(newfile, "\n");
			}
		}
	} else {
		if (type[1] == '3' || type[1] == '6') {
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					fwrite(&image[i][j].r, sizeof(unsigned char), 1, newfile);
					fwrite(&image[i][j].g, sizeof(unsigned char), 1, newfile);
					fwrite(&image[i][j].b, sizeof(unsigned char), 1, newfile);
				}
		} else {
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					fwrite(&image[i][j].val, sizeof(unsigned char), 1, newfile);
		}
	}
	fclose(newfile);
	printf("Saved %s\n", new_file);
}

void sum(double ker[][4], pixel **aux, pixel **image, int i, int j)
{
	double r, g, b;
	r = image[i][j].r * ker[1][1];
	g = image[i][j].g * ker[1][1];
	b = image[i][j].b * ker[1][1];
	//separ sumele pe mai multe randuri din cauza limitei de caractere
	r += ker[0][0] * image[i - 1][j - 1].r + ker[0][1] * image[i - 1][j].r;
	r += ker[0][2] * image[i - 1][j + 1].r + ker[1][0] * image[i][j - 1].r;
	r += ker[1][2] * image[i][j + 1].r + ker[2][0] * image[i + 1][j - 1].r;
	r += ker[2][1] * image[i + 1][j].r + ker[2][2] * image[i + 1][j + 1].r;

	g += ker[0][0] * image[i - 1][j - 1].g + ker[0][1] * image[i - 1][j].g;
	g += ker[0][2] * image[i - 1][j + 1].g + ker[1][0] * image[i][j - 1].g;
	g += ker[1][2] * image[i][j + 1].g + ker[2][0] * image[i + 1][j - 1].g;
	g += ker[2][1] * image[i + 1][j].g + ker[2][2] * image[i + 1][j + 1].g;

	b += ker[0][0] * image[i - 1][j - 1].b + ker[0][1] * image[i - 1][j].b;
	b += ker[0][2] * image[i - 1][j + 1].b + ker[1][0] * image[i][j - 1].b;
	b += ker[1][2] * image[i][j + 1].b + ker[2][0] * image[i + 1][j - 1].b;
	b += ker[2][1] * image[i + 1][j].b + ker[2][2] * image[i + 1][j + 1].b;

	if (r > 255)
		r = 255;
	if (r < 0)
		r = 0;
	if (g > 255)
		g = 255;
	if (g < 0)
		g = 0;
	if (b > 255)
		b = 255;
	if (b < 0)
		b = 0;
	aux[i][j].r = round(r);
	aux[i][j].g = round(g);
	aux[i][j].b = round(b);
}

void filter(int h, int w, int x1, int y1, int x2, int y2, pixel **image,
			double ker[][4])
{
	pixel **aux = alloc_image(h, w);
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			aux[i][j].r = image[i][j].r;
			aux[i][j].g = image[i][j].g;
			aux[i][j].b = image[i][j].b;
		}
	int cx1 = x1, cy1 = y1, cx2 = x2, cy2 = y2;
	if (x1 == 0) //pixelii care nu au suficienti vecini raman nemodificati
		cx1 = 1;
	if (x2 == h)
		cx2 = h - 1;
	if (y1 == 0)
		cy1 = 1;
	if (y2 == w)
		cy2 = w - 1;
	for (int i = cx1; i < cx2; i++)
		for (int j = cy1; j < cy2; j++)
			sum(ker, aux, image, i, j);
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			image[i][j].r = aux[i][j].r;
			image[i][j].g = aux[i][j].g;
			image[i][j].b = aux[i][j].b;
		}
	free_image(h, aux);
}

void apply(int h, int w, int x1, int y1, int x2, int y2, char type[2],
		   pixel **image, int ok)
{
	char par[20], ch;
	ch = getchar(); //verificam daca exista parametrul
	if (ch == '\n') {
		if (ok == 0)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", par);
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (type[1] != '3' && type[1] != '6') {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	if (strcmp(par, "EDGE") == 0) {
		double ker[][4] = {
			{-1, -1, -1},
			{-1, 8, -1},
			{-1, -1, -1},
		};
		filter(h, w, x1, y1, x2, y2, image, ker);
	} else if (strcmp(par, "SHARPEN") == 0) {
		double ker[][4] = {
			{0, -1, 0},
			{-1, 5, -1},
			{0, -1, 0},
		};
		filter(h, w, x1, y1, x2, y2, image, ker);
	} else if (strcmp(par, "BLUR") == 0) {
		double ker[][4] = {
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
		};
		filter(h, w, x1, y1, x2, y2, image, ker);
	} else if (strcmp(par, "GAUSSIAN_BLUR") == 0) {
		double ker[][4] = {
			{1.0 / 16, 2.0 / 16, 1.0 / 16},
			{2.0 / 16, 4.0 / 16, 2.0 / 16},
			{1.0 / 16, 2.0 / 16, 1.0 / 16},
		};
		filter(h, w, x1, y1, x2, y2, image, ker);
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
	printf("APPLY %s done\n", par);
}

void histogram(int h, int w, int ok, char type[], pixel **image)
{
	char ch, val[25];
	int x, y;
	ch = getchar();
	if (ch == '\n') { //nu mai exista valoarea x
		if (ok == 0)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", val);
	if (is_number(val))
		x = atoi(val);
	else {
		printf("Invalid command\n");
		return;
	}

	ch = getchar();
	if (ch == '\n') { //nu mai exista valoarea y
		if (ok == 0)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return;
	}
	ungetc(ch, stdin);
	scanf("%s", val);
	if (is_number(val))
		y = atoi(val);
	else {
		printf("Invalid command\n");
		return;
	}

	ch = getchar();
	if (ch != '\n') { //mai sunt si alte lucruri pe langa x si y
		while (ch != '\n')
			ch = getchar();
		if (ok == 0)
			printf("No image loaded\n");
		else
			printf("Invalid command\n");
		return;
	}
	ungetc(ch, stdin);

	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (type[1] == '3' || type[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}
	int *freq = calloc(y, sizeof(int));
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			freq[image[i][j].val / (256 / y)]++;
	int max = 0;
	for (int i = 0; i < y; i++)
		if (freq[i] > max)
			max = freq[i];
	for (int i = 0; i < y; i++) {
		int stars = freq[i] * x / max;
		printf("%d\t|\t", stars);
		for (int j = 0; j < stars; j++)
			printf("*");
		printf("\n");
	}
	free(freq);
}

void equalize(int h, int w, int ok, char type[], pixel **image)
{
	if (ok == 0) {
		printf("No image loaded\n");
		return;
	}
	if (type[1] == '3' || type[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}
	int freq[256] = {0}, area;
	area = h * w;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			freq[image[i][j].val]++;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++) {
			double newval;
			int sum = 0;
			for (int k = 0; k <= image[i][j].val; k++)
				sum += freq[k];
			newval = 255 * 1.0 / area * sum;
			image[i][j].val = round(newval);
			if (image[i][j].val < 0)
				image[i][j].val = 0;
			if (image[i][j].val > 255)
				image[i][j].val = 255;
	}
	printf("Equalize done\n");
}
