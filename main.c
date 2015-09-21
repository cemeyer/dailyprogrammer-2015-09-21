#define	_GNU_SOURCE

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t input_len;
char full_input[1024 * 8];
char inpp[1024 * 8];
char outp[1024 * 16];

unsigned blue_width, wido;
unsigned blue_height, height;

#define	BLUE(r, c)	inpp[ (r) * blue_width + (c) ]

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

static void
draw_ascii(void)
{
}

/* Flesh out rows -- make them consistently wide; drop the N line */
static void
parse_input(void)
{
	unsigned i, j;
	char *inp;

	inp = strchr(full_input, '\n') + 1;
	for (i = 0; i < blue_height; i++) {
		char *next;

		next = strchrnul(inp, '\n');
		if (next - inp > blue_width)
			blue_width = next - inp;
		inp = next + 1;
	}

	/* Rewind... */
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

	print_ascii(inpp, blue_height, blue_width);
}

int
main(int argc, char **argv)
{
	FILE *f;
	int rc;

	if (argc < 2)
		errx(1, "no input provided");

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
