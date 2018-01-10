#include "imgProcessing.hpp"

void toolsTI::convolution_3x3( cv::Mat* imageDepart,
					  cv::Mat* imageArrivee,
					  cv::Mat* filtre)
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
					somme += filtre->at<uchar>(k+1,l+1) * imageDepart->at<uchar>(i+k,j+l);
				}
			}
			imageArrivee->at<uchar>(i,j) = abs(somme/9);
		}
	}
}

void toolsTI::gradientH( cv::Mat* imageDepart, cv::Mat* imageArrivee  )
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

void toolsTI::gradientV( cv::Mat* imageDepart, cv::Mat* imageArrivee  )
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
