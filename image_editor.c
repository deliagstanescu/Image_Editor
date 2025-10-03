#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

int main(void)
{
	pixel **image = NULL;
	char comm[15], name[101], type[3];
	int ok = 0, x1 = 0, x2, y1 = 0, y2, h, w, max = 0, angle, exit = 0;
	FILE *file;
	while (scanf("%s", comm)) {
		if (strcmp(comm, "LOAD") == 0) {
			scanf("%s", name);
			file = fopen(name, "r");
			if (ok && image)//a fost retinuta alta imagine ce trebuie eliberata
				free_image(h, image);
			load_file(name, file, &ok);
			if (ok) {//fisierul a fost incarcat cu succes
				skip_comm(file);
				fscanf(file, "%s", type);
				values(file, type, &w, &h, &max);
				x1 = 0, x2 = h, y1 = 0, y2 = w;
				image = alloc_image(h, w);
				if (type[1] == '3' || type[1] == '6')//imaginea e color
					load_image_rgb(file, type, h, w, image);
				else
					load_image(file, type, h, w, image);
				fclose(file);
			}
		} else if (strcmp(comm, "SELECT") == 0) {
			char aux[10];
			scanf("%s", aux);
			if (strcmp(aux, "ALL") == 0) {
				select_all(h, w, &x1, &y1, &x2, &y2, ok);
			} else {
				int cy1;
				cy1 = atoi(aux);//ii dam lui cy1 val numarului citit de aux
				sel_val(h, w, &x1, &y1, &x2, &y2, cy1, ok);
			}
		} else if (strcmp(comm, "ROTATE") == 0) {
			scanf("%d", &angle);
			rotate(angle, &h, &w, x1, y1, &x2, &y2, ok, &image);
		} else if (strcmp(comm, "EQUALIZE") == 0) {
			equalize(h, w, ok, type, image);
		} else if (strcmp(comm, "CROP") == 0) {
			if (ok == 0) {
				printf("No image loaded\n");
			} else {
				pixel **aux_img = crop(&h, &w, &x1, &y1, &x2, &y2, image);
				image = aux_img;
			}
		} else if (strcmp(comm, "APPLY") == 0) {
			apply(h, w, x1, y1, x2, y2, type, image, ok);
		} else if (strcmp(comm, "HISTOGRAM") == 0) {
			histogram(h, w, ok, type, image);
		} else if (strcmp(comm, "SAVE") == 0) {
			char new_file[101], aux[10];
			int ascii = 0;
			scanf("%s", new_file);
			char ch = getchar();
			check_ascii(ch, aux, &ascii);
			save(new_file, image, h, w, max, type, ascii, ok);
		} else if (strcmp(comm, "EXIT") == 0) {
			if (ok)
				free_image(h, image);
			else
				printf("No image loaded\n");
			exit = 1;
		} else {
			char ch;
			while ((ch = getchar()) != '\n' && ch != EOF)
				continue; //scapam de tot randul comenzii neidentificate
			printf("Invalid command\n");
		}
		if (exit)
			return 0;
	}
	return 0;
}
