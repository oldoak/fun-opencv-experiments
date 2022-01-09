#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace cv;

enum class Color { blue=0 , green, red};
enum class Coins { five_pence=5, one_pence=1, ten_pence=10, two_pence=2, one_pound=100, two_pound=200 };

void ColorPixel
(const int bgr, Point middle, const int rad, Mat &in)
{
    const int blue {static_cast<int> (Color::blue)};
    const int green {static_cast<int> (Color::green)};
    const int red {static_cast<int> (Color::red)};
    const int all {3};

    switch(bgr)
    {
        case (blue):
            for(int r {middle.y}; r > middle.y-rad; --r)
            {
                for(int c {middle.x}; c < middle.x+rad; ++c)
                {
                    in.at<Vec3b>(r,c)[blue] =
                    saturate_cast<uchar>(in.at<Vec3b>(r,c)[blue] + 100);
                }
            }
            break;

        case (green):
            for(int r {middle.y}; r < middle.y+rad; ++r)
            {
                for(int c {middle.x}; c < middle.x+rad; ++c)
                {
                    in.at<Vec3b>(r,c)[green] =
                    saturate_cast<uchar>(in.at<Vec3b>(r,c)[green] + 100);
                }
            }
            break;

        case (red):
            for(int r {middle.y}; r < middle.y+rad; ++r)
            {
                for(int c {middle.x}; c > middle.x-rad; --c)
                {
                    in.at<Vec3b>(r,c)[red] =
                    saturate_cast<uchar>(in.at<Vec3b>(r,c)[red] + 100);
                }
            }
            break;

        case (all):
            for(int r {middle.y}; r > middle.y-rad; --r)
            {
                for(int c {middle.x}; c > middle.x-rad; --c)
                {
                    for(int p {0}; p < 3; ++p)
                    {
                        in.at<Vec3b>(r,c)[p] =
                        saturate_cast<uchar>(in.at<Vec3b>(r,c)[p] + 100);
                    }
                }
            }
            break;
    }
}

void MarkQuarters
(Mat &InputImage, const vector<Vec3f> &circles, vector<int> &coin_radii)
{
    for(size_t i = 0; i < circles.size(); ++i)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(InputImage, center, 3, Scalar(0,255,0), -1, 8, 0); // Center of circle
        circle(InputImage, center, radius, Scalar(0,0,255), 3, 8, 0); // Circumference 

        if((find(coin_radii.begin(), coin_radii.end(), radius)) == coin_radii.end())
        {
            coin_radii.push_back(radius);
        }
        
        int blue {static_cast<int> (Color::blue)};
        int green {static_cast<int> (Color::green)};
        int red {static_cast<int> (Color::red)};

        ColorPixel(blue, center, radius, InputImage);
        ColorPixel(green, center, radius, InputImage);
        ColorPixel(red, center, radius, InputImage);
        ColorPixel(3, center, radius, InputImage);
    }
}

int main
(int argc, char **argv)
{
    if(argc != 3)
    {
        cout << "Invalid number of parameters" << endl;
        return 1;
    }

    string image_file = argv[1];
    Mat InImage = imread(image_file.c_str());
    if (InImage.empty())
    {
        cout << "Error! Input image cannot be read...\n";
        return -1;
    }

    namedWindow("Original Image", CV_NORMAL);
    imshow("Original Image", InImage);

    Mat GrayImage;
    cvtColor(InImage, GrayImage, CV_BGR2GRAY);

    GaussianBlur(GrayImage, GrayImage, Size(9, 9), 2, 2); // Smooth of image

    vector<Vec3f> circles;

    // Hough Transform for circle detection
    HoughCircles(GrayImage, 
                 circles, 
                 CV_HOUGH_GRADIENT, 
                 2, 
                 GrayImage.rows/8,
                 100, 
                 90,
                 0, 
                 0);

    vector<int> coin_sizes;
    MarkQuarters(InImage, circles, coin_sizes);
    sort(coin_sizes.begin(), coin_sizes.end());

    vector<Coins> vc {Coins::five_pence, 
                      Coins::one_pence, 
                      Coins::ten_pence, 
                      Coins::two_pence, 
                      Coins::one_pound, 
                      Coins::two_pound};
    float sum {0};
    for(int i {0}; i < 7; ++i)
    {
        sum += static_cast<float> (vc[i]); 
    }
    cout << "£" << sum/100 << endl;
    
    namedWindow("Circular Coins Detected", CV_NORMAL);
    imshow("Circular Coins Detected", InImage);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(99);
    
    //string img_file_out = argv[2];
    //imwrite(img_file_out, InImage, compression_params);

    waitKey();

    destroyAllWindows();

    return 0;
}
