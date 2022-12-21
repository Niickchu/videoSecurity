#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/face/facerec.hpp>
#include "recognizeFace.h"
#include "servo.h"
#include "led.h"

#include <iostream>
#include <time.h>
#include <unistd.h>

using namespace std;
using namespace cv;
using namespace cv::face;

string people[4] = {"Nick", "Kunal", "Shravan", "Alex"};
bool isRunning = true;
Mat frame, grey, bufferFrame;
int id = 0;

CascadeClassifier face_cascade;
Ptr<LBPHFaceRecognizer> model;
VideoCapture cam(0, CAP_V4L2);
int minWidth = 0;
int minHeight = 0;

extern "C" void faceRecognition_init()
{
    printf("Loading face models... ");
    model = LBPHFaceRecognizer::create(); // create LBPH face recognizer
    model->read("/mnt/remote/myApps/data/models/trainer.yml");

    if (!face_cascade.load("/mnt/remote/myApps/data/haarcascades/haarcascade_frontalface_alt.xml"))
    { // load face cascade
        cout << "--(!)Error loading face cascade\n";
        return;
    }

    // sleep(5);
    cam.set(CAP_PROP_FRAME_WIDTH, 426);
    cam.set(CAP_PROP_FRAME_HEIGHT, 240);
    cam.set(CAP_PROP_BUFFERSIZE, 1);

    minWidth = 0.1 * cam.get(CAP_PROP_FRAME_WIDTH); // set min face size as 10% of the frame width
    minHeight = 0.1 * cam.get(CAP_PROP_FRAME_HEIGHT);
    printf("Done\n");
}

extern "C" int recognizeFace()
{
    int confidenceCounter = 0;
    int numOfAttempts = 0;
    cam.read(frame); // get rid of the buffer frame to get current frame-- this is a hack
    for (int i = 0; i < 15; i++){
   
        cam.read(frame);
        cvtColor(frame, grey, COLOR_BGR2GRAY); // convert to grayscale

        std::vector<Rect> faces;                                                                                // 1.1 to 1.X where x>1 makes it faster
        face_cascade.detectMultiScale(grey, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(minWidth, minHeight)); // everytime we scale, 5 is the min number of hits to detect a face

        if (faces.size() == 0){
            printf("No faces detected\n");
        }
        else if (faces.size() > 1){
            printf("Multiple faces detected\n");
        }

        else{
            Rect face = faces[0];

            // move servo to face
            SERVO_moveToFace(426 - (face.x + face.width / 2), 240 - (face.y + face.height / 2));

            Mat faceROI = grey(face); // region of interest is the face
            int label = 0;
            double confidence = 0;
            model->predict(faceROI, label, confidence);

            if (confidence < 55)
            {
                cout << "Face recognized as " << people[label] << " conf: " << 100 - confidence << endl;
                confidenceCounter++;
                i--;    //give more attempts if the face is recognized

                //display confidence
                fillMatrix(confidenceCounter);

                if (confidenceCounter >= 4)
                {
                    return label;
                }
            }
            else
            {   
                cout << "unrecognized" << endl;

                displayFace(face.x * 8 / 426, face.y * 8 / 240, 8*face.width / 240);
                printf("arguements: %d, %d, %d", face.x * 8 / 426, face.y * 8 / 240, 8*face.width / 240);
                confidenceCounter--;
                if (confidenceCounter < 0)      //dont let confidence below 0
                {
                    confidenceCounter = 0;
                }
            }
        }
    }
    return -1;
}
