#include "imgProcessing.hpp"

void toolsTI::convolution_3x3(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee, cv::Mat_<int>* filtre)
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

					somme += filtre->at<int>(k+1,l+1) * imageDepart->at<uchar>(i+k,j+l);
				}
			}
			//			if(somme != 0)
			//				printf("\n\nPixel(%d,%d), somme : %d\n\n", i,j,somme);
			imageArrivee->at<uchar>(i,j) = abs(somme/9);
		}
	}
}

void toolsTI::gradientH(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee)
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

void toolsTI::gradientV(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee)
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

void toolsTI::ndgToBinary(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee, int seuil)
{
	//source image must be composed of 1 cannal, CV_GRAYSCALE
	if(imageDepart->channels() == 1){
		int i,j;
		for(i=0;i<imageDepart->rows;i++)
		{
			for(j=0;j<imageDepart->cols;j++)
			{
				if(imageDepart->at<cv::Vec3d>(i,j)[0] > seuil)
					imageArrivee->at<cv::Vec3d>(i,j)[0] = 255;
				else
					imageArrivee->at<cv::Vec3d>(i,j)[0] = 0;
			}
		}
	}
}

void toolsTI::colorToNDG(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee)
{
	if(imageDepart->channels() != 3) {
		printf("L'image de départ n'est pas composée de 3 cannaux\n");
		return;
	}
	else if(imageArrivee->channels() != 1){
		printf("L'image d'arrivée n'est pas composée d'un seul cannal\n");
		return;
	}
	else{
		int i,j;
		for(i=0;i<imageDepart->rows;i++)
		{
			for(j=0;j<imageDepart->cols;j++)
			{
				imageArrivee->at<cv::Vec3d>(i,j)[0] = (imageDepart->at<cv::Vec3d>(i,j)[0] + imageDepart->at<cv::Vec3d>(i,j)[1] + imageDepart->at<cv::Vec3d>(i,j)[2]) / 3;
			}
		}
	}
}

void toolsTI::directionGradient(cv::Mat_<uchar>* gradientH, cv::Mat_<uchar>* gradientV, float** directionGradient, int rows, int cols){
	int i,j;
	for(i=0;i<gradientH->rows;i++)
	{
		for(j=0;j<gradientH->cols;j++)
		{

			if(gradientH->at<cv::Vec3d>(i,j)[0] != 0 && gradientV->at<cv::Vec3d>(i,j)[0] != 0) //esquive des division par 0
			{
				printf("testGradientH %d, testGradientV %d\n", gradientH->at<cv::Vec3d>(i,j)[0], gradientV->at<cv::Vec3d>(i,j)[0]);
				directionGradient[i][j] = atan( (float)gradientV->at<cv::Vec3d>(i,j)[0] / (float)gradientH->at<cv::Vec3d>(i,j)[0] ) * 180 / PI;
				printf("gradH(%f) gradV(%f) dirGrad(%d,%d) : %f\n", (float)gradientV->at<cv::Vec3d>(i,j)[0], (float)gradientH->at<cv::Vec3d>(i,j)[0], i, j, directionGradient[i][j]);
			}
		}
	}
}
