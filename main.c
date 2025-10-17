#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_MSG_LEN 4096
#define WRAP_WIDTH  40

static void die(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

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

    // Supprime un éventuel dernier '\n' inutile
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';

    return buf;
}


static char* join_args_skipping_flags(int argc, char **argv, char **out_size){
    char *size = "small"; // default
    // build message in a buffer
    static char msg[MAX_MSG_LEN];
    msg[0] = '\0';

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ss") == 0) { size = "small";  continue; }
        if (strcmp(argv[i], "-sm") == 0) { size = "medium"; continue; }
        if (strcmp(argv[i], "-sb") == 0) { size = "big";    continue; }
        // any other arg becomes part of the message
        if (strlen(msg) + strlen(argv[i]) + 1 >= sizeof(msg))
            die("message too long");
        if (msg[0] != '\0') strcat(msg, " ");
        strcat(msg, argv[i]);

    }

    *out_size = size;
    if (msg[0] == '\0') return NULL; // means: read from stdin



    return msg;
}

// Wrap `text` into lines of max `width` chars (breaking on spaces when possible).
// Produces a heap-allocated array of heap-allocated strings.
// Caller must free all lines and the array.
static void wrap_text(const char *text, int width, char ***out_lines, size_t *out_count) {
    if (width < 5) width = 5;

    // Copie de travail + nettoyage basique
    char *work = strdup(text ? text : "");
    if (!work) die("oom");
    for (char *p = work; *p; p++) {
        if (*p == '\t') *p = ' '; // remplacer tabulations
    }

    char **lines = NULL;
    size_t count = 0, cap = 0;

    char *p = work;
    bool first_line = true;

    while (*p) {
        // On saute les espaces seulement avant la toute première ligne
        if (first_line) {
            while (*p == ' ') p++;
            first_line = false;
        }

        if (*p == '\0') break;

        size_t take = 0;
        size_t last_space = 0;

        // Cherche jusqu'à la largeur max ou un saut de ligne
        while (p[take] && p[take] != '\n' && take < (size_t)width) {
            if (p[take] == ' ') last_space = take;
            take++;
        }

        size_t cut = take;
        if (p[cut] == '\n') {
            // On s'arrête au saut de ligne explicite
        } else if (p[cut] && p[cut] != '\n' && last_space > 0) {
            // Si on dépasse la largeur, on coupe au dernier espace
            cut = last_space;
        }

        // Allocation dynamique du tableau de lignes
        if (cap == count) {
            cap = cap ? cap * 2 : 8;
            char **tmp = realloc(lines, cap * sizeof(*lines));
            if (!tmp) die("oom");
            lines = tmp;
        }

        // Longueur du segment (sans les espaces de fin)
        size_t seglen = cut;
        while (seglen > 0 && p[seglen - 1] == ' ') seglen--;

        // Copie du segment dans une nouvelle ligne
        char *line = malloc(seglen + 1);
        if (!line) die("oom");
        memcpy(line, p, seglen);
        line[seglen] = '\0';
        lines[count++] = line;

        // Avance le pointeur dans le texte
        if (p[cut] == '\n') {
            p += cut + 1; // saute le \n
        } else if (p[cut] == '\0') {
            p += cut;
        } else {
            p += cut;
            while (*p == ' ') p++; // saute les espaces inutiles entre mots
        }
    }

    // Si aucun texte n’a été capturé, créer une ligne vide
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
    for (size_t i = 0; i < n; i++) free(lines[i]);
    free(lines);
}

static void print_bubble(const char *msg) {
    char **lines = NULL;
    size_t n = 0;
    wrap_text(msg, WRAP_WIDTH, &lines, &n);

    // find max width
    size_t maxw = 0;
    for (size_t i = 0; i < n; i++) {
        size_t w = strlen(lines[i]);
        if (w > maxw) maxw = w;
    }

    // top border
    printf(" ");
    for (size_t i = 0; i < maxw + 2; i++) putchar('-');
    putchar('\n');

    if (n == 1) {
        // single-line style: < msg >
        printf("< %s", lines[0]);
        // padding
        for (size_t k = strlen(lines[0]); k < maxw; k++) putchar(' ');
        printf(" >\n");
    } else {
        // multi-line style:
        /* / line1 \ */
        // | ...   |
        // \ lineN /
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

    // bottom border
    printf(" ");
    for (size_t i = 0; i < maxw + 2; i++) putchar('-');
    putchar('\n');

    free_lines(lines, n);
}

static void print_tail(void) {
    // petit "curseur" de bulle type cowsay
    printf("        \\\n");
    printf("         \\\n");
}

static void print_ascii_from_file(const char *size) {
    char path[256];
    snprintf(path, sizeof(path), "ASCII/%s/default.txt", size);

    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Warning: could not open '%s' (falling back to ASCII/small/default.txt)\n", path);
        snprintf(path, sizeof(path), "ASCII/small/default.txt");
        f = fopen(path, "r");
        if (!f) die("failed to open any ASCII art file");
    }

    char line[4096];
    while (fgets(line, sizeof(line), f)) {
        fputs(line, stdout); // conserve les \n du fichier
    }
    fclose(f);
}

int main(int argc, char **argv) {
    char *size = NULL;
    char *msg = join_args_skipping_flags(argc, argv, &size);

    char *final_msg = NULL;
    if (!msg) {
        if (isatty(STDIN_FILENO)) {
            // Pas de pipe ni de redirection → message par défaut
            final_msg = "You like kissing boys don't you?";
        } else {
            final_msg = read_stdin_all();
            if(final_msg[0] == '\0'){
                final_msg = "You like kissing boys don't you?";
            }
        }
    } else {
        final_msg = msg;
    }


    print_bubble(final_msg);
    print_tail();
    print_ascii_from_file(size);
    return 0;
}

