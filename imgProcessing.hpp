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

namespace toolsTI {
		void convolution_3x3(cv::Mat* imageDepart, cv::Mat* imageArrivee, cv::Mat* filtre);
		void gradientH(cv::Mat* imageDepart, cv::Mat* imageArrivee);
		void gradientV(cv::Mat* imageDepart, cv::Mat* imageArrivee);
};

#endif /* IMGPROCESSING_HPP_ */
