# Workshop 6: Creating a new sensor for a camera

In this workshop, you create a new camera sensor. Sensors, such as cameras, microphones, and so on produce data and send it to extractors.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!.

Complete the following tasks:

1. [Understanding some Intu terminology](#understanding-some-intu-terminology)
2. [Building the Intu SDK](#building-the-intu-sdk)
3. [Creating a new sensor](#creating-a-new-sensor)
4. [Configuring Intu to include your new camera sensor](#configuring-intu-to-include-your-new-camera-sensor)

## Understanding some Intu terminology

Before you create new sensor, become familiar with the following terminology:

  * **Blackboard**: The central message broker on which all the agents post data and listen for incoming data.
  * **Publish**: To push data onto the blackboard under a particular topic. 
  * **Subscribe**: Subscribing to a topic on the blackboard means to listen to the blackboard and wait for any other agents to post to the blackboard under a particular topic.
  * **Topic**: A channel to which agents publish and subscribe.

## Building the Intu SDK

Follow the instructions for your platform.

**Before you begin**:

1. [Download the Intu SDK](https://github.ibm.com/watson-labs-austin/self-sdk).
2. Create a new directory named `intu`, and unzip the SDK package into it.

### Building the SDK for OS X

1. Set up [CMake](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#required-buidsys). To install CMake by using Homebrew, run `brew install cmake`. To install Homebrew, run the following command in your terminal: `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`.
2. Set up [qiBuild](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#qibuild-install) by running the following commands:
  * `pip install qibuild`: To correctly configure pip, download [Anaconda Python Version 2.7](https://www.continuum.io/downloads))
  * `qibuild config --wizard`: Use the default setup for steps by pressing 1 twice.
3. Run the following commands:
  * `cd {Self root directory}`
  * `./scripts/build_mac.sh`


### Building the SDK for Windows

1. Install [Visual Studio 2015](https://www.visualstudio.com/downloads/).
2. Open the solution found in vs2015/self-sdk.sln
3. Right click on the "self-sdk" project and select "Set as Startup Project".
4. Right click on the "self-sdk" projet, open properties. In the Debugging tab of the properties, you will need to change "Working Directory" to "$(TargetDir)".
5. Select Build->Build Solution
6. Select Debug->Start Debugging to run the project with debugging

**Important**: If you use SourceTree, the process might get stuck when trying to pull by using SSH. Run the following commands on the command line to fix the problem with the git client that's trying to be interactive:
* cd "C:\Program Files (x86)\Atlassian\SourceTree\tools\putty"
* plink git@github.ibm.com
## Creating a new sensor

1. Open the `self-sdk-develop` file as a project in the IDE for your platform.
2. Now, let's create and populate directory specifically for this workshop.
  1. Locate the `examples` directory under the `self-sdk-develop` project that you opened. This directory contains a `sensor` directory and a `CMakeLists.txt` file.
  2. Right-click the `examples` directory, and click **New**->**Directory**. Name it `workshop_six`. Your new directory is created.
  3. Copy the `CMakeLists.txt` file from the examples directory, and paste it in the `workshop_six` directory. This file helps to build the plugin for the emotion agent.
  4. Open the `CMakeLists.txt` file in the `examples` directory, and add the following line: `add_subdirectory(workshop_six)`. Your file contains the following three lines:
  ```
    include_directories(".")

    add_subdirectory(sensor)
    add_subdirectory(workshop_six)
  ```
  5. Open the `CMakeLists.txt` file in the `workshop_six` directory, and overwrite its content with this code:
  
  ```
	include_directories(.)

	file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
	qi_create_lib(workshop_six_plugin SHARED ${SELF_CPP})
	qi_use_lib(workshop_six_plugin self wdc OPENCV2_CORE OPENCV2_HIGHGUI)
	qi_stage_lib(workshop_six_plugin)
  ```
3. Navigate back to the `workshop_six` directory, create a new directory, and name it `agents`.
4. Locate the Workshop 3 code snippet files in `self-sdk-develop/docs/workshops-devcon/6/code-snippets/WorkshopSixAgent_start`, copy the `WorkshopSixAgent.cpp` and the `WorkshopSixAgent.h` files, and paste them into the `agents` directory that you created.
4. Open the `WorkshopSixAgent.cpp` file, which contains the following functions that enable the camera sensor you'll create:

  * **OnStart()**: Initializes the camera sensor. It subscribes the sensor to the blackboard. After initialization is complete, the sensor subscribes to the OnEmotion, OnLearningIntent, and OnEmotionCheck functions. 
  * **OnStop()**: Stops the camera sensor. After the sensor is called, it is no longer subscribed to the blackboard.
  * **OnPause()**: Pauses the camera sensor.
  * **OnResume()**: Resumes the camera sensor's recording.
  * **CaptureVideo()**: Records by using the Mac Camera Sensor. OpenCV is used to fill the buffer by calling encoding to .JPEG.
  * **SendingData()**: Checks whether the camera is paused. If it's not paused, data is sent.

The OnStart, OnStop, OnPause and OnResume functions are already completely built out.

In the next step, you build the CaptureVideo and SendingData function bodies yourself.

4. Write the CaptureVideo and SendingData function bodies.
  1. For CaptureVideo(), copy the following code and paste it into the function body:
  ```
	 #ifndef _WIN32
    while(!m_StopThread)
    {
        cv::Mat frame;
        m_Capture.read(frame);
        std::vector<unsigned char> outputVector;
        if ( cv::imencode(".jpg", frame, outputVector) && m_Paused <= 0 )
            ThreadPool::Instance()->InvokeOnMain<VideoData *>( DELEGATE( WorkshopSixSensor, SendingData, VideoData *, this ), new VideoData(outputVector));
        else
            Log::Error( "WorkshopSixSensor", "Failed to imencode()" );
        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(1000 / m_fFramesPerSec));
    }
	#endif
  ```
This code captures the vision from the camera. Using OpenCV, the buffer is encoded as a JPEG as long as the it hasn't been paused. This 
  2. For SendingData(), copy the following code and paste it into the function body:
  ```
	if (m_Paused <= 0)
    SendData(a_pData);
  ```
First, this code checks whether the sensor has been paused. If it hasn't been paused, it calls the SendData() function to post the data in the buffer. 

5. Rebuild this project in the SDK.

**Congratulations!** You just built all the functions required for a new camera sensor. This process created the `libworkshop_six_plugin.dylib` in the `bin` directory for your platform in the `self-sdk-develop` directory. If you're using OS X, the path is `Self/self-sdk-develop/bin/mac`.

In the next task, you update the `body.json` file to include the new plugin so that Intu can use it.

## Configuring Intu to include your new camera sensor

1. Navigate to the `self/etc/profile` directory, and open the `body.json` file.
2. Locate the `m_Libs` variable.
  * If you're using OS X, the variable is `"m_Libs" : [ "platform_mac" ],`
  * If you're using Windows, the variable is `"m_Libs" : [ "platform_win" ],`
3. Add the information for the new plugin to the end of the `m_Libs` variable for your platform:
  * If you're using OS X, the variable is `"m_Libs" : [ "platform_mac", "workshop_six_plugin"],`
  * If you're using Windows, the variable is `"m_Libs" : [ "platform_win", "workshop_six_plugin"],`

**Important**: If you completed Workshop 3, your m_Libs variable contains the workshop_six_plugin, too.

5. Save your changes.
6. Return to the directory for you platform in the `/bin` directory, and run one of the following commands:
  * If you're using OS X, run `pwd`.
  * If you're using Windows, run `cd`.
7. Run `export LD_LIBRARY_PATH=*the path returned in Step 7*`. 
**Important**: You must run this command for every terminal session that is running Self. If you close the terminal that's running Self, you must run this export command to run Self again.
8. Run Self for your platform.
  * If you're using OS X, navigate to the `mac` directory, and run `./self_instance -c 0 -f 0`.
  * If you're using Windows, NEED INFO HERE.

Self runs with logging. Within the terminal, you can see the logs. If the camera sensor is set up correctly, messages like these will be near the beginning of the log:
```

	[11/01/16 17:17:18][DEBH][WorkshopSixSensor] Starting Camera!
	[11/01/16 17:17:19][DEBH][WorkshopSixSensor] Video Capture is now opened!
```
**Congratulations!** You created a new camera sensor for your Intu embodiment.

## After DevCon ends

If you want to test Intu after the trial period ends, you must create your own instances of these services and configure Intu to use them.

### Creating instances of Watson services
To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the navigation bar.
2. Click **Watson** in the categories menu.
3. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
5. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

### Configuring Intu to use your service instances
Your installation is preconfigured to use the Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services. To configure Intu to use your instances of these services, complete the following steps:

1. Expand **All Organizations** by clicking the arrow icon.
2. Click the name of your organization.
3. Expand your organization by clicking the arrow icon.
4. Click the name of your group.
5. Click **Services** in the navigation bar.
6. For your instances of the Conversation service, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
