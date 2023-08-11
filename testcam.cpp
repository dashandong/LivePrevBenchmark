#include "testcam.h"
#include <QDebug>

const int imgWidth = 2048;
const int imgHeight = 2048;
const int imgNumber = 100;

TestCam::TestCam()
{
    buffer = new uchar[imgWidth * imgHeight * imgNumber];
    for (int i = 0; i < imgNumber; ++i) {
        for (int y = 0; y < imgHeight; ++y) {
            for (int x = 0; x < imgWidth; ++x) {
                buffer[imgWidth * imgHeight * i +y*imgWidth+x] = (sin(M_PI*((float)x/imgWidth + (float)y/imgHeight + (float)i/imgNumber) *2)+1)/2 * 255;
            }
        }
        imgList.append(QImage(buffer+imgWidth * imgHeight * i, imgWidth, imgHeight, QImage::Format_Grayscale8));
    }
}

TestCam::~TestCam()
{
    if (startRunning)
        stopCapture();
    imgList.clear();
    delete[] buffer;
}

void TestCam::startCapture()
{
    mutex.lock();
    startRunning = true;
    mutex.unlock();
    start();
}

void TestCam::stopCapture()
{
    mutex.lock();
    startRunning = false;
    mutex.unlock();
    wait();
    iterN = 0;
}

void TestCam::run()
{
    while (startRunning) {
        emit imageReady(imgList.at(iterN%imgNumber));
        usleep(100);
        iterN++;
    }
    return;
}
