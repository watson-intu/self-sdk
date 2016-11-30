# Workshop 6: Creating a camera sensor.

In this workshop, you create a camera sensor. Sensors take in data from the outside world to process. The camera sensor will aim to take in visual data from the environment, ready to be processed on Intu. 

**Before you begin:** You must have a **Mac**, and you must have completed Workshop 1: Say Hello!. You will notice that Intu and Self are used interchangeably. Self is the technical name for Intu.

Complete the following tasks:

1. [Understanding some Self terminology](#understanding-some-intu-terminology)
2. [Building the Self SDK](#building-the-intu-sdk)
3. [Creating a camera sensor](#creating-a-camera-sensor)
4. [Configuring your Intu instance to include the camera sensor](#configuring-your-Intu-instance-to-include-the-emotion-agent)


## 1. Understanding some Self terminology

Before you create an emotion agent, become familiar with the following terminology:

  * **Blackboard**: The central message broker on which all the agents post data and listen for incoming data.
  
  * **Publish**: To push data onto the blackboard under a particular topic. 
  
  * **Subscribe**: Subscribing to a topic on the blackboard means to listen to the blackboard and wait for any other agents to post to the blackboard under a particular topic.
  
  * **Topic**: A channel to which Agents publish and subscribe.

## 2. Building the Self SDK

Follow the instructions for your platform.

**Before you begin**:

1. [Download the Self SDK](https://github.com/watson-intu/self-sdk). Click on the **download icon** next to the default **master** branch selected.

2. Create a new directory named **intu** in a directory of your choosing.

3. Unzip the **wlabs_self-sdk-master.zip** file into **intu**, making sure that you retain the folder structure, i.e. your intu directory should now contain the unzipped **wlabs_self-sdk-masterr** folder.

### A. Preparing for OS X

**Note**: If you have already completed **Preparing for OS X** in another workshop, you can skip this section.

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
   3. Specify your password, and press **Return** or **Enter**. If you have **macOS Sierra**, the following message is displayed:
   
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

		**Note**: If you have **macOS Sierra**, you may see the following message if you happen to have an outdated version of **Xcode**: 
	
		```
	Your Xcode (7.3.1) is outdated.
	Please update to Xcode 8.1 (or delete it).
	Xcode can be updated from the App Store.
		```
	
		If you see this message, you will need to update **Xcode**.
	
3. Download **Anaconda 4.2.0 Python 2.7 version** by using the **Graphical Installer**. It is required to correctly configure pip in the following step.
   1. Open a new browser window and [download Anaconda 4.2.0 Python 2.7 version](https://www.continuum.io/downloads).
   2. Click the solid blue GRAPHICAL INSTALLER button for Python 2.7 Version. It should be 403 MB. The .pkg file downloads.
   3. After the file is downloaded, double-click it, and follow the prompts to install Anaconda.
   4. Open a **new** Terminal window and make sure your version of Python has been successfully updated by running the command: `python --version` 
 	
 		You should see: `Python 2.7.12 :: Anaconda 4.2.0 (x86_64)`
  
4. Set up qiBuild.
   1. Run: `pip install qibuild`
   2. Run: `qibuild config --wizard` and be sure to include the two hyphens before **wizard** in the last command.
   3. When the "Please choose a generator" prompt is displayed, specify **1**, and press **Enter**.
   4. When the "Please choose an IDE" prompt is displayed, specify **1**, and press **Enter**.
   
### B. Building the Intu SDK for OS X

1. In a new Terminal window, navigate to the **wlabs_self-sdk-master** directory inside **intu**. You should just be able to run: `cd intu/wlabs_self-sdk-master` 

	**Note:** If you have **already built the Intu SDK for OS X** in another workshop, run: `./scripts/clean.sh`    

2. Run:
`./scripts/build_mac.sh`  

	**Note:** If you are running this script for the first time and you see the following message, do not worry as you do not have the toolchain. Let the script run and proceed with the next step.
```
[ERROR]: Exception No such toolchain: mac-self
Known toolchains are:
```

## 3. Creating a camera sensor

### A. Preparing your directories and files for the camera sensor

1. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/).

2. In **CLion**, select Open -> home directory -> intu -> wlabs_self-sdk-master and click **OK**. 

	Note that a window may appear prompting you to open your project in a New Window or This Window. Select **New Window**. At the bottom of your CLion window, in the Problems tab, you will see the following Error, which you do not need to worry about:
	
	```
	Error: By not providing "FindSELF.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "SELF", but CMake did not find one.
Could not find a package configuration file provided by "SELF" with any of the following names:
  SELFConfig.cmake   self-config.cmake
Add the installation prefix of "SELF" to CMAKE_PREFIX_PATH or set "SELF_DIR" to a directory containing one of the above files.  If "SELF" provides a separate development package or SDK, be sure it has been installed.
	```

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

3. Create a new directory inside this called **sensors** in the **workshop_six** directory.

4. Locate the Workshop 6 code snippet files **to be filled in** in:

 `wlabs_self-sdk-master/docs/workshops-devcon/6/code-snippets/WorkshopSixSensor_start`

5. Copy the `WorkshopSixSensor.cpp` and the `WorkshopSixSensor.h` files and paste them into the **sensors** directory that you created.

### B. Building out the OnStart(), CaptureVideo() and SendingData() functions for the camera sensor

Open the `WorkshopSixSensor.cpp` file, which contains the following functions that enable the emotion agent you'll create:

  * **OnStart()**: Initializes the camera sensor. The OnStart() function is called by the SensorManager class. Because OnStart occurs on the main thread, we want to do as little as possible processing and do the core logic on a background thread. Here we instantiate the VideoCapture object from opencv and spawn a background thread to capture the images. Using our Delegate class, we can call functions in C++ on different threads.
  
  * **OnStop()**: Stops the camera sensor. After the sensor is called, it should wait for any processing to occur and release any objects in memory.
  
  * **OnPause()**: Pauses the camera sensor. The pause function increments an m_Pause integer. The reason this is not a boolean is because other places throughout core Intu can pause a sensor, and we need to keep track how many times pause is called before we know we can resume. When Pause is called, no data is sent to the extractor that has subscribed to its data.
  
  * **OnResume()**: Resumes the sensor. The sensor can then start sending data to the extractor that is subscribed to it.
  
  * **CaptureVideo()**: Using opencv, we take raw buffers from the camera and encode them in JPEG format. This will continually happen based on the frames per second that is declared from the m_FramesPerSecond variable, and will stop when the OnStop() function is called.
 
  * **SendingData()**: Checks whether the camera is paused. If it's not paused, data is sent to all extractors that have subscribed to it.

The OnStop, OnPause and OnResume functions are already completely built out.

In the next step, you will build out the **OnStart**, **CaptureVideo** and **SendingData** functions using the example code provided.

1. In **wlabs_self-sdk-master****w/docs/workshops-devcon/6/code-snippets/WorkshopSix_Snippets**, you will see the `WorkshopSixCodeSnippets.txt` file. Open this file and find the OnStart, CaptureVideo and SendingData functions.

2. From `WorkshopSixCodeSnippets.txt `, for the **OnStart()** function, copy the code directly below **//Code for OnStart()**. Now paste this inside the function body **{}** of **OnStart()** in `WorkshopSixSensor.cpp` inside your **sensors** directory, overwriting all the code already inside the function body **{}**. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.


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

3. For the **CaptureVideo()** function, copy the code directly below **//Code for CaptureVideo()**. Now paste this inside the function body **{}** of ** CaptureVideo()** in `WorkshopSixSensor.cpp`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.


  
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
  
4. For the **SendingData()** function, copy the code directly below **//Code for SendingData()**. Now paste this inside the function body **{}** of **SendingData()** in `WorkshopSixSensor.cpp`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.

  
  ```
	if (m_Paused <= 0)
        SendData(a_pData);
  ```


5. Open a **new** Terminal window and navigate to **intu/wlabs_self-sdk-master** by running: `cd intu/wlabs_self-sdk-master`.

	**Note:** If you have built the SDK in a previous workshop, make sure you run: `./scripts/clean.sh`.

6. Run the build script: `./scripts/build_mac.sh`. **Do not close this Terminal window.**

**Congratulations!** You just built all the functions required for the camera sensor. 

In the next task, you will update the `body.json` file also located in the **intu/wlabs_self-sdk-master/bin/mac/etc/profile** directory to include the new plugin so that Intu can use it.


## 4. Configuring your Intu instance to include the camera sensor

### A. Retrieving the credentials for your Organization in the Intu Gateway

1. [Log in to the Intu Gateway](https://rg-gateway.mybluemix.net/). 

2. Click on **VIEW CREDENTIALS** in the left hand navigation bar.

3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.

4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file using your favourite text editor.

### B. Configuring your `body.json` file

1. Open your `body.json` file. This will be in **master_self-sdk-master/bin/mac/etc/profile**.
	
2. Locate the `m_Libs` variable. The variable should look like `"m_Libs" : [ "platform_mac" ],`

3. Add **workshop****_six****_plugin** to the end of the `m_Libs` variable for your platform, **as shown below**:
  `"m_Libs" : [ "platform_mac", "workshop_six_plugin"],`
4. Now find `"m_EmbodimentCreds":{ ... }` in your `body.json` file. Replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in step 4 of the previous section.

5. Locate `m_Sensors` inside the `body.json`. Add the following

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

### C. Building Intu

1. Return to your most recent Terminal window, where you should already be in the **mac** directory. Otherwise, open a **new** Terminal window and navigate to this directory by running: `cd intu/wlabs_self-sdk-master/bin/mac`

2. Run the following command:
  `export LD_LIBRARY_PATH=./`
3. Run Intu by issuing the following command: `./self_instance `

Now Intu is running, you should see your Mac camera turn on. Now Intu will be able to capture vision through your Mac camera.

### D. Challenge: Add a camera sensor to a Raspberry Pi

The process you have just undertaken was adding a camera into your Intu instance on your Mac, by extending the self-sdk. The challenge is to now add a camera instance for the Raspberry Pi. (Hint: do workshop 5 first and think how you can alter the process and think how how could get the code for the camera on the Raspberry Pi instead of the LED gesture. 

**Pro Tips:** 

1. In your Terminal/PuTTY window, SSH into your Raspberry Pi, and run: `sudo modprobe bcm2835-v4l2` to ensure that the PiCam data is passed through. 

2. Enable the camera on your Raspberry Pi. 
	1. Open the raspi-config tool from your SSH session to the Raspberry Pi by running: `sudo raspi-config`.
	2. Select `Enable camera` and hit **Enter**. 
	3. Then go to **Finish** and you'll be prompted to reboot.

## After DevCon ends

Your instance of Intu is preconfigured with the following Watson services: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech. The preconfiguration is enabled for 30 days. If you want to test Intu after 30 days, you must create your own instances of these services and configure Intu to use them.

### A. Creating instances of Watson services

To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favourite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the top right navigation bar.

2. In the All Categories menu on the left, under Services, click on **Watson**.

3. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new Conversation service instance.
  5. Copy the values for your password and username and paste them into a new text file in your favourite text editor.
  6. Click the **<--Watson** breadcrumb near the top left (directly above your Conversation service name). The list of your service instances is displayed.
  7. Add the next service instance by clicking the **Create Watson** **+** button. The Watson service catalog is displayed.

4. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same substeps 1 - 7 that you completed to create the Conversation service instance.


### B. Configuring Intu to use your service instances

To configure Intu to use your instances of these Watson services, log in to the [Intu Gateway](https://rg-gateway.mybluemix.net/) and complete the following steps:

1. Click on **MANAGE** on the left hand side navigation bar, and select **Services**. 

2. Select your Organization and Group in the top Filter by menu, if not already selected.

3. For your instances of the Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, and specify the user ID and password (saved in your text file in the previous section **Creating instances of Watson services**), and click **Save**.

4. To configure your instance of **Conversation**, navigate to **DOWNLOADS** on the left of your Intu Gateway browser page, download the **Intu Starter Kit**, and follow the instructions in the `readme.txt` file. Alternatively, go to the instructions for **Workshop 2**, and follow the steps in: **1. Setting up the Conversation service**.
 

**Important:** Do not change the service endpoint for your services unless you are an enterprise user.
