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

#include "simplifier/Simplifier.h"

using shape_simplification::simplifier::Simplifier;

bool _silent;

void printTime(clock_t &time, std::string message);

void displayUsage()
{
    std::cout << "Lexis:\n";
    std::cout << "* width: width to be used for the boxes decomposing the stacked image.\n";
    std::cout << "* maxcc: maximal size of a component to be completely removed.\n"
                 "  Give a negative value if no limit is wanted.\n";
    std::cout << "* nberim: number of images to load in the stacked image.\n";
    std::cout << "* in_fp: name prefix of the input images, if existing. \n"
                 "  E.g. \"flower_\" if the files are named \"flower_xxxx.png\", \n"
                 "  where xxxx has to be the numbering of the images with 4 digits.\n";
    std::cout << "* in_dp: directory path where the input images are stored.\n";
    std::cout << "* out_fp: name prefix for the output images.\n";
    std::cout << "* out_dp: directory path where the output images should be stored.\n";
    std::cout << "* min_cc: Lowest number of components possible. Default value: 1.\n";
    std::cout << "* min_l: Lowest number of loops possible. Default value: 0.\n";
    std::cout << "* min_v: Lowest number of voids possible. Default value: 0.\n";
    std::cout << "* ratio: Maximum % of noise to remove (Value between 0 and 100). "
                 "Default value: 100.\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << " -s \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#Standard simplifier\n";
    std::cout << " -swg \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#Standard simplifier with guarantee\n";
    std::cout << " -f \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#Standard simplifier with Substraction first\n";
    std::cout << " -o \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#With candidates ordered by increasing length\n";
    std::cout << " -a \twidth nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t\t "
                 "#Additions only\n";
    std::cout << " -awg \twidth nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t\t "
                 "#Additions only with guarantee\n";
    std::cout << " -r \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#Removals only\n";
    std::cout << " -rwg \twidth maxcc nberim inf_dp/in_fp out_dp[/out_fp] "
                 "[min_cc min_l min_v [ratio]]\t "
                 "#Removals only with guarantee\n";
    std::cout << "\n";
    std::cout << "Additional options:\n";
    std::cout << "  --silent: avoids output in the terminal.\n";
    std::cout << "\nFor more details, see Readme file.\n";
    std::cout << "\n";
}

int main(int argc, char **argv) {
    if (argc < 6 || argc > 13){
        displayUsage();
        return 0;
    }

    std::vector<char*> args;
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--silent") == 0) _silent = true;
        else args.push_back(argv[i]);
    }

    Simplifier simplifier(_silent);
    unsigned int boxWidth;
    int ccSize;
    unsigned int numberOfImages;
    std::string inputFileNameBase;
    std::string outputFileNameBase;
    unsigned int minNumberOfComponents = 1;
    unsigned int minNumberOfLoops = 0;
    unsigned int minNumberOfVoids = 0;
    double minRatio = 100.0;

    if (args.size() > 6){
        if (args.size() > 10 && args.size() < 8){
            displayUsage();
            return 0;
        }

        if (args.size() == 10 ||
                (args.size() == 9 && strcmp(args.at(0), "-a") == 0) ||
                (args.size() == 9 && strcmp(args.at(0), "-awg") == 0)){
            minRatio = atoi(args.back());
            args.pop_back();
        }

        minNumberOfVoids = atoi(args.back());
        args.pop_back();
        minNumberOfLoops = atoi(args.back());
        args.pop_back();
        minNumberOfComponents = atoi(args.back());
        args.pop_back();
    }

    clock_t totalTime;
    clock_t time;

    if (strcmp(args.at(0), "-a") == 0 || strcmp(args.at(0), "-awg") == 0){
        if (args.size() != 5){
            displayUsage();
            return 0;
        }
        boxWidth = atoi(args.at(1));
        numberOfImages = atoi(args.at(2));
        inputFileNameBase = args.at(3);
        outputFileNameBase = args.at(4);
    } else {
        if (args.size() != 6){
            displayUsage();
            return 0;
        }
        boxWidth = atoi(args.at(1));
        ccSize = atoi(args.at(2));
        numberOfImages = atoi(args.at(3));
        inputFileNameBase = args.at(4);
        outputFileNameBase = args.at(5);
    }

    totalTime = clock();
    time = clock();
    simplifier.loadPreprocessedImage(inputFileNameBase,
                                     numberOfImages,
                                     minNumberOfComponents,
                                     minNumberOfLoops,
                                     minNumberOfVoids,
                                     minRatio);
    printTime(time, "Image load time: ");

    if (strcmp(args.at(0), "-s") == 0) {
        time = clock();
        simplifier.simplify(boxWidth, ccSize);
        printTime(time, "Process time: ");
    } else if (strcmp(args.at(0), "-f") == 0) {
        time = clock();
        simplifier.simplifyWithSubFirst(boxWidth, ccSize);
        printTime(time, "Process time: ");
    } else if (strcmp(args.at(0), "-a") == 0) {
        time = clock();
        simplifier.simplifyOnlyAdditively(boxWidth);
        printTime(time, "Process time: ");
    } else if (strcmp(args.at(0), "-r") == 0) {
        time = clock();
        simplifier.simplifyOnlySubstractively(boxWidth, ccSize);
        printTime(time, "Process time: ");
    } else {
        displayUsage();
        return 0;
    }

    time = clock();
    simplifier.saveImage(outputFileNameBase);
    printTime(time, "Simplification save time: ");

    printTime(totalTime, "Total elapsed time: ");

    return 0;
}

void printTime(clock_t &time, std::string message){
    if (_silent) return;

    std::cout << message << " " << std::setprecision(10)
              << (static_cast<float>(clock() - time)) / CLOCKS_PER_SEC
              << " seconds.\n";
}








