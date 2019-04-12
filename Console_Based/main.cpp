#include <QCoreApplication>
#include <opencv/cv.hpp>
#include <QDebug>
#include <QByteArray>
#include <markerdetector.h>
#include<stereoProcess.h>
#include <QtNetwork/QUdpSocket>
#include "testclass.h"

using namespace cv;

void makeBuffer(QByteArray &arr , double X,double Y , double Z , qint32 msg_id){

    qint16 dev_id=1;
    qint16 msg_len=24;
    arr.clear();
    arr.append(reinterpret_cast<const char *>(&dev_id),sizeof(dev_id));
    arr.append(reinterpret_cast<const char *>(&msg_id),sizeof(msg_id));
    arr.append(reinterpret_cast<const char *>(&msg_len),sizeof(msg_len));
    arr.append(reinterpret_cast<const char *>(&X),sizeof(X));
    arr.append(reinterpret_cast<const char *>(&Y),sizeof(Y));
    arr.append(reinterpret_cast<const char *>(&Z),sizeof(Z));


}

int main(int argc, char *argv[])
{
    qint32 msg_id=0;
    QCoreApplication a(argc, argv);

    double leftCamX,rightCamX,leftCamY,rightCamY;
    double posX,posY,posZ;
    markerDetector leftCamMarker;
    markerDetector rightCamMrker;
    stereoProcess stereoProcessor;
    QUdpSocket Socket;
    Mat leftCamF;
    Mat rightCamF;
    Mat Fixed1;
    Mat Fixed2;
    QByteArray buffer;

     //VideoCapture  leftCam(2);
     //VideoCapture rightCam(1);
    stereoProcessor.printInfo();
     VideoCapture leftCam("Left_Video.mpg");
     VideoCapture rightCam("Right_Video.mpg");

    if(leftCam.isOpened()==0 || rightCam.isOpened()==0){
        qDebug() << "Couldn't Open the Video input";
        return a.exec();

    }else{
        leftCam.set(CAP_PROP_FPS,120);
        rightCam.set(CAP_PROP_FPS,120);
        while(waitKey(1)){

            leftCam >> leftCamF;
            rightCam >> rightCamF;

            if(leftCamF.empty() || rightCamF.empty()){
                qDebug()<<"Empty Frame!!!";
                return a.exec();
            }

            if(1){

                stereoProcessor.fixImage(leftCamF,Fixed1,1);
                stereoProcessor.fixImage(rightCamF,Fixed2,2);
                leftCamMarker.getPixelPos(leftCamF,&leftCamX,&leftCamY);
                rightCamMrker.getPixelPos(rightCamF,&rightCamX,&rightCamY);
                stereoProcessor.Process2(rightCamX,rightCamY,leftCamX,leftCamY,&posX,&posY,&posZ);

                qDebug() << "X , Y , Z : "<< (posX*1000) <<  (posY*1000) << (posZ*1000);
                makeBuffer(buffer,posX,posY,posZ,msg_id++);
                Socket.writeDatagram(buffer,QHostAddress("192.168.2.50"),5000);

            }
            else
                qDebug()<<"No Object Found !!!";

            imshow("Left camera feed",leftCamF);
            imshow("Right camera feed",rightCamF);

        }
        leftCam.release();
        rightCam.release();

    }

    return a.exec();

}
