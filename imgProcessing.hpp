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
		void convolution_3x3(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee, cv::Mat_<int>* filtre);
		void gradientH(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee);
		void gradientV(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee);
		void ndgToBinary(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee, int seuil);
		void colorToNDG(cv::Mat_<uchar>* imageDepart, cv::Mat_<uchar>* imageArrivee);
		void directionGradient(cv::Mat_<uchar>* gradientH, cv::Mat_<uchar>* gradientV, float** directionGradient, int rows, int cols);
};

#endif /* IMGPROCESSING_HPP_ */
