#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


bool** create_laby(int len){
    bool** laby = malloc(sizeof(bool*)*len);
    for (int i = 0; i < len; i++){
        laby[i] = malloc(sizeof(bool)*4);
        for (int j = 0 ; j < 4 ; j++){
            laby[i][j] = false;
        }
    }
    return laby;
}

void destroy_laby(bool** laby, int len){
    for (int i = 0 ; i < len ; i++){
        free(laby[i]);
    }
    free(laby);
}


void add_edge(bool** laby, int n, int len, int s1, int s2){
    assert(s1 >= 0 && s1 < len && s2 >= 0 && s2 < len);
    if (s2 == s1 + n){
        laby[s1][3] = true;
        laby[s2][0] = true;
    }
    else if (s2 == s1 - n){
        laby[s1][0] = true;
        laby[s2][3] = true;
    }
    else if (s2 == s1 - 1){
        laby[s1][1] = true;
        laby[s2][2] = true;
    }
    else if (s2 == s1 + 1){
        laby[s1][2] = true;
        laby[s2][1] = true;
    }
}

int i_voisin(int i, int s, int n){
    if (i == 0) return s - n;
    else if (i == 1) return s - 1;
    else if (i == 2) return s + 1;
    else if (i == 3) return s + n;
    else return -1 ;
    }

void explore(bool** laby, int n, int len, int s, bool* vus, bool** laby2, int papa){
    if (!vus[s]){
        vus[s] = true;
        add_edge(laby2, n, len, papa, s);


        /* determination de l'ordre de passage des voinsins dans explore*/
        int* voisins = malloc(sizeof(int)*4);
        voisins[0] = 0; voisins[1] = 1 ; voisins[2] = 2 ; voisins[3]= 3 ;
        for (int i = 0 ; i < 3 ; i++){
            int v1 = rand() % (4 - i) + i;

            /* swap v1 et la case d'indide 0*/
            int temp = voisins[v1];
            voisins[v1] = voisins[i];
            voisins[i] = temp;
        }

        int num0 = voisins[0]; int num1 = voisins[1]; int num2 = voisins[2]; int num3 = voisins[3]; 
        int i_v0 = i_voisin(num0, s, n); int i_v1 = i_voisin(num1, s, n); 
        int i_v2 = i_voisin(num2, s, n); int i_v3 = i_voisin(num3, s, n);
        free(voisins);

        if (laby[s][num0]){
            explore(laby, n, len, i_v0, vus, laby2, s);
        }
        if (laby[s][num1]){
            explore(laby, n, len, i_v1, vus, laby2, s);
        }
        if (laby[s][num2]){
            explore(laby, n, len, i_v2, vus, laby2, s);
        }
        if (laby[s][num3]){
            explore(laby, n, len, i_v3, vus, laby2, s);
        }
    }
}

bool** generation_laby(int n /*taille d'un coté du carré*/){
    int len = n*n;
    bool** laby1 = create_laby(len);
    bool** laby2 = create_laby(len);

    /* ajout de toute les arretes de laby1 */
    for (int j = 0 ; j < n-1; j++){
        for (int i = 0 ; i < n-1 ; i++){
            add_edge(laby1, n, len, j*n+i, j*n+i+1);
            add_edge(laby1, n, len, j*n+i, (j+1)*n+i);
        }
        add_edge(laby1, n, len, (n-1)*n + j, (n-1)*n + j+1);
        add_edge(laby1, n, len, j*n + n-1, (j+1)*n + n-1);
    }

    bool* vus = malloc(sizeof(bool)* len);
    for (int i = 0 ; i < len ; i++){
        vus[i] = false;
    }
    explore(laby1, n, len, 0, vus, laby2, 0);

    free(vus);    
    destroy_laby(laby1, n*n);
    return laby2;
}


void escape_game(bool** laby, int n, int entree, int sortie){
    int s_actu = entree;
    char a = 0;
    printf("Press :   z     to move.\n        q s d \n\nAre you ready ? Y / N : ");
    scanf("%c", &a);
    if (a == 'N') return ; 
    else{
        printf("======================\n");
        int compteur = 1;
        while (s_actu != sortie){
            printf("Mouvement n°%d\n\n", compteur);
            bool haut = laby[s_actu][0];
            bool gauche = laby[s_actu][1];
            bool droite = laby[s_actu][2];
            bool bas = laby[s_actu][3];

            if (haut) printf("   ^\n   |\n");
            else printf("  \n");
            if (gauche) printf("<-   ");
            else printf("     ");
            if (droite) printf("->\n");
            else printf("  \n");
            if (bas) printf("   |\n   v\n");

            char b = 0;
            scanf("%c", &b);
            scanf("%c", &b);
            if (b == 'q' && gauche ) s_actu = s_actu - 1;
            else if (b == 'd' && droite ) s_actu = s_actu + 1; 
            else if (b == 'z' && haut) s_actu = s_actu - n;
            else if (b == 's' && bas) s_actu = s_actu + n ;
            else printf("tu ne peux pas aller dans cette direction, recommences.\n");
            printf("==========================\n");
            compteur += 1;
        }
        printf("Bravo tu as trouvé la sortie ! (en %d mouvements)\n", compteur - 1);
        destroy_laby(laby, n*n);
        return ;
    }
}

int main(int argc, char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    else {
        int n = atoi(argv[1]);
        escape_game(generation_laby(n), n, 0, n*n - 1);
        return 0;
    }
}