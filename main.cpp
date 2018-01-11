/*
 * main.cpp
 *
 *  Created on: 9 janv. 2018
 *      Author: ochtar
 */

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "imgProcessing.hpp"

using namespace cv;

int main(int argc, char** argv)
{
	Mat_<uchar> ellipseBlanche = imread("res/ellipse_blanche.png", CV_8UC3);   // Read the file

	if(! ellipseBlanche.data )                              // Check for invalid input
	{
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	int rows = (int) ellipseBlanche.rows;
	int cols = (int) ellipseBlanche.cols;

	/* passage en niveau de gris et binarisation */
	cv::Mat_<uchar> imgColor;
	imgColor = imread("res/test.jpg", CV_8UC3);
	namedWindow( "color", WINDOW_FREERATIO );// Create a window for display.
	imshow("color", imgColor);
	cv::Mat_<uchar> imgNDG;
	int rowsColor = imgColor.rows;
	int colsColor = imgColor.cols;
	imgNDG = cv::Mat::zeros(rowsColor, colsColor, CV_8UC1);
	namedWindow( "ndg", WINDOW_FREERATIO);
	toolsTI::colorToNDG(&imgColor, &imgNDG);
	imshow("ndg", imgNDG);

	/* gradient horizontaux et verticaux */
	cv::Mat_<uchar> ellipseBlancheGradH;
	ellipseBlancheGradH = cv::Mat::zeros(rows, cols, CV_8UC3);
	toolsTI::gradientH(&ellipseBlanche, &ellipseBlancheGradH);
	cv::Mat_<uchar> ellipseBlancheGradV;
	ellipseBlancheGradV = cv::Mat::zeros(rows, cols, CV_8UC3);
	toolsTI::gradientV(&ellipseBlanche, &ellipseBlancheGradV);

	/* direction de gradient */

	// changer par un tableau a double entrée + dessiner les droites (ds un premier temps)
	// ptet faire gaffe au sens de rotation -> pi/2
	float** dirGradient;
	dirGradient = new float*[rows];
	int i;
	for(i=0;i<rows;i++)
		dirGradient[i] = new float[cols];
	toolsTI::directionGradient(&ellipseBlancheGradH, &ellipseBlancheGradV, dirGradient, rows, cols);



	namedWindow( "ellipseBlanche", WINDOW_AUTOSIZE );// Create a window for display.
	namedWindow( "ellipseBlancheGrad_H", WINDOW_AUTOSIZE);
	namedWindow( "ellipseBlancheGrad_V", WINDOW_AUTOSIZE);

	imshow( "ellipseBlanche", ellipseBlanche );                   // Show our image inside it.
	imshow( "ellipseBlancheGrad_H", ellipseBlancheGradH);
	imshow( "ellipseBlancheGrad_V", ellipseBlancheGradV);

	waitKey(0);                                 // Wait for a keystroke in the window

	/* désallocation mémoire */
	~ellipseBlanche;
	~ellipseBlancheGradH;
	~ellipseBlancheGradV;

	return 0;
}




