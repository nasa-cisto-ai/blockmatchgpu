#include <iostream>
#include "BlockMatcherGPU.h" 
#include <nvToolsExt.h>
#include <nvToolsExtCuda.h>
#include <string>
#include <vector>
#include <gdal.h>
#include <gdal_priv.h>


int main(int argc, char** argv) {

    int block_size = 11;
    int search_range = 20;

    int rows = 1000;
    int cols = 1000;

    // Create instances of the left and right image data (dummy data)
    std::vector<double> left_image(rows * cols, 0);
    std::vector<double> right_image(rows * cols, 0);

    // Populate the dummy data (e.g., random values for demonstration)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            left_image[i * cols + j] = static_cast<double>(rand() % 256);  // Random values between 0 and 255
            right_image[i * cols + j] = static_cast<double>(rand() % 256);
        }
    }

    /*
    // Adding gdal components
    GDALAllRegister(); // Register GDAL drivers

    // set filename variables
    const char* leftImagePath;
    const char* rightImagePath;

    // if filename is specified, read that filename
	if (argc > 1) {
		leftImagePath = argv[1];
        rightImagePath = argv[2];
	}

    // Open the left and right images using GDAL
    GDALDataset *leftImageDataset = static_cast<GDALDataset *>(GDALOpen("/nobackup/jacaraba/development/out_stereo/run-L.tif", GA_ReadOnly));
    GDALDataset *rightImageDataset = static_cast<GDALDataset *>(GDALOpen("/nobackup/jacaraba/development/out_stereo/run-R.tif", GA_ReadOnly));

    if (!leftImageDataset || !rightImageDataset)
    {
        std::cerr << "Error: Unable to open input images using GDAL." << std::endl;
        GDALClose(leftImageDataset);
        GDALClose(rightImageDataset);
        return 1;
    }

    // get number of columns and rows, left and right images have the same size
    int cols = leftImageDataset->GetRasterXSize();
    int rows = leftImageDataset->GetRasterYSize();

    // output some of the information
    std::cout << "ROWS" << " " << rows << " \n";
    std::cout << "COLS" << " " << cols << " \n";

    // define vectors
    std::vector<double> left_image(rows*cols);
    std::vector<double> right_image(rows*cols);

    // read the imagery and load the data into the vector dataset
    leftImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, left_image.data(), cols, rows, GDT_Float32, 0, 0);
    rightImageDataset->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, cols, rows, right_image.data(), cols, rows, GDT_Float32, 0, 0);
    */

    // Create a BlockMatcherGPU instance
    BlockMatcherGPU blockMatchergpu(rows, cols, block_size, search_range);

    // Compute the disparity map using the dummy data
    //blockMatchergpu.compute_disparity(left_image, right_image);
    //blockMatchergpu.compute_disparity_gpu(left_image, right_image);
    blockMatchergpu.compute_disparity_gpu_v2(left_image, right_image);


    // Access the disparity map (disparity values are stored in disparityProcessor.disparity_map)
    std::vector<double> disparity_map = blockMatchergpu.disparity_map;

    //GDALClose(leftImageDataset);
    //GDALClose(rightImageDataset);
   
    return 0;
}