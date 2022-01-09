#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Execution: PixelIntensityColor <input image> <contrast int> <brightness int> <output image>
// This code is truly horrible.  It's OK for sampling techniques but needs to be written better
// and more efficiently for larger projects.

enum class Color { blue=0, green, red };

void WriteFile(string file, const Mat &m)
{
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);

    imwrite("results/" + file, m, compression_params);
}

int main(int argc, char **argv)
{
    if(argc != 6)
    {
        cout << "Invalid number of parameters" << endl;
        return 1;
    }

    string  input_image = argv[1];
    Mat in_image = imread(input_image, IMREAD_UNCHANGED);
    if (in_image.empty())
    {

        cout << "Error! Input image cannot be read...\n";
        return -1;
    }

    Size image_sz = in_image.size();
    Mat out_image = Mat::zeros(image_sz, in_image.type());

    double contrast = strtod(argv[2], NULL);
    double brightness = atoi(argv[3]);

    int rows = in_image.rows;
    int cols = in_image.cols;
    rows = image_sz.height;
    cols = image_sz.width;
    
    int blue {static_cast<int> (Color::blue)};
    int green {static_cast<int> (Color::green)};
    int red {static_cast<int> (Color::red)};
    for(int r {0}; r < rows; ++r)
    {
        for(int c {0}; c < cols; ++c)
        {
             if((r >= 100 && r <= 250) && (c >= 100 && c <= 600))
             {
                 out_image.at<Vec3b>(r,c)[blue] =
                 saturate_cast<uchar>(contrast*(in_image.at<Vec3b>(r,c)[blue]) + brightness);

                 out_image.at<Vec3b>(r,c)[green] =
                 saturate_cast<uchar>(contrast*(in_image.at<Vec3b>(r,c)[green]) + brightness);

                 out_image.at<Vec3b>(r,c)[red] =
                 saturate_cast<uchar>(contrast*(in_image.at<Vec3b>(r,c)[red]) + brightness);
             }
             else
             {
                 out_image.at<Vec3b>(r,c)[blue] = 0.5*saturate_cast<uchar>(in_image.at<Vec3b>(r,c)[blue]);

                 out_image.at<Vec3b>(r,c)[green] = 0.5*saturate_cast<uchar>(in_image.at<Vec3b>(r,c)[green]);

                 out_image.at<Vec3b>(r,c)[red] = 0.5*saturate_cast<uchar>(in_image.at<Vec3b>(r,c)[red]);
             }
        }
    }

    string img_file_out = argv[4];
    if(*argv[5] == 'N')
    {
        Rect rect = Rect(100, 100, 500, 150);
        if(rect.x >= 0 && rect.y >= 0 && 
           rect.width + rect.x < out_image.cols && 
           rect.height + rect.y < out_image.rows)
        {
            Mat spp = out_image.clone();
            Mat crop(spp, rect);
            Mat sp = Mat::zeros(crop.rows, crop.cols, CV_8U);
            randu(sp,0,255);

            Mat black = sp < 40;
            Mat white = sp > 200;

            crop.setTo(255,white);
            crop.setTo(0,black);

            namedWindow("Altered Image with Noise", WINDOW_AUTOSIZE);
            imshow("Altered Image with Noise", spp);
            WriteFile("with_noise_" + img_file_out, spp);
        }
        else
        {
            cout << "ROI size is invalid" << endl;
        }
    }

    namedWindow(input_image, WINDOW_AUTOSIZE);
    imshow(input_image, in_image);

    namedWindow("Altered Image", WINDOW_AUTOSIZE);
    imshow("Altered Image", out_image);

    WriteFile(img_file_out, out_image);

    cout << "Press any key to exit...\n";
    waitKey();

    return 0;
}
