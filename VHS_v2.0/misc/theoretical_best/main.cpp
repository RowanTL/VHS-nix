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

#include "TheoreticalOptimum.h"

clock_t _time;
bool _silent;

void printMessage(std::string message);

void resetTimer();
void printTime(std::string message);
void printAndResetTime(std::string message);

void displayUsage()
{
    std::cout << "Usage:\n";
    std::cout << "\n";
    std::cout << "  ./optsol nberim inf_dp/in_fp\n";
    std::cout << "\n";
    std::cout << "Lexis:\n";
    std::cout << "* nberim: number of images to process.\n";
    std::cout << "* in_fp: name prefix of the input images, if existing. \n"
                 "  E.g. \"flower_\" if the files are named \"flower_xxxx.png\", \n"
                 "  where xxxx has to be the numbering of the images with 4 digits.\n";
    std::cout << "* in_dp: directory path where the input images are stored.\n";
    std::cout << "\nfor more details, see Readme file\n";
    std::cout << "\n";
}

int main(int argc, char **argv) {
    if (argc != 3){
        displayUsage();
        return 0;
    }

    unsigned int numberOfImages = atoi(argv[1]);
    std::string inputFileNameBase(argv[2]);

    TheoreticalOptimum to;

    resetTimer();
    to.loadPreprocessedImage(inputFileNameBase, numberOfImages);
    printAndResetTime("Image load time:");

    printMessage("--Computation.");
    to.printTheoreticalOptimum();
    printTime("Computation time:");

    return 0;
}

void printMessage(std::string message){
    std::cout << message << std::endl;
}

void resetTimer(){
    _time = clock();
}

void printTime(std::string message){
    std::cout << message << " " << std::setprecision(10)
              << (static_cast<float>(clock() - _time)) / CLOCKS_PER_SEC
              << " seconds." << std::endl;
}

void printAndResetTime(std::string message){
    printTime(message);
    resetTimer();
}

