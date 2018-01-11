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
	Mat_<uchar> imageColor = imread("res/ellipse_blanche.jpg", CV_8UC3);   // Read the file
	Mat_<int> imageNDG = Mat_<int>(imageColor.rows,imageColor.cols);
	toolsTI::convertUCharToNDGInt(&imageColor, &imageNDG);

	if(! imageColor.data )                              // Check for invalid input
	{
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	int rows = (int) imageColor.rows;
	int cols = (int) imageColor.cols;

	/* gradient horizontaux et verticaux */
	cv::Mat_<int> gradientH = cv::Mat_<int>(rows, cols);
	toolsTI::gradientH(&imageNDG, &gradientH);
	cv::Mat_<int> gradientV = cv::Mat_<int>(rows, cols);
	toolsTI::gradientV(&imageNDG, &gradientV);

	/* direction de gradient */

	// changer par un tableau a double entrée + dessiner les droites (ds un premier temps)
	// ptet faire gaffe au sens de rotation -> pi/2
	float** dirGradient;
	dirGradient = new float*[rows];
	int i;
	for(i=0;i<rows;i++)
		dirGradient[i] = new float[cols];
	toolsTI::directionGradient(&gradientH, &gradientV, dirGradient, rows, cols);



	namedWindow( "ellipseBlanche", WINDOW_AUTOSIZE );// Create a window for display.
	namedWindow( "ellipseNdg", WINDOW_AUTOSIZE);


	imshow( "ellipseBlanche", imageColor );
	imshow( "ellipseNdg", imageNDG);

	waitKey(0);                                 // Wait for a keystroke in the window

	/* désallocation mémoire */
	~imageColor;
	~imageNDG;
	~gradientH;
	~gradientV;
	delete [] dirGradient;

	return 0;
}




