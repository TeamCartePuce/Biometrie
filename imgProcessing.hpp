/*
 * imgProcessing.hpp
 *
 *  Created on: 9 janv. 2018
 *      Author: Florian C.
 */

#ifndef IMGPROCESSING_HPP_
#define IMGPROCESSING_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cmath>

#define PI 3.14159265

namespace toolsTI {

		/* fonctions de conversion */
		void convertUCharToNDGInt(cv::Mat_<uchar>* imageDepart, cv::Mat_<int>* imageArrivee);
		void convertIntToUCharToDisplay(cv::Mat_<int>* imageDepart, cv::Mat_<uchar>* imageDisplayed, int rows, int cols);

		/* convolution & gradient */
		void convolution_3x3(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee, cv::Mat_<int>* filtre);
		void gradientH(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee);
		void gradientV(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee);

		/* binarization */
		void ndgToBinary(cv::Mat_<int>* imageDepart, cv::Mat_<int>* imageArrivee, int seuil);
//		void colorToNDG(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee);

		/* direction du gradient */
		void directionGradient(cv::Mat_<int>* gradientH, cv::Mat_<int>* gradientV, float** directionGradient, int rows, int cols);
};

#endif /* IMGPROCESSING_HPP_ */
