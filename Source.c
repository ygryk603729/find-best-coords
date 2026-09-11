#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int x;
	int y;
} Point;

int read_points(const char* filename, Point** points, int* N, int* N_max) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error opening file\n");
		return 1;
	}

	while (fscanf(file, "%d,%d", &(*points)[*N].x, &(*points)[*N].y) == 2) {		
		(*N)++;

		//динамическое увеличение массива при превышении размера
		if ((*N)+1 > *N_max) {
			*N_max *= 2;
			Point* new_points = (Point*)realloc(*points, *N_max * sizeof(Point));

			if (new_points != NULL) {
				*points = new_points;
			}
			else {
				printf("Error reallocating memory\n");
				fclose(file);
				return 1;
			}
		}
	}

	if (!feof(file)) {
		printf("Error: incorrect data in line %d\n", *N);
		return 1;
	}
	fclose(file);

	if (*N == 0) { 
		printf("Error, 0 target detected\n");
		return 1; }
	return 0;
}

int find_best_coords(Point* points, const int R, int* max_aims, const int N, Point* best_coords) {
	*max_aims = 0;

	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			int curr_aims = 0;
			for (int i = 0; i < N; i++) {
				int dx = points[i].x - x;
				int dy = points[i].y - y;
				if (dx * dx + dy * dy <= R * R) {
					curr_aims++;
				}
			}
			if (curr_aims > *max_aims) {
				*max_aims = curr_aims;
				(*best_coords).x = x;
				(*best_coords).y = y;
			}
		}
	}
	if (*max_aims == 0) return -1;
	else return 0;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: %s <filename> <radius>\n", argv[0]);
		return 1;
	}
	const char* filename = argv[1];
	const int R = atoi(argv[2]);
	if (R <= 0) {
		printf("Error: radius must be a positive number\n");
		return 1;
	}

	int N = 0;
	int N_max = 100;
	Point* points = (Point*)malloc(N_max * sizeof(Point)); //значение 100 взято как константа, так как координаты 0-99
	if (points == NULL) {
		printf("Error allocating memory\n");
		free(points);
		points = NULL;
		return 1;
	}
	if (read_points(filename, &points, &N, &N_max) != 0) {
		return 1;
	}

	int max_aims;
	Point best_coords = { -1, -1 };
	if (find_best_coords(points, R, &max_aims, N, &best_coords)==0) //в случае успеха
		printf("Best coordinates: (%d, %d), hitting %d aims\n", best_coords.x, best_coords.y, max_aims);
	else 
		printf("We cannot hit any targets, radius is too small\n");
	free(points);
	return 0;
}