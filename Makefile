v1 = gcc
v2 = -Wall -Wextra
input = /home/student/Downloads/PCLP1-check-tema3-20241209/tasks/image_editor/tests/00-image_editor/00-image_editor.in
problema = image_editor
build: $(problema)
image_editor: image_editor.c functions.c
	$(v1) $(v2) image_editor.c functions.c -lm -o image_editor
clean:
	rm -f $(problema) *.o