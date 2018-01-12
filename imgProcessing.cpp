#include "imgProcessing.hpp"

void toolsTI::convertUCharToNDGInt(cv::Mat_<uchar>* imageDepart, cv::Mat_<int>* imageArrivee){
	int i,j,k;
	printf("Nb Channels = %d\n", imageDepart->channels());
	for(i=0;i<imageDepart->rows;i++){
		for(j=0;j<imageDepart->cols;j++){
			if(imageDepart->channels() == 3){
				imageArrivee->at<int>(i,j) = ((int)imageDepart->at<cv::Vec3d>(i,j)[0] + (int)imageDepart->at<cv::Vec3d>(i,j)[1] + (int)imageDepart->at<cv::Vec3d>(i,j)[2])/3;
			}
			else if(imageDepart->channels() == 1){
				imageArrivee->at<int>(i,j) = (int)imageDepart->at<uchar>(i,j);
			}
		}
	}
}

void toolsTI::convertIntToUCharToDisplay(cv::Mat_<int>* imageDepart, cv::Mat_<uchar>* imageDisplayed, int rows, int cols){
	int i,j,k;
		for(i=0;i<rows;i++){
			for(j=0;j<cols;j++){
					imageDepart->at<int>(i,j) = abs(imageDepart->at<int>(i,j));
					if(imageDepart->at<int>(i,j) > 255)
						imageDisplayed->at<uchar>(i,j) = 255;
					else if(imageDepart->at<int>(i,j) < 0)
						imageDisplayed->at<uchar>(i,j) = 0;
					else{
						imageDisplayed->at<uchar>(i,j) = imageDepart->at<int>(i,j);
					}
			}
		}
}

void toolsTI::convolution_3x3(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee, cv::Mat_<int>* filtre)
{
	long nrh, nrl, nch, ncl; //max_row, min_row, max_column, min_column
	nch = imageDepart->cols -1 ;
	nrh = imageDepart->rows -1 ;
	nrl = 1;
	ncl = 1;

	long i, j, k, l;
	int somme;

	//Application du masque sur l'image
	for(i=nrl; i<nrh; i++) {
		for(j=ncl+1;j<nch;j++) {
			somme = 0;
			for(k=-1;k<=1;k++) {
				for(l=-1;l<=1;l++) {
					//					if(filtre->at<int>(k+1,l+1) * imageDepart->at<uchar>(i+k,j+l) != 0)
					//					{
					//						printf("Florian : iD %d\t", imageDepart->at<uchar>(i+k,j+l) );
					//						printf("filtre %d\t", filtre->at<int>(k+1,l+1) );
					//					}

					somme += filtre->at<int>(k+1,l+1) * imageDepart->at<int>(i+k,j+l);
				}
			}
			//			if(somme != 0)
			//				printf("\n\nPixel(%d,%d), somme : %d\n\n", i,j,somme);
			imageArrivee->at<int>(i,j) = somme/9;
		}
	}
}

void toolsTI::gradientH(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee)
{
	cv::Mat_<int> sobelH = cv::Mat_<int>(3,3);
	sobelH.at<int>(0,0) = -1;
	sobelH.at<int>(0,1) = 0;
	sobelH.at<int>(0,2) = 1;
	sobelH.at<int>(1,0) = -2;
	sobelH.at<int>(1,1) = 0;
	sobelH.at<int>(1,2) = 2;
	sobelH.at<int>(2,0) = -1;
	sobelH.at<int>(2,1) = 0;
	sobelH.at<int>(2,2) = 1;

	convolution_3x3(imageDepart, imageArrivee, &sobelH);

	~sobelH;
}

void toolsTI::gradientV(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee)
{
	cv::Mat_<int> sobelV = cv::Mat_<int>(3,3);
	sobelV.at<int>(0,0) = 1;
	sobelV.at<int>(0,1) = 2;
	sobelV.at<int>(0,2) = 1;
	sobelV.at<int>(1,0) = 0;
	sobelV.at<int>(1,1) = 0;
	sobelV.at<int>(1,2) = 0;
	sobelV.at<int>(2,0) = -1;
	sobelV.at<int>(2,1) = -2;
	sobelV.at<int>(2,2) = -1;

	convolution_3x3(imageDepart, imageArrivee, &sobelV);

	~sobelV;
}

void toolsTI::calculGradientSeuille(cv::Mat_<int>* gradientH, cv::Mat_<int>* gradientV, cv::Mat_<int>* gradientSeuille){
	int i,j;
	for(i=0;i<gradientH->rows;i++){
		for(j=0;j<gradientH->cols;j++){
			gradientSeuille->at<int>(i,j) = sqrt( (double)(gradientH->at<int>(i,j)*gradientH->at<int>(i,j)) + ((double)(gradientV->at<int>(i,j)*gradientV->at<int>(i,j))) );
			printf("gradientSeuille(%d,%d) = %f\n", i, j, gradientSeuille->at<int>(i,j));
		}
	}
}

void toolsTI::ndgToBinary(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee, int seuil)
{
	if(imageDepart->channels() == 1){
		int i,j;
		for(i=0;i<imageDepart->rows;i++)
		{
			for(j=0;j<imageDepart->cols;j++)
			{
				if(imageDepart->at<int>(i,j) > seuil)
					imageArrivee->at<int>(i,j) = 255;
				else
					imageArrivee->at<int>(i,j) = 0;
			}
		}
	}
	else{
		printf("source image must be composed of 1 cannal, CV_GRAYSCALE\n");
	}
}

//void toolsTI::colorToNDG(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee)
//{
//	if(imageDepart->channels() != 3) {
//		printf("L'image de départ n'est pas composée de 3 cannaux\n");
//		return;
//	}
//	else if(imageArrivee->channels() != 1){
//		printf("L'image d'arrivée n'est pas composée d'un seul cannal\n");
//		return;
//	}
//	else{
//		cv::Mat_<int> imgDepart = cv::Mat_<int>(imageDepart->rows, imageDepart->cols);
//		cv::Mat_<int> imgDepart = cv::Mat_<int>(imageDepart->rows, imageDepart->cols);
//		int i,j;
//		for(i=0;i<imageDepart->rows;i++)
//		{
//			for(j=0;j<imageDepart->cols;j++)
//			{
//				imageDepart
//				imageArrivee->at<cv::Vec3d>(i,j)[0] = ((int)imageDepart->at<cv::Vec3d>(i,j)[0] + (int)imageDepart->at<cv::Vec3d>(i,j)[1] + (int)imageDepart->at<cv::Vec3d>(i,j)[2]) / 3;
//			}
//		}
//	}
//}

void toolsTI::directionGradient(cv::Mat_<int>* gradientH, cv::Mat_<int>* gradientV, float** directionGradient, int rows, int cols){
	int i,j;
	for(i=0;i<gradientH->rows;i++)
	{
		for(j=0;j<gradientH->cols;j++)
		{
			if(gradientH->at<int>(i,j) != 0) //esquive des division par 0
			{
				directionGradient[i][j] = (atan( (float)gradientV->at<int>(i,j) / (float)gradientH->at<int>(i,j)) - PI/2);
				printf("gradH(%f) gradV(%f) dirGrad(%d,%d) : %f\n", (float)gradientV->at<int>(i,j), (float)gradientH->at<int>(i,j), i, j, directionGradient[i][j]*180/PI);
			}
		}
	}
}


