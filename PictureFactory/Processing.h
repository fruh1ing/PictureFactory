#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <QImage>
using namespace cv;

cv::Mat dft_procssing(std::string& imgpath);

QImage cvMat2QImage(const cv::Mat& mat);
