#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Execution: PixelIntensityBW <input image> <intensity int> <output image>

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        cout << "Invalid number of parameters" << endl;
        return 1;
    }

    Mat in_image, out_image;

    in_image = imread(argv[1], IMREAD_UNCHANGED);
    if (in_image.empty())
    {

        cout << "Error! Input image cannot be read...\n";
        return -1;
    }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], in_image);

    cvtColor(in_image, out_image, COLOR_BGR2GRAY);

    Size image_sz = in_image.size();
    int rows = in_image.rows;
    int cols = in_image.cols;
    rows = image_sz.height;
    cols = image_sz.width;
    
    float intense = atoi(argv[2]);
    for(int r {0}; r < rows; ++r)
    {
        for(int c {0}; c < cols; ++c)
        {
            out_image.at<uchar>(r, c) /= intense;
        }
    }

    namedWindow("Altered Image", WINDOW_AUTOSIZE);
    imshow("Altered Image", out_image);

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(99);
    
    std::string img_file_out = argv[3];
    imwrite(img_file_out, out_image, compression_params);
    cout << "Press any key to exit...\n";
    waitKey();

    return 0;
}
