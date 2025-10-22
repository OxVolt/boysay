#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>

#define MAX_MSG_LEN 4096
#define WRAP_WIDTH  40

// ------------------------------------------------------------
// Error handling
// ------------------------------------------------------------
static void die(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

// ------------------------------------------------------------
// Reading the message (stdin or arguments)
// ------------------------------------------------------------
static char* read_stdin_all(void) {
    static char buf[MAX_MSG_LEN];
    size_t len = 0;
    int c;

    while ((c = fgetc(stdin)) != EOF) {
        if (len + 1 >= sizeof(buf))
            die("input too long");
        buf[len++] = (char)c;
    }
    buf[len] = '\0';
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    return buf;
}

static char* join_args_skipping_flags(int argc, char **argv, char **out_size, char **out_flag) {
    char *size = "small"; // default
    char *flag = "default"; // default
    static char msg[MAX_MSG_LEN];
    msg[0] = '\0';

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ss") == 0) { size = "small";  continue; }
        if (strcmp(argv[i], "-sm") == 0) { size = "medium"; continue; }
        if (strcmp(argv[i], "-sb") == 0) { size = "big";    continue; }
        
        if (strcmp(argv[i], "-d") == 0) { flag = "default";    continue; }
        if (strcmp(argv[i], "-c") == 0) { flag = "cute";    continue; }
        if (strcmp(argv[i], "-s") == 0) { flag = "standing";    continue; }

        if (strlen(msg) + strlen(argv[i]) + 1 >= sizeof(msg))
            die("message too long");
        if (msg[0] != '\0') strcat(msg, " ");
        strcat(msg, argv[i]);
    }

    *out_size = size;
    *out_flag = flag;
    if (msg[0] == '\0') return NULL;
    return msg;
}

// ------------------------------------------------------------
// Text formatting (text bubbles)
// ------------------------------------------------------------
static void wrap_text(const char *text, int width, char ***out_lines, size_t *out_count) {
    if (width < 5) width = 5;
    char *work = strdup(text ? text : "");
    if (!work) die("oom");

    // Replaces tabs with spaces
    for (char *p = work; *p; p++) {
        if (*p == '\t')
            *p = ' ';
    }

    // Prepare the list of lines
    char **lines = NULL;
    size_t count = 0, cap = 0;
    char *p = work;


    while (*p) {
        while (*p == ' ') p++;
        if (*p == '\0') break;

        size_t take = 0, last_space = 0;
        while (p[take] && p[take] != '\n' && take < (size_t)width) {
            if (p[take] == ' ') last_space = take;
            take++;
        }

        size_t cut = take;
        if (p[cut] && p[cut] != '\n' && last_space > 0)
            cut = last_space;

        if (cap == count) {
            cap = cap ? cap * 2 : 8;
            char **tmp = realloc(lines, cap * sizeof(*lines));
            if (!tmp) die("oom");
            lines = tmp;
        }

        size_t seglen = cut;
        while (seglen > 0 && p[seglen - 1] == ' ') seglen--;
        char *line = malloc(seglen + 1);
        if (!line) die("oom");
        memcpy(line, p, seglen);
        line[seglen] = '\0';
        lines[count++] = line;

        if (p[cut] == '\n') p += cut + 1;
        else p += cut;
    }

    if (count == 0) {
        lines = malloc(sizeof(*lines));
        if (!lines) die("oom");
        lines[0] = strdup("");
        count = 1;
    }

    free(work);
    *out_lines = lines;
    *out_count = count;
}

static void free_lines(char **lines, size_t n) {
    for (size_t i = 0; i < n; i++)
        free(lines[i]);
    free(lines);
}

static void print_bubble(const char *msg) {
    char **lines = NULL;
    size_t n = 0;
    wrap_text(msg, WRAP_WIDTH, &lines, &n);

    size_t maxw = 0;
    for (size_t i = 0; i < n; i++) {
        if (strlen(lines[i]) > maxw)
            maxw = strlen(lines[i]);
    }

    printf(" ");
    for (size_t i = 0; i < maxw + 2; i++)
        putchar('-');
    putchar('\n');

    if (n == 1) {
        printf("< %s", lines[0]);
        for (size_t k = strlen(lines[0]); k < maxw; k++) putchar(' ');
        printf(" >\n");
    } else {
        printf("/ %s", lines[0]);
        for (size_t k = strlen(lines[0]); k < maxw; k++) putchar(' ');
        printf(" \\\n");

        for (size_t i = 1; i + 1 < n; i++) {
            printf("| %s", lines[i]);
            for (size_t k = strlen(lines[i]); k < maxw; k++) putchar(' ');
            printf(" |\n");
        }

        printf("\\ %s", lines[n - 1]);
        for (size_t k = strlen(lines[n - 1]); k < maxw; k++) putchar(' ');
        printf(" /\n");
    }

    printf(" ");
    for (size_t i = 0; i < maxw + 2; i++)
        putchar('-');
    putchar('\n');

    free_lines(lines, n);
}

static void print_tail(void) {
    printf("        \\\n");
    printf("         \\\n");
}

// ------------------------------------------------------------
// Reading ASCII art (local or installed)
// ------------------------------------------------------------
static void print_ascii_from_file(const char *size, const char *flag) {
    char path[512];
    FILE *f = NULL;

    // Search in the local folder (dev)
    snprintf(path, sizeof(path), "ASCII/%s/%s.txt", size, flag);
    f = fopen(path, "r");

    // If not found, search in the installation folder
    if (!f) {
        snprintf(path, sizeof(path),
                 "/usr/local/share/boysay/ASCII/%s/%s.txt", size, flag);
        f = fopen(path, "r");
    }

    // Minimal fallback if not found
    if (!f) {
        fprintf(stderr, "Warning: could not find ASCII art for size '%s'\n", size);
        printf("   🧍‍♂️ You like kissing boys, don't you?\n");
        return;
    }

    char line[4096];
    while (fgets(line, sizeof(line), f))
        fputs(line, stdout);
    fclose(f);
}

// ------------------------------------------------------------
// Main program
// ------------------------------------------------------------
int main(int argc, char **argv) {
    char *size = NULL;
    char *flag = NULL;
    char *msg = join_args_skipping_flags(argc, argv, &size, &flag);

    char *final_msg = NULL;
    if (!msg) {
        if (isatty(STDIN_FILENO))
            final_msg = "You like kissing boys don't you?";
        else {
            final_msg = read_stdin_all();
            if (final_msg[0] == '\0')
                final_msg = "You like kissing boys don't you?";
        }
    } else {
        final_msg = msg;
    }

    print_bubble(final_msg);
    print_tail();
    print_ascii_from_file(size, flag);
    return 0;
}
