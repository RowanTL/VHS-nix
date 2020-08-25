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
    std::cout << "\n";
    std::cout << "Lexis:\n";
    std::cout << "> nberim: number of images to load in the stacked image.\n";
    std::cout << "> in_fp: name prefix of the input images, if existing. \n"
                 "  E.g. \"flower_\" if the files are named \"flower_xxxx.png\", \n"
                 "  where xxxx has to be the numbering of the images with 4 digits.\n";
    std::cout << "> in_dp: directory path where the input images are stored.\n";
    std::cout << "> out_fp: name prefix for the output images.\n";
    std::cout << "> out_dp: directory path where the output images should be stored.\n";
    std::cout << "\n";
    std::cout << "Usage:\n";
    std::cout << "\nType of simplification (obligatory, choose only one):\n";
    std::cout << " -s nberim inf_dp/in_fp out_dp[/out_fp]\t#Standard simplifier\n";
    std::cout << " -f nberim inf_dp/in_fp out_dp[/out_fp]\t#Standard simplifier with "
                 "removals first\n";
    std::cout << " -a nberim inf_dp/in_fp out_dp[/out_fp]\t#Additions only\n";
    std::cout << " -r nberim inf_dp/in_fp out_dp[/out_fp]\t#Removals only\n";
    std::cout << "\nShape related options:\n";
    std::cout << " -tc value\t#Grayscale threshold value for the core.\n"
                 "\t\t Default value: 200.\n";
    std::cout << " -ts value\t#Grayscale threshold value for the shape.\n"
                 "\t\t Default value: 150.\n";
    std::cout << " -tn value\t#Grayscale threshold value for the neighborhood.\n"
                 "\t\t Default value: 100.\n";
    std::cout << " -opt0 value\t#Lowest number of connected components possible.\n"
                 "\t\t Default value: 1.\n";
    std::cout << " -opt1 value\t#Lowest number of loops possible.\n"
                 "\t\t Default value: 0.\n";
    std::cout << " -opt2 value\t#Lowest number of voids possible.\n"
                 "\t\t Default value: 0.\n";
    std::cout << "\nSimplification related options:\n";
    std::cout << " -w value\t#Width to be used for the boxes.\n"
                 "\t\t Default value: 100.\n";
    std::cout << " -cc value\t#Maximal size of a component to be completely removed.\n"
                 "\t\t Give a negative value if no limit is wanted.\n"
                 "\t\t Default value: -1.\n";
    std::cout << " -pc value\t#Maximum % of noise to remove (Value between 0 and 100).\n"
                 "\t\t Default value: 100.\n";
    std::cout << "\nAdditional options:\n";
    std::cout << " --silent: avoids output in the terminal.\n";
    std::cout << "\nFor more details, see the Readme file.\n";
    std::cout << "\n";
}

int main(int argc, char **argv) {
    if (argc < 5 || argc > 23){
        displayUsage();
        return 0;
    }

    enum simplification_type{
        S, F, A, R
    };

    Simplifier::ImageData data;
    simplification_type simp = S;
    unsigned int boxWidth = 100;
    int ccSize = -1;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--silent") == 0)
        {
            _silent = true;
            i++;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            if (i + 3 >= argc) {
                displayUsage();
                return 0;
            }
            simp = S;
            data.numberOfImages = atoi(argv[i+1]);
            data.inputFileNameBase = argv[i+2];
            data.outputFileNameBase = argv[i+3];
            i += 4;
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            if (i + 3 >= argc) {
                displayUsage();
                return 0;
            }
            simp = F;
            data.numberOfImages = atoi(argv[i+1]);
            data.inputFileNameBase = argv[i+2];
            data.outputFileNameBase = argv[i+3];
            i += 4;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            if (i + 3 >= argc) {
                displayUsage();
                return 0;
            }
            simp = A;
            data.numberOfImages = atoi(argv[i+1]);
            data.inputFileNameBase = argv[i+2];
            data.outputFileNameBase = argv[i+3];
            i += 4;
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            if (i + 3 >= argc) {
                displayUsage();
                return 0;
            }
            simp = R;
            data.numberOfImages = atoi(argv[i+1]);
            data.inputFileNameBase = argv[i+2];
            data.outputFileNameBase = argv[i+3];
            i += 4;
        }
        else if (strcmp(argv[i], "-tc") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.coreThreshold = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-ts") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.shapeThreshold = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-tn") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.neighborhoodThreshold = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-opt0") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.minNumberOfComponents = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-opt1") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.minNumberOfLoops = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-opt2") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.minNumberOfVoids = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            boxWidth = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-cc") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            ccSize = atoi(argv[i+1]);
            i += 2;
        }
        else if (strcmp(argv[i], "-pc") == 0)
        {
            if (i + 1 >= argc) {
                displayUsage();
                return 0;
            }
            data.minRatio = atoi(argv[i+1]);
            i += 2;
        }
        else
        {
            displayUsage();
            return 0;
        }
    }

    Simplifier simplifier(_silent);

    clock_t totalTime = clock();
    clock_t time = clock();

    simplifier.loadPreprocessedImage(data);
    printTime(time, "Image load time: ");

    if (simp == S) {
        time = clock();
        simplifier.simplify(boxWidth, ccSize);
        printTime(time, "Process time: ");
    } else if (simp == F) {
        time = clock();
        simplifier.simplifyWithSubFirst(boxWidth, ccSize);
        printTime(time, "Process time: ");
    } else if (simp == A) {
        time = clock();
        simplifier.simplifyOnlyAdditively(boxWidth);
        printTime(time, "Process time: ");
    } else if (simp == R) {
        time = clock();
        simplifier.simplifyOnlySubstractively(boxWidth, ccSize);
        printTime(time, "Process time: ");
    }

    time = clock();
    simplifier.saveImage(data);
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








