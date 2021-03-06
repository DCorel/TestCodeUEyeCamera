#include <ueyecamera.h>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    UEyeCamera Camera;
    string cameraSerialNumber = "4002718268";
    bool result = false;
    int cameraID;
    CAM_VALUE Parameters;
    int triggerCounter = 0;
    int triggerStatus;
    Parameters.d_value = 9.5;       //Setting the Exposure time [ms]
    Parameters.i_val = 50;          //Setting the Harware gain (Range: 0 - 100)
    bool initFailed = false;
    string fileName;
    string filePath = "/home/stagiairpc/Desktop/";
    string imageFormat = "BMP"; //"BMP" or "PNG" or "JPEG"
    int imageQuality = 75;
    int ImageNr = 0;
    stringstream s;

    try
    {
        Camera.GetCameraID(cameraSerialNumber, result, cameraID);
        if(result == true)
        {
            Camera.set_CameraHandle(cameraID);
            Camera.Connect();
            Camera.AllocateMemory();
            Camera.SetParameters(CAM_GAIN, Parameters);
            Camera.SetParameters(CAM_EXPOSURE, Parameters);
            Camera.SetTriggerMode(true);
        }else
        {
            cout << "Failed to connect.."<< endl;
        }

    }catch(UEyeCameraException i)
    {
        initFailed = true;
        const char* ch = i.ConvertError(i);
        cout << "Error! : " << ch << endl;
    }


    if(initFailed == false)
    {
        while(1)
        {
            //Main loop for checking of the triggers.



            //Camera.GetUEyeCameraAmountOfTriggers(triggerCounter);



            Camera.GetUEyeCameraTriggerInputStatus(triggerStatus);
            if(triggerStatus == 1)
            {
                s << ImageNr;
                fileName = "Image" + s.str();
                //triggerCounter++;
                //cout << triggerCounter<< endl; IS_SET_EVENT_EXTTRIG IS_SET_EVENT_FRAME
                cout<<"Triggered"<<endl;
                Camera.ImageCapture();
                Camera.EnableEvent(IS_SET_EVENT_FRAME);
                Camera.WaitOnEvent(IS_SET_EVENT_FRAME, 1000, true);
                Camera.SaveImage(filePath, fileName, imageFormat, imageQuality);
                //calledImageCapture = false;
                ImageNr++;
            }

/*
            if(triggerStatus == 1)
            {
                cout << "One" << endl;
            }else if(triggerStatus == 0)
            {
                cout << "Zero" << endl;
            }

            /*
            if(triggerCounter > 0)
            {

                s << ImageNr;
                fileName = "Image" + s.str();

                Camera.SaveImage(filePath, fileName, imageFormat, imageQuality);

                cout << "Image saved!" << endl;

                triggerCounter = 0;
                ImageNr++;

            }
            */





        }
    }

}
