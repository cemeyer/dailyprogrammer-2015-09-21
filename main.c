#define	_GNU_SOURCE

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

size_t input_len;
char full_input[1024 * 8];
char inpp[1024 * 8];
char outp[1024 * 16];
unsigned roofs[256];

unsigned blue_width, wido;
unsigned blue_height, height;

#define	BLUE(r, c)	inpp[ (r) * blue_width + (c) ]
#define	OUT(r,c,dr,dc)	outp[ ((r + blue_width) * 2 + (dr)) * wido + (c) * 4 + (dc)]

static void
print_ascii(const char *i, unsigned h, unsigned w)
{
	unsigned j;

	while (h) {
		for (j = 0; j < w; j++) {
			putchar(*i);
			i++;
		}
		putchar('\n');
		h--;
	}
	putchar('\n');
}

static char
get_blue_neighbor(unsigned r, unsigned c)
{

	if (r >= blue_height || c >= blue_width)
		return (' ');
	return (BLUE(r, c));
}

/* Draw a single "box" (one '*' in the "blueprint") */
static void
drawbox(unsigned row, unsigned col)
{
	bool nl, nr, nu, nd, ndl, ndr;

	nl = nr = nu = nd = ndl = ndr = false;

	/* Special drawing for doors and windows */
	if (BLUE(row, col) == 'h') {
		OUT(row, col, 1, 1) = '|';
		OUT(row, col, 1, 3) = '|';
	} else if (BLUE(row, col) == 'o')
		OUT(row, col, 1, 2) = 'o';

	/* Box corners and walls depend on neighboring squares */
	nl = (get_blue_neighbor(row, col - 1) != ' ');
	nr = (get_blue_neighbor(row, col + 1) != ' ');
	nu = (get_blue_neighbor(row - 1, col) != ' ');
	nd = (get_blue_neighbor(row + 1, col) != ' ');

	ndr = (get_blue_neighbor(row + 1, col + 1) != ' ');
	ndl = (get_blue_neighbor(row + 1, col - 1) != ' ');

	if (!nl) {
		OUT(row, col, 1, 0) = '|';
		if (nd && !ndl)
			OUT(row, col, 2, 0) = '|';
		else
			OUT(row, col, 2, 0) = '+';
		if (!nu)
			OUT(row, col, 0, 0) = '+';
	}
	if (!nr) {
		OUT(row, col, 1, 4) = '|';
		if (nd && !ndr)
			OUT(row, col, 2, 4) = '|';
		else
			OUT(row, col, 2, 4) = '+';
	}
	if (!nd) {
		OUT(row, col, 2, 1) = '-';
		OUT(row, col, 2, 2) = '-';
		OUT(row, col, 2, 3) = '-';
		if (nr)
			OUT(row, col, 2, 4) = '-';
		else
			OUT(row, col, 2, 4) = '+';
	}
	if (!nu) {
		OUT(row, col, 0, 1) = '-';
		OUT(row, col, 0, 2) = '-';
		OUT(row, col, 0, 3) = '-';
		if (nr && OUT(row, col, 0, 4) == ' ')
			OUT(row, col, 0, 4) = '-';
		else
			OUT(row, col, 0, 4) = '+';
	}
}

/*
 * Draw a single steeple above the point on the roof, (row, col), of width
 * rsegw.
 */
static void
drawroof(unsigned col, unsigned row, unsigned rsegw)
{
	int i;

	for (i = 0; i < 2 * (int)rsegw - 1; i++) {
		OUT(row, col, -i - 1, i + 1) = '/';
		OUT(row, col + rsegw - 1, -i - 1, 3 - i) = '\\';
	}
	OUT(row - rsegw, col, 0, 2 * rsegw) = 'A';
}

static void
draw_ascii(void)
{
	unsigned i, j, h;

	/* Single walls are shared, boxes are actually 4x2. */
	wido = 4 * blue_width + 1;
	height = 2 * blue_height + 1 + 2 * blue_width;

	/* Pick a ground-floor location for the door. */
	h = random() % blue_width;
	for (i = 0; h != 0;) {
		if (BLUE(blue_height - 1, i) != ' ') {
			h--;
			i++;
		}
		if (i >= blue_width)
			i = 0;
	}
	BLUE(blue_height - 1, i) = 'h';

	/* Fill in windows randomly */
	for (i = 0; i < blue_height; i++) {
		for (j = 0; j < blue_width; j++) {
			if (BLUE(i, j) != ' ' && BLUE(i, j) != 'h')
				if ((random() % 2) == 0)
					BLUE(i, j) = 'o';
		}
	}

#if 0
	/* Print out our modified blueprint. */
	print_ascii(inpp, blue_height, blue_width);
#endif

	/*
	 * Draw every box in the building.  While we're here, find the highest
	 * boxes across the building (the roof)
	 */
	memset(roofs, 0xff, sizeof(roofs));
	for (i = 0; i < blue_height; i++) {
		for (j = 0; j < blue_width; j++) {
			if (BLUE(i, j) == ' ')
				continue;

			drawbox(i, j);
			if (roofs[j] > i)
				roofs[j] = i;
		}
	}

	/*
	 * Finally, look for 'runs' of level roof (which will share a single
	 * steeple) and draw a steeple for each segment.
	 */
	j = 0;
	h = 0;
	for (i = 0; i < blue_width; i++) {
		if (j == 0) {
			h = roofs[i];
			j++;
		} else if (roofs[i] == h) {
			j++;
		} else {
			drawroof(i - j, h, j);
			j = 1;
			h = roofs[i];
		}
	}
	drawroof(i - j, h, j);

	print_ascii(outp, height, wido);
}

/* Flesh out rows -- make them consistently wide; drop the N line */
static void
parse_input(void)
{
	unsigned i, j;
	char *inp;

	/*
	 * Read through the input once just to determine the width of the
	 * blueprint.
	 */
	inp = strchr(full_input, '\n') + 1;
	for (i = 0; i < blue_height; i++) {
		char *next;

		next = strchrnul(inp, '\n');
		if (next - inp > blue_width)
			blue_width = next - inp;
		inp = next + 1;
	}

	/* Rewind and read into our 2D inpp array. */
	inp = strchr(full_input, '\n') + 1;
	for (i = 0; i < blue_height; i++) {
		for (j = 0; j < blue_width && *inp != '\n'; j++) {
			BLUE(i, j) = *inp;
			inp++;
		}
		if (*inp != '\n')
			inp = strchr(inp, '\n');
		inp++;
	}
}

int
main(int argc, char **argv)
{
	FILE *f;
	int rc;

	if (argc < 2)
		errx(1, "no input provided");

	srand(time(NULL));

	f = fopen(argv[1], "rb");
	if (!f)
		err(1, "fopen(%s)", argv[1]);

	input_len = fread(full_input, 1, sizeof(full_input), f);
	fclose(f);

	memset(outp, ' ', sizeof(outp));
	memset(inpp, ' ', sizeof(inpp));

	rc = sscanf(full_input, "%u", &blue_height);
	if (rc < 1)
		err(1, "sscanf");

	parse_input();
	draw_ascii();
	return (0);
}
