/*
 * Copyright 2020 St. Louis University
 * Contributed by: Hannah Schreiber
 *
 * This file is part of VHS.
 *
 * VHS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VHS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with VHS. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <algorithm>

#include "util/BaseImageStack.h"
#include "util/RawImageStack.h"

using shape_simplification::util::BaseImageStack;
using shape_simplification::util::RawImageStack;

clock_t _time;
bool _silent;
unsigned int _translation;

void printMessage(std::string message);

void resetTimer();
void printTime(std::string message);
void printAndResetTime(std::string message);

void operator<<(std::string baseFilename, BaseImageStack &outputImages);

void displayUsage()
{
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "  ./imgproc down noise nberim inf_dp/in_fp out_dp[/out_fp] [trans]\n";
    std::cout << "\n";
    std::cout << "Adding the argument --silent avoid outputs in the terminal.\n";
    std::cout << "\n";
    std::cout << "Lexis:\n";
    std::cout << "* down: Downsampling value.\n";
    std::cout << "* noise: Maximal noise value (unsigned integer value)\n"
                 "  added to the gray level value.\n";
    std::cout << "* nberim: number of images to process.\n";
    std::cout << "* in_fp: name prefix of the input images, if existing. \n"
                 "  E.g. \"flower_\" if the files are named \"flower_xxxx.png\", \n"
                 "  where xxxx has to be the numbering of the images with 4 digits.\n";
    std::cout << "* in_dp: directory path where the input images are stored.\n";
    std::cout << "* out_fp: name prefix for the output images.\n";
    std::cout << "* out_dp: directory path where the output images should be stored.\n";
    std::cout << "* trans: translates the grayscale level value by the given amount.\n";
    std::cout << "\nfor more details, see Readme file\n";
    std::cout << "\n";
}

int main(int argc, char **argv) {
    if (argc < 6 || argc > 8){
        displayUsage();
        return 0;
    }

    std::vector<char*> args;
    _silent = false;
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--silent") == 0) _silent = true;
        else args.push_back(argv[i]);
    }
    if (args.size() < 5 || args.size() > 6){
        displayUsage();
        return 0;
    }

    unsigned int downsampling = atoi(args.at(0));
    unsigned int noise = atoi(args.at(1));
    unsigned int numberOfImages = atoi(args.at(2));
    std::string inputFileNameBase(args.at(3));
    std::string outputFileNameBase(args.at(4));
    _translation = 0;
    if (args.size() == 6) _translation = atoi(args.at(5));

    RawImageStack raw(downsampling, noise);

    resetTimer();
    printMessage("--Load image.");

    raw.load(inputFileNameBase, numberOfImages);

    printAndResetTime("Image load time:");
    printMessage("--Write image.");

    outputFileNameBase << raw;

    printTime("Image writing time:");

    return 0;
}

void printMessage(std::string message){
    if (_silent) return;

    std::cout << message << std::endl;
}

void resetTimer(){
    if (_silent) return;

    _time = clock();
}

void printTime(std::string message){
    if (_silent) return;

    std::cout << message << " " << std::setprecision(10)
              << (static_cast<float>(clock() - _time)) / CLOCKS_PER_SEC
              << " seconds." << std::endl;
}

void printAndResetTime(std::string message){
    if (_silent) return;

    printTime(message);
    resetTimer();
}

/**
 * @brief Saves an image stack as a set of PNG images.
 * @param baseFilename Directory the images should be saved in.
 * @param outputImages Image stack to save.
 */
void operator<<(std::string baseFilename, BaseImageStack &outputImages) {
    const std::array<unsigned int, 3> &dimensions = outputImages.getDimensions();
    unsigned int numberOfImages = dimensions[2];

    for (unsigned int i = 0; i < numberOfImages; i++) {
        std::string filename = baseFilename;
        if (i < 10)
            filename += "000";
        else if (i < 100)
            filename += "00";
        else if (i < 1000)
            filename += "0";
        filename += std::to_string(i) + ".png";

        png::image<png::gray_pixel> image(dimensions[0], dimensions[1]);
        unsigned int p[3] = {0, 0, i};
        for (p[1] = 0; p[1] < dimensions[1]; p[1]++)
            for (p[0] = 0; p[0] < dimensions[0]; p[0]++) {
                if (outputImages.getLevel(p[0], p[1], p[2]) < 10){
                    image[p[1]][p[0]] = outputImages.getLevel(p[0], p[1], p[2]);
                } else {
                    image[p[1]][p[0]] =
                            std::min(
                                outputImages.getLevel(p[0], p[1], p[2]) + _translation,
                            255U
                            );
                }

            }

        image.write(filename);
    }
}
