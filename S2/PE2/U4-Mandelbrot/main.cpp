#include <iostream>
#include <list>
#include <fstream>
#include "Complex.h"

double mapToReal(int x, int imgWidth, double minR, double maxR){
    double range = maxR - minR;
    return x * (range / imgWidth) + minR;
}

double mapToImaginary(int y, int imgHeight, double minI, double maxI){
    double range = maxI - minI;
    return y * (range / imgHeight) +minI;
}

double findMandelbrot(double cr, double ci, int maxIterations){
    int i = 0;
    double zr = 0.0, zi = 0.0;
    while(i < maxIterations && (zr * zr + zi * zi) < 4.0){
        double temp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = temp;
        i++;
    }
    return i;
}

int main(int argc, char *argv[]) {
    Complex c = Complex(1.0, 3.0);
    Complex c1 = Complex(2.0, 5.0);
    c.add(c1);
    c.print();

    std::ifstream fin("input.txt");
    int imgWidth, imgHeight, maxN;
    double minR, maxR, minI, maxI;
    if(!fin){
        std::cout << "Couldnt open file! \n";
        return -1;
    }
    fin >> imgWidth >> imgHeight >> maxN;
    fin >> minR >> maxR >> minI >> maxI;
    fin.close();

    std::ofstream fOut("output.ppm");
    fOut << "P3\n";
    fOut << imgWidth << " " << imgHeight << "\n";
    fOut << "256\n";
    for (int y = 0; y < imgHeight; ++y) {
        for (int x = 0; x < imgWidth; ++x) {
            double cr = mapToReal(x, imgWidth, minR, maxR);
            double ci = mapToImaginary(y, imgHeight, minI, maxI);
            int n = findMandelbrot(cr, ci, maxN);
            int r = (n%256), g = (n%256), b = (n%256);
            fOut << r << " " << g << " " << b << " ";
        }
        fOut << std::endl;
    }
    fOut.close();
    std::cout << "Finished!\n";
    return 0;
}