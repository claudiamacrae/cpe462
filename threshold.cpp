// threshold.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Usage: thresholding inputImage.jpg
// Claudia MacRae

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
	//FILE* in, * out;
	int   j, k, width, height;
	int** image_in, ** image_out;
	float sum1, sum2;
	float new_T, old_T, delta_T;
	long count1, count2;
	//errno_t err;


	/****************************/
	/* Some OpenCV here */
	/********************/

	Mat M_in = imread(argv[1]);
	Mat_<uchar> M_in_g(M_in.rows, M_in.cols);
	cvtColor(M_in, M_in_g, COLOR_BGR2GRAY);
	//cout << "height" << M_in_g.rows << endl;

	String windowName1 = "Input Image"; //Name of the window

	namedWindow(windowName1); // Create a window

	imshow(windowName1, M_in_g); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	//destroyWindow(windowName1); //destroy the created window

	height = M_in_g.rows;
	width = M_in_g.cols;

	/********************************************************************/

	image_in = (int**)calloc(height, sizeof(int*));
	if (!image_in) {
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	image_out = (int**)calloc(height, sizeof(int*));
	if (!image_out) {
		printf("Error: Can't allocate memmory!\n");
		return(1);
	}

	for (j = 0; j < height; j++) {
		image_in[j] = (int*)calloc(width, sizeof(int));

		if (!image_in[j]) {
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

		image_out[j] = (int*)calloc(width, sizeof(int));
		if (!image_out[j]) {
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

	}

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			image_in[j][k] = M_in_g(j, k);
		}
	}

	/********************************************************************/
	/* Image Processing begins                                          */
	/********************************************************************/

	new_T = 200.0;
	delta_T = 50.0;

	while (delta_T > 5 || delta_T < -5) {
		count1 = 0;
		count2 = 0;
		sum1 = 0;
		sum2 = 0;

		for (j = 0; j < height; j++) {
			for (k = 0; k < width; k++) {
				//if pixel of input image is greater than the threshold, increment 1
				if (image_in[j][k] >= new_T) {
					sum1 += image_in[j][k];
					count1++;
				}//if it is less than T, increment 2
				else {
					sum2 += image_in[j][k];
					count2++;
				}
			}
		}
		//save current T and calculate new value and delta
		old_T = new_T;
		new_T = ((sum1 / count1) + (sum2 / count2)) / 2;
		delta_T = new_T - old_T;
		printf("New Threshold = % f \n", new_T);	//output threshold

		//Thresholding
		for (j = 0; j < height; j++) {
			for (k = 0; k < width; k++) {
				//using new threshold, all pixels above are made black and those below, white
				image_out[j][k] = (image_in[j][k] >= new_T ? 255:0);
			}
		}
	}

	

	/********************************************************************/
	/* Image Processing ends                                          */
	/********************************************************************/

	/****************************/
	/* Some OpenCV here */
	/********************/

	Mat_<uchar> M_out(height, width);

	for (int ii = 0; ii < height; ii++) {
		for (int jj = 0; jj < width; jj++) {
			M_out(ii, jj) = image_out[ii][jj];
		}
	}

	String windowName2 = "Output Image"; //Name of the window

	namedWindow(windowName2); // Create a window

	imshow(windowName2, M_out); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName2); //destroy the created window


	bool isSuccess = imwrite(argv[2], M_out); //write the image to a file as JPEG
	//bool isSuccess = imwrite("MyOutputImage.png", M_out); //write the image to a file as PNG
	if (isSuccess == false) {
		cout << "Failed to save the image" << endl;
		//cin.get(); //wait for a key press
		return -1;
	}


	/***********************/

	for (j = 0; j < height; j++) {
		free(image_in[j]);
		free(image_out[j]);
	}
	free(image_in);
	free(image_out);

	return 0;
}
