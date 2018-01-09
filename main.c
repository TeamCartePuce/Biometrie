#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"

void copie_pgm() {
	long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	byte **I;

	I = LoadPGM_bmatrix("cubesx3.pgm", &nrl, &nrh, &ncl, &nch);

	SavePGM_bmatrix(I,nrl,nrh,ncl,nch,"imageTest.pgm");
	free_bmatrix(I, nrl, nrh, ncl, nch);
}

void passe_bas(char *imageDepart, char *imageArrivee, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3) {
	long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	byte **I, **R; //Image de départ, image d'arrivée
	int **Int;	//Filtre
	long i, j, k, l;
	int somme;

	I = LoadPGM_bmatrix(imageDepart, &nrl, &nrh, &ncl, &nch);
	R = bmatrix(nrl, nrh, ncl, nch);

	//Initialisation de l'image finale
	for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
			R[i][j] = 0;
		}
	}

	//Initialisation du masque
	Int = imatrix(0,2,0,2);
	Int[0][0] = x1;
	Int[0][1] = x2;
	Int[0][2] = x3;
	Int[1][0] = y1;
	Int[1][1] = y2;
	Int[1][2] = y3;
	Int[2][0] = z1;
	Int[2][1] = z2;
	Int[2][2] = z3;

	//Application du masque sur l'image
	for(i=(nrl+1); i<nrh; i++) {
		for(j=(ncl+1);j<nch;j++) {
			somme = 0;
			for(k=-1;k<=1;k++) {
				for(l=-1;l<=1;l++) {
					somme += Int[k+1][l+1]*I[i+k][j+l];
				}
			}
			R[i][j] = abs(somme/9);
		}
	}


	//Création de l'image
	SavePGM_bmatrix(R,nrl,nrh,ncl,nch,imageArrivee);

	free_bmatrix(I, nrl, nrh, ncl, nch);
	free_bmatrix(R, nrl, nrh, ncl, nch);
	free_imatrix(Int, 0, 2, 0, 2);
}

void gradient_horizontal(char *imageDepart, char *imageArrivee) {
	passe_bas(imageDepart, imageArrivee,-1,-2,-1,0,0,0,1,2,1);
}

void gradient_vertical(char *imageDepart, char *imageArrivee) {
	passe_bas(imageDepart, imageArrivee, -1,0,1,-2,0,+2,-1,0,1);
}

void gradient_norme(char *imageGradientHor, char *imageGradientVer, char *imageGradientNorme) {

    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	byte **H, **V, **R; //Image gradient horizontal, image gradient vertical, image d'arrivée
	int i, j;

    H = LoadPGM_bmatrix(imageGradientHor, &nrl, &nrh, &ncl, &nch);
    V = LoadPGM_bmatrix(imageGradientVer, &nrl, &nrh, &ncl, &nch);

	R = bmatrix(nrl, nrh, ncl, nch);

	//Initialisation de l'image finale
	for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
			R[i][j] = 0;
		}
	}

	for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
			R[i][j] = (int) (sqrt((pow(H[i][j], 2)) + (pow(V[i][j], 2))));
		}
	}

    //Création de l'image
	SavePGM_bmatrix(R,nrl,nrh,ncl,nch,imageGradientNorme);

	free_bmatrix(H, nrl, nrh, ncl, nch);
	free_bmatrix(V, nrl, nrh, ncl, nch);
	free_bmatrix(R, nrl, nrh, ncl, nch);
}

double meanGradiant(char *imageGradientNorme){
    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	byte **H; //Image gradient horizontal, image gradient vertical, image d'arrivée
	int i, j;
    double mean = 0;
    H = LoadPGM_bmatrix(imageGradientNorme, &nrl, &nrh, &ncl, &nch);
    int totalPixel = (nrh+1)*(nch+1);
    //printf("Nombre de pixels : %d\n", totalPixel);

    for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
			mean +=H[i][j];
		}
	}

	mean /= totalPixel;

    printf("Moyenne du gradient : %f\n", mean);

    free_bmatrix(H, nrl, nrh, ncl, nch);

    return mean;
}

void histogramme(int* tab, char *image) {

    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	byte **I;
	int i, j, tmp;

	I = LoadPGM_bmatrix(image, &nrl, &nrh, &ncl, &nch);

    for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
            //printf("Niveau de gris : %d\n", I[i][j]);
            tmp = I[i][j];
            tab[tmp]++;
		}
	}

	free_bmatrix(I, nrl, nrh, ncl, nch);
}

//Calcule le taux de rouge, vert et bleu pour une image
double* taux_couleur(char *image, int* tabRouge, int* tabVert, int* tabBleu) {

    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
    rgb8 **I;
    int i, j, tmpR, tmpV, tmpB;
    double* taux;
    double rouge=0, vert=0, bleu=0;

    taux = malloc(3 * sizeof(double));

    I = LoadPPM_rgb8matrix(image, &nrl, &nrh, &ncl, &nch);
    int totalPixel = (nrh+1)*(nch+1);

    for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
            rouge += (I[i][j]).r;
            vert += (I[i][j]).g;
            bleu += (I[i][j]).b;
            tmpR = (I[i][j]).r;
            tabRouge[tmpR]++;
            tmpV = (I[i][j]).g;
            tabVert[tmpV]++;
            tmpB = (I[i][j]).b;
            tabBleu[tmpB]++;
		}
    }

    rouge /= totalPixel;
    vert /= totalPixel;
    bleu /= totalPixel;

    rouge = rouge * 100 / 255;
    vert = vert * 100 / 255;
    bleu = bleu * 100 / 255;

    printf("Rouge %.0f%% - Vert %.0f%% - Bleu %.0f%%\n", rouge, vert, bleu);
    taux[0] = rouge;
    taux[1] = vert;
    taux[2] = bleu;

    free_rgb8matrix(I, nrl, nrh, ncl, nch);
    return taux;
}

//Transforme une image couleur en image en niveau de gris (.ppm en .pgm)
void CouleurEnNiveauGris(char *imageCouleur, char *imageNDG) {

    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
    byte **I;
    rgb8 **C;
    int i, j;

    C = LoadPPM_rgb8matrix(imageCouleur, &nrl, &nrh, &ncl, &nch);
    I = bmatrix(nrl, nrh, ncl, nch);

    for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
            I[i][j] = (C[i][j].r + C[i][j].g + C[i][j].b) / 3;
		}
    }

    SavePGM_bmatrix(I,nrl,nrh,ncl,nch,imageNDG);

    free_rgb8matrix(C, nrl, nrh, ncl, nch);
    free_bmatrix(I, nrl, nrh, ncl, nch);
}

void distance_histogramme(int* t1,int* t2){
        int i;
        int tableau[256];
        for (i=0;i<256;i++){
        tableau[i]=fabs(t1[i]-t2[i]);
    }
  for(i = 0; i < 256; i++) {
        printf("Case %d : %d\n", i, tableau[i]);
    }
}

void getContour(char *image, char *image2){
  long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
    byte **H, **R;
    int i, j;
  //H = LoadPGM_bmatrix("image_grad_norme.pgm", &nrl, &nrh, &ncl, &nch);
    H = LoadPGM_bmatrix(image, &nrl, &nrh, &ncl, &nch);
    R = bmatrix(nrl, nrh, ncl, nch);
  int seuil = 7;

  for(i=nrl;i<=nrh;i++) {
        for(j=ncl;j<=nch;j++) {
            if(H[i][j]>=seuil){
                R[i][j]=255;
            }
            else{
                R[i][j]=0;
            }
        }
    }

    //SavePGM_bmatrix(R,nrl,nrh,ncl,nch,"image_contour.pgm");
    SavePGM_bmatrix(R,nrl,nrh,ncl,nch,image2);


    free_bmatrix(H, nrl, nrh, ncl, nch);
    free_bmatrix(R, nrl, nrh, ncl, nch);
}

int countPixelContour(char *image){
    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
    byte **H;
    int i, j;
    //H = LoadPGM_bmatrix("image_contour.pgm", &nrl, &nrh, &ncl, &nch);
    H = LoadPGM_bmatrix(image, &nrl, &nrh, &ncl, &nch);
    int compteur=0;
    //printf("Nombre de pixels : %d\n", totalPixel);

    for(i=nrl;i<=nrh;i++) {
        for(j=ncl;j<=nch;j++) {
            if(H[i][j]==255){
                compteur++;
            }
        }
    }

    printf("Nombre de pixels dans contour : %d\n", compteur);
    free_bmatrix(H, nrl, nrh, ncl, nch);
    return compteur;
}

int countPixel(char *image){
    long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
    byte **H;
    H = LoadPGM_bmatrix(image, &nrl, &nrh, &ncl, &nch);
    int totalPixel = (nrh+1)*(nch+1);
    printf("Nombre de pixels total : %d\n", totalPixel);
    free_bmatrix(H, nrl, nrh, ncl, nch);
    return totalPixel;
}

char * concat(char * str1, char * str2){
    int nbc1 = strlen(str1);
    int nbc2 = strlen(str2);
    unsigned int i = 0;
    char * rstr = (char*)malloc((nbc1+nbc2+1) * sizeof(char));
    for(i = 0; i < nbc1; i++){
        *(rstr + i) = *(str1 + i);
    }

    for(i = 0; i < nbc2; i++){
        *(rstr + nbc1 + i) = *(str2 + i);
    }
    *(rstr + nbc1 + nbc2 + 1) = '\0';
    return rstr;
}

char* strext(char* src) {
    int size = strlen(src);
    int i,j;
    char *nom;
    for(i=size-1; i>=0; i--) {
        if(src[i] == '.') {
            nom = (char*) calloc(i, sizeof(char));
            if(nom==NULL) {
                perror("calloc");
            }
            else {
            for(j=0; j<i; j++) {
                nom[j] = src[j];
            }
            return nom;
            }
        }
    }
    return src;
}

char* extensionTxt(char *str) {
    str = strext(str);
    strcat(str, ".txt");
    return str;
}

void write_histogramme(char *nomImage, double moyenneNormeGradient, int nbPixel, int nbPixelContour, double *tauxCouleur,
                       int *NDG, int *R, int *G, int *B) {
    FILE* fichier;
    char* nom = extensionTxt(nomImage);

    fichier = fopen(nom, "w+");
    int i;

    if(fichier != NULL) {
      /*  for(i = 0; i < 256; i++) {
            if(tab[i]!=0) {
                fprintf(fichier, "%d:%d\n", i, tab[i]);
            }
        }*/
        fprintf(fichier, "nom:%s\n", nomImage);
        fprintf(fichier, "moy:%f\n", moyenneNormeGradient);
        fprintf(fichier, "nbPixel:%d\n", nbPixel);
        fprintf(fichier, "nbPixelContour:%d\n", nbPixelContour);
        fprintf(fichier, "rouge:%.0f;vert:%.0f;bleu:%.0f\n", tauxCouleur[0], tauxCouleur[1], tauxCouleur[2]);

        fprintf(fichier, "NDG=");
        for(i=0; i<256; i++) {
            fprintf(fichier, "%d:%d", i, NDG[i]);
            if(i!=255) fprintf(fichier, ";");
        }
        fprintf(fichier, "\n");
        fprintf(fichier, "R=");
        for(i=0; i<256; i++) {
            fprintf(fichier, "%d:%d", i, R[i]);
            if(i!=255) fprintf(fichier, ";");
        }
        fprintf(fichier, "\n");
        fprintf(fichier, "G=");
        for(i=0; i<256; i++) {
            fprintf(fichier, "%d:%d", i, G[i]);
            if(i!=255) fprintf(fichier, ";");
        }
        fprintf(fichier, "\n");
        fprintf(fichier, "B=");
        for(i=0; i<256; i++) {
            fprintf(fichier, "%d:%d", i, B[i]);
            if(i!=255) fprintf(fichier, ";");
        }
        fprintf(fichier, "\n");

        fclose(fichier);
    }
    else {
        printf("Impossible d'ouvrir le fichier !\n");
    }
}

void derouler(char* image) {
    printf("image : %s\n", image);
    char *imageBase = image;
    printf("imageBase : %s\n", imageBase);

    char *imageStr = strext(image);
    printf("imageStr : %s\n", imageStr);

    char *imageNDG = concat(imageStr, ".pgm");
    printf("imageNDG : %s\n", imageNDG);

    //imageStr = strext(image);
	char *imagePasseBas = concat(imageStr, "_PB.pgm");
	printf("imagePasseBas : %s\n", imagePasseBas);

	//Cette ligne plante (j'ai fais un exemple en commentaire qui plante pas dans le main)
	//imageStr = strext(image);
	char *imageGradientHorizontal = concat(imageStr, "_GH.pgm");
	printf("imageGradientHorizontal : %s\n", imageGradientHorizontal);

	//imageStr = strext(image);
	char *imageGradientVertical = concat(imageStr, "_GV.pgm");
	printf("imageGradientVertical : %s\n", imageGradientVertical);

	//imageStr = strext(image);
	char *imageNormeGradient = concat(imageStr, "_NG.pgm");
	printf("imageNormeGradient : %s\n", imageNormeGradient);

	//imageStr = strext(image);
	char *imageContour = concat(imageStr, "_IC.pgm");
	printf("imageContour : %s\n", imageContour);

	int NDG[256], R[256], G[256], B[256];
	int i;
	for(i = 0; i < 256; i++) {
        NDG[i] = 0;
        R[i] = 0;
        G[i] = 0;
        B[i] = 0;
        //printf("Case %d : %d\n", i, tab[i]);
	}

	CouleurEnNiveauGris(imageBase, imageNDG);

	passe_bas(imageNDG, imagePasseBas, 1,1,1,1,1,1,1,1,1);
    gradient_horizontal(imageNDG, imageGradientHorizontal);
    gradient_vertical(imageNDG, imageGradientVertical);
    gradient_norme(imageGradientHorizontal, imageGradientVertical, imageNormeGradient);
    getContour(imageNormeGradient, imageContour);

	double moyenneNormeGradient = meanGradiant(imageNormeGradient);
    int nbPixelContour = countPixelContour(imageContour);
    int nbPixel = countPixel(imageContour);
    double* tauxCouleur = taux_couleur(imageBase, R, G, B);
    histogramme(NDG, imageNDG);

    printf("\nFICHIER\n");
    printf("nom:%s\n", imageBase);
    printf("moy:%f\n", moyenneNormeGradient);
    printf("nbPixelContour:%d\n", nbPixelContour);
    printf("Rouge:%.0f;Vert:%.0f;Bleu:%.0f\n", tauxCouleur[0], tauxCouleur[1], tauxCouleur[2]);
    printf("NDG=");
    for(i=0; i<256; i++) {
        printf("%d:%d", i, NDG[i]);
        if(i!=255) printf(";");
    }
    printf("\n");
    printf("R=");
    for(i=0; i<256; i++) {
        printf("%d:%d", i, R[i]);
        if(i!=255) printf(";");
    }
    printf("\n");
    printf("G=");
    for(i=0; i<256; i++) {
        printf("%d:%d", i, G[i]);
        if(i!=255) printf(";");
    }
    printf("\n");
    printf("B=");
    for(i=0; i<256; i++) {
        printf("%d:%d", i, B[i]);
        if(i!=255) printf(";");
    }
    printf("\n");

    write_histogramme(imageBase, moyenneNormeGradient, nbPixel, nbPixelContour, tauxCouleur, NDG, R, G, B);

}

void testDistance(char *image1, char *image2) {

    //IMAGE1
    printf("image1 : %s\n", image1);
    char *imageBase1 = image1;
    printf("imageBase1 : %s\n", imageBase1);

    char *imageStr1 = strext(image1);
    printf("imageStr1 : %s\n", imageStr1);

    char *imageNDG1 = concat(imageStr1, ".pgm");
    printf("imageNDG1 : %s\n", imageNDG1);

	char *imagePasseBas1 = concat(imageStr1, "_PB.pgm");
	printf("imagePasseBas1 : %s\n", imagePasseBas1);

	char *imageGradientHorizontal1 = concat(imageStr1, "_GH.pgm");
	printf("imageGradientHorizontal1 : %s\n", imageGradientHorizontal1);

	char *imageGradientVertical1 = concat(imageStr1, "_GV.pgm");
	printf("imageGradientVertical1 : %s\n", imageGradientVertical1);

	char *imageNormeGradient1 = concat(imageStr1, "_NG.pgm");
	printf("imageNormeGradient1 : %s\n", imageNormeGradient1);

	char *imageContour1 = concat(imageStr1, "_IC.pgm");
	printf("imageContour1 : %s\n", imageContour1);

	int NDG1[256], R1[256], G1[256], B1[256];
	int i1;
	for(i1 = 0; i1 < 256; i1++) {
        NDG1[i1] = 0;
        R1[i1] = 0;
        G1[i1] = 0;
        B1[i1] = 0;
        //printf("Case %d : %d\n", i, tab[i]);
	}

	CouleurEnNiveauGris(imageBase1, imageNDG1);

	passe_bas(imageNDG1, imagePasseBas1, 1,1,1,1,1,1,1,1,1);
    gradient_horizontal(imageNDG1, imageGradientHorizontal1);
    gradient_vertical(imageNDG1, imageGradientVertical1);
    gradient_norme(imageGradientHorizontal1, imageGradientVertical1, imageNormeGradient1);
    getContour(imageNormeGradient1, imageContour1);

	double moyenneNormeGradient1 = meanGradiant(imageNormeGradient1);
    int nbPixelContour1 = countPixelContour(imageContour1);
    int nbPixel1 = countPixel(imageContour1);
    double* tauxCouleur1 = taux_couleur(imageBase1, R1, G1, B1);
    histogramme(NDG1, imageNDG1);
    write_histogramme(imageBase1, moyenneNormeGradient1, nbPixel1, nbPixelContour1, tauxCouleur1, NDG1, R1, G1, B1);

    printf("\n");

    //IMAGE2
    printf("image2 : %s\n", image2);
    char *imageBase2 = image2;
    printf("imageBase2 : %s\n", imageBase2);

    char *imageStr2 = strext(image2);
    printf("imageStr2 : %s\n", imageStr2);

    char *imageNDG2 = concat(imageStr2, ".pgm");
    printf("imageNDG2 : %s\n", imageNDG2);

	char *imagePasseBas2 = concat(imageStr2, "_PB.pgm");
	printf("imagePasseBas2 : %s\n", imagePasseBas2);

	char *imageGradientHorizontal2 = concat(imageStr2, "_GH.pgm");
	printf("imageGradientHorizontal2 : %s\n", imageGradientHorizontal2);

	char *imageGradientVertical2 = concat(imageStr2, "_GV.pgm");
	printf("imageGradientVertical2 : %s\n", imageGradientVertical2);

	char *imageNormeGradient2 = concat(imageStr2, "_NG.pgm");
	printf("imageNormeGradient2 : %s\n", imageNormeGradient2);

	char *imageContour2 = concat(imageStr2, "_IC.pgm");
	printf("imageContour2 : %s\n", imageContour2);

	int NDG2[256], R2[256], G2[256], B2[256];
	int i2;
	for(i2 = 0; i2 < 256; i2++) {
        NDG2[i2] = 0;
        R2[i2] = 0;
        G2[i2] = 0;
        B2[i2] = 0;
        //printf("Case %d : %d\n", i, tab[i]);
	}

	CouleurEnNiveauGris(imageBase2, imageNDG2);

	passe_bas(imageNDG2, imagePasseBas2, 1,1,1,1,1,1,1,1,1);
    gradient_horizontal(imageNDG2, imageGradientHorizontal2);
    gradient_vertical(imageNDG2, imageGradientVertical2);
    gradient_norme(imageGradientHorizontal2, imageGradientVertical2, imageNormeGradient2);
    getContour(imageNormeGradient2, imageContour2);

	double moyenneNormeGradient2 = meanGradiant(imageNormeGradient2);
    int nbPixelContour2 = countPixelContour(imageContour2);
    int nbPixel2 = countPixel(imageContour2);
    double* tauxCouleur2 = taux_couleur(imageBase2, R2, G2, B2);
    histogramme(NDG2, imageNDG2);
    write_histogramme(imageBase2, moyenneNormeGradient2, nbPixel2, nbPixelContour2, tauxCouleur2, NDG2, R2, G2, B2);

    printf("\n");
    distance_histogramme(NDG1, NDG2);

}

int main() {
    //Il y aura qu'a parcourir un tableau avec les noms d'image et executer la fonction derouler.
    char* image = "88.ppm";
    char* imageCopie = "89.ppm";
    //derouler(image);
    testDistance(image, imageCopie);
    return 0;
}
