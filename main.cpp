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
	Mat ellipseBlanche = imread("res/ellipse_blanche.png", CV_16UC3);   // Read the file

	if(! ellipseBlanche.data )                              // Check for invalid input
	{
		std::cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	int rows = (int) ellipseBlanche.rows;
	int cols = (int) ellipseBlanche.cols;

	/* gradient horizontaux et verticaux */
	cv::Mat ellipseBlancheGradH;
	ellipseBlancheGradH = cv::Mat::zeros(rows, cols, CV_16UC3);
	cv::Mat ellipseBlancheGradV;
	ellipseBlancheGradV = cv::Mat::zeros(rows, cols, CV_16UC3);

	toolsTI::gradientH(&ellipseBlanche, &ellipseBlancheGradH);
	toolsTI::gradientV(&ellipseBlanche, &ellipseBlancheGradV);

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




