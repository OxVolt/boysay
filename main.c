#include <stdio.h>
#include <string.h>

#define MAX_MSG_LEN 512

int main(int argc, char *argv[]) {
    char msg[MAX_MSG_LEN] = {0};

    // Si aucun argument → lire depuis stdin
    if (argc == 1) {
        if (fgets(msg, sizeof(msg), stdin) == NULL) {
            fprintf(stderr, "Usage: %s [message]\n", argv[0]);
            return 1;
        }
        msg[strcspn(msg, "\n")] = 0; // Enlever le \n
    } 
    else {
        // Concaténer les arguments en une seule phrase
        for (int i = 1; i < argc; i++) {
            strcat(msg, argv[i]);
            if (i < argc - 1) strcat(msg, " ");
        }
    }

    int len = strlen(msg);

    // Afficher la bulle de texte
    printf(" ");
    for (int i = 0; i < len + 2; i++) printf("-");
    printf("\n< %s >\n ", msg);
    for (int i = 0; i < len + 2; i++) printf("-");
    printf("\n");

    // Dessin du personnage


printf("⠀⠀⠀⢀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡀⠀\n");
printf("⠀⠀⢠⠏⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠴⠚⠉⠀⠙⡆\n");
printf("⠀⢰⡏⠀⠀⠀⠀⠙⢦⡀⠀⣶⠓⠲⠦⣤⣀⠀⠀⠀⣴⠟⠁⠀⠀⠀⠀⠀⣿\n");
printf("⠀⣾⠀⠀⠀⠀⠀⠀⠀⠹⣦⠸⣆⠀⠀⠀⠉⠳⣄⣼⠃⠀⠀⠀⠀⠀⠀⠀⢸\n");
printf("⠀⡿⠀⠀⠀⠀⠀⠀⠀⠀⣨⠷⠛⠃⠀⠀⠀⠀⠈⠓⠄⠀⠀⠀⠀⠀⠀⠀⢸\n");
printf("⠀⣧⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n");
printf("⠀⢿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾\n");
printf("⠀⠈⣧⠀⠀⠤⣤⠤⢤⣤⣤⣤⠀⠀⠀⠀⠀⣴⣶⣶⡖⠒⢶⠒⠀⠀⠀⣼⠃\n");
printf("⣤⣄⣘⢧⡀⢰⠇⠀⢸⣿⣿⣿⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠘⡇⠀⡤⠾⠥⣤\n");
printf("⠻⣄⠈⠉⠁⢸⡀⠀⠘⣿⣿⡿⠀⠀⠀⠀⠀⠻⣿⡿⠃⠀⠀⡇⠀⠀⢀⡼⠋\n");
printf("⠀⠈⢻⠆⡄⣦⣡⠄⠀⠈⠉⠀⠈⠉⠁⠀⠀⠀⠀⠀⠀⡴⣦⠒⠀⠐⢯⡀⠀\n");
printf("⠀⣠⠏⠀⠀⠉⠀⠀⠀⠀⠀⠠⡤⠴⠒⠒⠒⠚⠀⠀⠀⠀⠁⢀⠀⠀⢈⣻⡄\n");
printf("⠀⠛⠒⠚⠛⠳⠦⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣤⠴⠞⠋⠉⠉⠉⠉⠀\n");
printf("⠀⠀⠀⠀⠀⠀⠀⠈⢿⣍⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⢻⡄⠀⠀⠀⠀⠀⠀⠀\n");
printf("⠀⠀⠀⠀⠀⠀⠀⠀⢠⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⡄⠀⠀⠀⠀⠀⠀\n");
printf("⠀⠀⠀⠀⠀⠀⠀⠀⠛⢻⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⠀⠀⠀⠀⠀⠀\n");
printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⡇⠀⠀⠀⠀⠀\n");



    return 0;
}
