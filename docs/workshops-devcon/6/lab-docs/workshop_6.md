# Workshop 6: Creating a camera agent

In this workshop, you create an camera sensors. Sensors take in data from the outside world to process. The camera sensor will aim to take in visual data from the environment, ready to be processed on Intu. 

**Before you begin:** You must have a **Mac**, and you must have completed Workshop 1: Say Hello!.

Complete the following tasks:

1. [Understanding some Intu terminology](#understanding-some-intu-terminology)
2. [Building the Intu SDK](#building-the-intu-sdk)
3. [Creating a camera sensor](#creating-a-camera-sensor)
4. [Configuring your Intu instance to include the camera sensor](#configuring-your-Intu-instance-to-include-the-emotion-agent)


## 1. Understanding some Intu terminology

Before you create an emotion agent, become familiar with the following terminology:

  * **Blackboard**: The central message broker on which all the agents post data and listen for incoming data.
  * **Publish**: To push data onto the blackboard under a particular topic. 
  * **Subscribe**: Subscribing to a topic on the blackboard means to listen to the blackboard and wait for any other agents to post to the blackboard under a particular topic.
  * **Topic**: A channel to which Agents publish and subscribe.

##2. Building the Intu SDK

Follow the instructions for your platform.

**Before you begin**:

1. [Download the Intu SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on **Clone or download** and select **Download Zip**.
2. Create a new directory named **intu** in a directory of your choosing.
3. Unzip the self-sdk-develop.zip file into **intu**, making sure that you retain the folder structure. I.e. Your intu directory should now contain the unzipped **self-sdk-develop** folder.

### Preparing OS X

**Note**: If you have already completed **Preparing OS X** in another workshop, you can skip this section.

1. Install Homebrew by completing the following steps:
   1. Open a new terminal window, and run the command:
   ```
   ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
   ```
   The following message is displayed:
   ```
   Press RETURN to continue or any other key to abort
   ```
   2. Press **Return** or **Enter**. A prompt for your laptop's password is displayed.
   3. Specify your password, and press **Return** or **Enter**. If you're using **Sierra OS X**, the following message is displayed:
   ```
   HEAD is now at 89fd34b Merge pull request #1368 from MikeMcQuaid/build-options-file
   Error: /usr/local/Cellar is not writable. You should change the
   ownership and permissions of /usr/local/Cellar back to your
   user account:
   sudo chown -R $(whoami) /usr/local/Cellar
   Failed during: /usr/local/bin/brew update --force
   ```
   Run: `sudo chown -R $(whoami) /usr/local/Cellar`
   4. Now repeat step 1 by running: 
    ```
   ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
   ```
2. Install CMake using Homebrew.
	1. Run the command: `sudo chown -R $(whoami) /usr/local`   
	2. Now run: `brew install cmake`
3. Download **Anaconda 4.2.0 Python 2.7 version** by using the **Graphical Installer**. It is required to correctly configure pip in the following step.
   1. Copy and open the following in a browser: https://www.continuum.io/downloads
   2. Click the solid blue GRAPHICAL INSTALLER button for Python 2.7 Version. It should be 403 MB. The .pkg file downloads.
   3. After the file is downloaded, double-click it, and follow the prompts to install Anaconda.
4. Set up qiBuild.
   1. Run the following commands:
   * `pip install qibuild`
   * `qibuild config --wizard`. Be sure to include the two hyphens before the word wizard in the last command.
   2. When the "Please choose a generator" prompt is displayed, specify **1**, and press **Enter**.
   3. When the "Please choose an IDE" prompt is displayed, specify **1**, and press **Enter**.

### Building the Intu SDK for OS X
1. In a new Terminal window, navigate to the **self-sdk-develop** directory inside **Intu**. You should just be able to run: `cd intu/self-sdk-develop` 

	**Note:** If you have **already built the Intu SDK for OS X** in another workshop, run: `./scripts/clean.sh`    


2. Run:
`./scripts/build_mac.sh`  
If you're running this script for the first time and you see the following message, don't worry. You don't have the toolchain.
```
[ERROR]: Exception No such toolchain: mac-self
Known toolchains are:
```


## 3. Creating a camera sensor
### 1. Preparing your directories and files


1. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/).

2. In **CLion**, select Open -> home directory -> intu -> self-sdk-develop and click **OK**. 

	Note that a window may appear prompting you to open your project in a New Window or This Window. Select **New Window**. At the bottom of your CLion window, in the Problems tab, you will see the following Error, which you do not need to worry about:

	Error: By not providing "FindSELF.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "SELF", but CMake did not find one.
Could not find a package configuration file provided by "SELF" with any of the following names:
  SELFConfig.cmake   self-config.cmake
Add the installation prefix of "SELF" to CMAKE_PREFIX_PATH or set "SELF_DIR" to a directory containing one of the above files.  If "SELF" provides a separate development package or SDK, be sure it has been installed.

2i. Inside the CLion **self-sdk-develop project**, right-click **examples**, select **New**, and select **Directory**. Type in **workshop_six** for the new directory name, and click **OK**.
 
2ii. Right-click the `CMakeLists.txt` file in the **examples** directory, and click **Copy**. (If you are unsure of the directory you are in, look in the top-left navigation bar.)
  
2iii. Right-click the **workshop_six** directory, and click **Paste**. This file helps to build the plugin for the camera sensor.

2iv. Return to the **examples** directory, open the `CMakeLists.txt` file, and add the following line: `add_subdirectory(workshop_six)` at the end. Your file contains the following three lines:

  ```
    include_directories(".")

    add_subdirectory(sensor)
    add_subdirectory(workshop_six)
  ```
2v. Open the `CMakeLists.txt` file in the **workshop_six** directory, and overwrite its content with this code:

  ```
include_directories(.)

file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(workshop_six_plugin SHARED ${SELF_CPP})
qi_use_lib(workshop_six_plugin self wdc OPENCV2_CORE OPENCV2_HIGHGUI)
qi_stage_lib(workshop_six_plugin)
  ```

1. Create a new directory inside this called **sensors** in the **workshop_six** directory.
2. Locate the Workshop 6 code snippet files **to be filled in** in:

 `self-sdk-develop/docs/workshops-devcon/6/code-snippets/WorkshopSixSensor_start`

3. Copy the `WorkshopSixSensor.cpp` and the `WorkshopSixSensor.h` files and paste them into the **sensors** directory that you created.


### 2. Build out the CaptureVideo() and SendingData() functions

Open the `WorkshopSixSensor.cpp` file, which contains the following functions that enable the emotion agent you'll create:

* **OnStart()** - This function will act as the initialisation for the Camera Sensor i.e. it will subscribe the Camera Sensor to the Blackboard. Once initialised the Camera Sensor which will Subscribe to OnEmotion, OnLearningIntent and OnEmotionCheck. 

* **OnStop()** - This function will act to cease the Camera Sensor i.e. once called the Camera Sensor will no longer be subscribed to the Blackboard (and cease to exist?).

* **OnPause()** - This function is used to pause the Camera Sensor.

* **OnResume()** - This function is used to resume the Camera Sensor to start recording.

* **CaptureVideo()** - This function is responsible for recording from using the Mac Camera Sensor. OpenCV will be used to fill the buffer by calling encoding to a JPEG. decrease(negative) the EmotionalState score by 0.1. 

* **SendingData()** - Is used to first check if the camera is paused or not. If the camera is not then the data is sent.

  
The OnStop, OnPause and OnResume functions are already completely built out.

In the next step, you will build out the **OnStart**, **CaptureVideo** and **SendingData** functions using the example code provided.

**Building the sensor function bodies**

1. In **self-sdk-develop/docs/workshops-devcon/3/code-snippets/WorkshopSix_Snippets**, you will see the `WorkshopSixCodeSnippets.txt` file. Open this file and find the OnStart, CaptureVideo and SendingData functions.

1. For **OnStart()**, copy the code directly below **//Code for OnStart()** in `WorkshopSixCodeSnippets.txt`. Paste this inside the function body **{}** of **OnStart()** in `WorkshopSixSensor.cpp` located inside your **sensors** directory. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues. Copy the code and overwrite the all code already in **OnStart()**.

  ```
  	    Log::Debug("WorkshopSixSensor", "Starting Camera!");
    m_StopThread = false;
    m_ThreadStopped = false;
#ifndef _WIN32
    m_Capture.open(0);

    if( !m_Capture.isOpened() )
    {
        Log::Error("MacCamera", "Could not open Mac Camera - closing sensor");
        return false;
    }
    Log::Debug("WorkshopSixSensor", "Video Capture is now opened!");
#endif
    ThreadPool::Instance()->InvokeOnThread<void *>(DELEGATE(WorkshopSixSensor, CaptureVideo, void *, this), 0);
    return true;
    
    ```

1. For **CaptureVideo()**, copy the code directly below **//Code for CaptureVideo()** in `WorkshopSixCodeSnippets.txt`. Paste this inside the function body **{}** of **CaptureVideo()** in `WorkshopSixSensor.cpp` located inside your **sensors** directory. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.
  
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
      The above code will capture the vision from the camera. Using OpenCV the buffer will be filled as a JPEG, as long as the it hasn't been paused.
      
2. For **SendingData()**, copy the code directly below **//Code for SendingData()** in `WorkshopSixCodeSnippets.txt`. Paste this inside the function body **{}** of **SendingData()** in `WorkshopSixSensor.cpp` located inside your **sensors** directory. 

	The code which you need is displayed below:
  
  ```
	if (m_Paused <= 0)
        SendData(a_pData);
  ```


1. Open a **new** Terminal window and navigate to **intu/self-sdk-develop** by running: `cd intu/self-sdk-develop`.

	**Note:** If you have built the SDK in a previous workshop, make sure you run: `./scripts/clean.sh`.

2. Run the build script: `./scripts/build_mac.sh`. **Do not close this Terminal window.**

**Congratulations!** You just built all the functions required for the camera sensor. 

In the next task, you will update the `body.json` file also located in the **intu/self-sdk-develop/bin/mac/etc** directory to include the new plugin so that Intu can use it.


## 4. Configuring your Intu instance to include the camera sensor

### 1. Retrieving the credentials for your Organization in the Intu Gateway
1. Log in to the [Watson Intu Gateway](https://rg-gateway.mybluemix.net/). 

2. Click on **VIEW CREDENTIALS** in the left hand navigation bar.

3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.
4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file using your favourite text editor.

### 2. Configuring your `body.json` file

1. Open your `body.json` file. This will be in **self-sdk-develop/bin/mac/etc/**.
	
2. Locate the `m_Libs` variable. The variable should look like `"m_Libs" : [ "platform_mac" ],`
3. Add **workshop****_six****_plugin** to the end of the `m_Libs` variable for your platform, **as shown below**:
  `"m_Libs" : [ "platform_mac", "workshop_six_plugin"],`
6. Now find `"m_EmbodimentCreds":{ ... }` in your `body.json` file. Replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in step 4 of the previous section.
7. Locate `m_Sensors` inside the `body.json`. Add the following

```
{
       "Type_" : "Camera",
       "m_SensorId" : "8f385c2a-ecb0-3bfb-32af-3c54ec18db5c",
       "m_fFramesPerSec" : 10
    },
    
```

It should look something like

```
"m_Sensors" : [
      {
       "Type_" : "Camera",
       "m_SensorId" : "8f385c2a-ecb0-3bfb-32af-3c54ec18db5c",
       "m_fFramesPerSec" : 10
    },
      {....
```

Save your changes.

### 3. Building Intu

1. In your most recent Terminal window, navigate to the **bin** directory (**self-sdk-develop/bin/mac**).
2. Run the following command:
  `export LD_LIBRARY_PATH=./`
3. In the directory **intu/self-sdk-develop/bin/mac**, run Intu by issuing the following command: `./self_instance ` .

Now self is running, you should see your Mac camera turn on. Now Intu will be able to learn objects. Hold an object [i.e. a pen] to the camera and say "this is an [object name i.e. a pen]". Intu should now be able to recognise the object [i.e. a pen].

### 4. [Challenge]: Add camera sensor to the Raspberry Pi

The process you have just undertaken was adding a camera into your Intu instance on your Mac, by extending the self-sdk. The challenge is to now add a camera instance for the Raspberry Pi. (Hint: do workshop 5 first and think how you can alter the process and think how how could get the code for the camera on the Raspberry Pi instead of the LED gesture. 

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
  4. Click **View Credentials** for the new service instance, and keep this window open.
  5. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  6. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
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
