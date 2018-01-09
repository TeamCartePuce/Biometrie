/*
 *  Fonctions de traitement d'images
 */


void passe_bas( cv::Mat& imageDepart,
				cv::Mat& imageArrivee,
				int x1,
				int x2,
				int x3,
				int y1,
				int y2,
				int y3,
				int z1,
				int z2,
				int z3)
{
	long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	nch = imageDepart.cols -1 ;
	nrh = imageDepart.rows -1 ;
	nrl = 1;
	ncl = 1;

	cv::Mat_<int> filtre;	//Filtre
	long i, j, k, l;
	int somme;

	imageArrivee = new cv::Mat_<float>(imageDepart.rows, imageDepart.cols);

	//Initialisation de l'image finale
	for(i=nrl;i<=nrh;i++) {
		for(j=ncl;j<=nch;j++) {
			imageArrivee.at<uchar>(i,j) = 0;
		}
	}

	//Initialisation du masque
	filtre = new cv::Mat_<int>(3,3);
	filtre.at<uchar>(0,0) = x1;
	filtre[0][1] = x2;
	filtre[0][2] = x3;
	filtre[1][0] = y1;
	filtre[1][1] = y2;
	filtre[1][2] = y3;
	filtre[2][0] = z1;
	filtre[2][1] = z2;
	filtre[2][2] = z3;

	//Application du masque sur l'image
	for(i=(nrl+1); i<nrh; i++) {
		for(j=(ncl+1);j<nch;j++) {
			somme = 0;
			for(k=-1;k<=1;k++) {
				for(l=-1;l<=1;l++) {
					somme += filtre.at<uchar>(k+1,l+1) * imageDepart.at<uchar>(i+k,j+l);
				}
			}
			imageArrivee.at<uchar>(i,j) = abs(somme/9);
		}
	}


	//Cr�ation de l'image
	SavePGM_bmatrix(R,nrl,nrh,ncl,nch,imageArrivee);

	free_bmatrix(I, nrl, nrh, ncl, nch);
	free_bmatrix(R, nrl, nrh, ncl, nch);
	free_imatrix(filtre, 0, 2, 0, 2);
}
