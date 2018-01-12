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
	cv::Mat_<int> gradientSeuille = cv::Mat_<int>(rows, cols);
	toolsTI::calculGradientSeuille(&gradientH, &gradientV, &gradientSeuille);

	/* direction de gradient */

	// changer par un tableau a double entrée + dessiner les droites (ds un premier temps)
	// ptet faire gaffe au sens de rotation -> pi/2
	float** dirGradient;
	dirGradient = new float*[rows];
	int i,j;
	for(i=0;i<rows;i++)
		dirGradient[i] = new float[cols];
	toolsTI::directionGradient(&gradientH, &gradientV, dirGradient, rows, cols);

	namedWindow( "ellipseBlanche", WINDOW_AUTOSIZE );// Create a window for display.
	imshow("ellipseBlanche", imageColor );

	namedWindow( "ellipseNdg", WINDOW_AUTOSIZE);
	imshow("ellipseNdg", imageNDG);

	cv::Mat_<uchar> imageGradientH = cv::Mat_<uchar>(rows,cols);
	toolsTI::convertIntToUCharToDisplay( &gradientH, &imageGradientH, rows, cols);
	namedWindow("gradientH", WINDOW_AUTOSIZE);
	imshow("gradientH", imageGradientH);

	cv::Mat_<uchar> imageGradientV = cv::Mat_<uchar>(rows,cols);
	toolsTI::convertIntToUCharToDisplay( &gradientV, &imageGradientV, rows, cols);
	namedWindow("gradientV", WINDOW_AUTOSIZE);
	imshow("gradientV", imageGradientV);

	cv::Mat_<uchar> imageGradient = cv::Mat_<uchar>(rows,cols);
	toolsTI::convertIntToUCharToDisplay( &gradientSeuille, &imageGradient, rows, cols);


	/* draw gradient direction on gradient image */
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			if(gradientSeuille.at<int>(i,j) > SEUIL_GRADIENT)
			{

				cv::Point p1 = cv::Point(j,i);
				cv::Point p2 = cv::Point();
				p2.x = gradientSeuille.at<int>(i,j)*cos(dirGradient[i][j]) + p1.x;
				p2.y = gradientSeuille.at<int>(i,j)*sin(dirGradient[i][j]) + p1.y;
				cv::line(imageGradient, p1, p2, Scalar( 255, 0, 255 ),1,LINE_AA,0);
			}
		}
	}

	namedWindow("gradient", WINDOW_AUTOSIZE);
	imshow("gradient", imageGradient);

	waitKey(0);                                 // Wait for a keystroke in the window

	/* désallocation mémoire */
	~imageColor;
	~imageNDG;
	~gradientH;
	~gradientV;
	delete [] dirGradient;
	~imageGradientH;
	~imageGradientV;
	~imageGradient;

	return 0;
}




