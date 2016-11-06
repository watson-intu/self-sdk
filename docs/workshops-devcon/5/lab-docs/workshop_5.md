# Workshop 5 – Raspberry Pi


For this workshop, you will need the following:

* Raspberry Pi 3 with power cable
* Anker Bluetooth Speaker with power cable and 3.5mm audio cable
* USB Mini Microphone
* PiCamera for the Raspberry Pi 3
* LED, diode, 3 female-female jumper wires
* 32 GB SD card already imaged  -  **IF YOU HAVE NOT IMAGED YOUR CARD PLEASE GO TO THE APPENDIX**
*	Workshop 1 completed which gives you
	*	An account, organization, group and parent on the Intu Gateway
	*	Body.json configured with your Intu Gateway credentials
*	Monitor (with a HDMI connection)
*	Keyboard and Mouse (USB connection)

**Note: you should have assembled the Raspberry Pi from the instructions file from the [Workshop 5 Lab Docs - Assembling the Raspberery Pi.pdf](https://hub.jazz.net/project/wlabs/self-sdk/overview#https://hub.jazz.net/git/wlabs%252Fself-sdk/contents/master/docs/workshops-devcon/5/lab-docs/Assembling%2520the%2520Raspberry%2520Pi.pdf)**

## Setup Wi-Fi on the Raspberry Pi
1. Connect your Raspberry Pi to a power source, and connect external keyboard, mouse and monitor your Pi.
2. You should see a window on your monitor. Click on the Wifi networks (w) icon at the top of the window, select your network (at devcon, it will be `ROBOT_PED1`), and enter your password (given by the administrator).
3. On the right of the Bluetooth icon in the top right corner of the screen, you will see a successful connection to your wifi network - as shown by a blue wifi icon - or an icon with two red crosses if you have been disconnected. To reconnect, click on the two red crosses, select your network, and type in the password.
4. Get the IP address of the Pi.
   * 1. Click on the Terminal icon on the top left toolbar.
   * 2. Type in `ifconfig` and hit Enter.
        Look for the wlan0 section. The inet addr gives you the address (e.g. 10.0.1.2).
   * 3. Now you can ssh into the Raspberry Pi from your laptop using: `ssh pi@{ip_address}` (e.g. `ssh pi@10.0.1.2`)
        You will be prompted for a password. The default password is: `raspberry`.
        PC Users use Putty to connect to ip_address, then when prompted, username is `pi` and password is `raspberry`
        If you cannot connect from any OS, reboot the Pi by connecting to your Pi via ssh (or putty ssh) and running the command: `sudo reboot`

At this point, you can disconnect the external monitor, mouse and keyboard from the Pi. Go back to your laptop (**make sure it is on the same Wifi network**). Now make sure you reboot the Pi: sudo reboot by connecting to your Pi via ssh (or putty ssh) and running the command: `sudo reboot`

**Note: Unless explicitly stated, all the following steps are to be run on your local computer.**

## Download self-sdk onto your computer and add in the LED gesture code.

**Before you begin**:

1. [Download the Intu SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on **Clone or download** and select **Download Zip**.
2. Create a new directory named **intu** in your home directory.
3. Unzip the walbs_self-sdk-master.zip file into **intu**, making sure that you retain the folder structure. I.e. Your intu directory should now contain the unzipped **wlabs_self-sdk-master** folder.

### Preparing for OSX
1. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/).

2. In **CLion**, select Open -> home directory -> intu -> wlabs_self-sdk-master and click **OK**. 

	Note that a window may appear prompting you to open your project in a New Window or This Window. Select **New Window**. At the bottom of your CLion window, in the Problems tab, you will see the following Error, which you do not need to worry about:
	
	```
	Error: By not providing "FindSELF.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "SELF", but CMake did not find one.
Could not find a package configuration file provided by "SELF" with any of the following names:
  SELFConfig.cmake   self-config.cmake
Add the installation prefix of "SELF" to CMAKE_PREFIX_PATH or set "SELF_DIR" to a directory containing one of the above files.  If "SELF" provides a separate development package or SDK, be sure it has been installed.
	```

 2i. Inside the CLion **wlabs_self-sdk-maseter project**, right-click **examples**, select **New**, and select **Directory**. Type in **workshop_five** for the new directory name, and click **OK**.
 
 2ii. Right-click the `CMakeLists.txt` file in the **examples** directory, and click **Copy**. (If you are unsure of the directory you are in, look in the top-left navigation bar.)
  
 2iii. Right-click the **workshop_five** directory, and click **Paste**. This file helps to build the plugin for the the LED gesture.

 2iv. Open the `CMakeLists.txt` file in the **workshop_five** directory, and overwrite its content with this code:

  ```
include_directories(. wiringPI)
SET(GCC_COVERAGE_LINK_FLAGS “-lwiringPi”)
add_definitions(${GCC_COVERAGE_LINK_FLAGS})

file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(workshop_five_plugin SHARED ${SELF_CPP})
qi_use_lib(workshop_five_plugin self wdc)
qi_stage_lib(workshop_five_plugin)

target_link_libraries(workshop_five_plugin wiringPi)
  ```

3. Create a new directory inside this called **gestures** in the **workshop_five** directory.
4. Locate the Workshop 5 code snippet files **to be filled in** in:

 `wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_start`

5. Copy the `WorkshopFiveGesture.cpp` and the `WorkshopFiveGesture.h` files and paste them into the **gestures** directory that you created.


	* Open the `WorkshopFiveGesture.cpp` file, which contains the following functions that enable the gesture you'll create:

	* The Execute, Abort, AnimateThread and AnimateDone functions are already completely built out.

	* In the next step, you will build out the **DoAnimateThread** function using the example code provided.

**Building the gesture function body**

1. In **wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFive_Snippets**, you will see the `WorkshopFiveCodeSnippets.txt` file. Open this file and find the DoAnimateThread functions.

1. For **DoAnimateThread()**, copy the entire contents in `WorkshopFiveCodeSnippets.txt`. Paste this inside the function body **{}** of **DoAnimateThread()** in `WorkshopFiveGesture.cpp` located inside your **gestures** directory. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues. Copy the code and overwrite the all code already in **DoAnimateThread()**, inside of the brackets.

  ```
    std::vector<AnimationEntry> anims;
    for(size_t i=0;i<m_Animations.size();++i)
    {
        anims.push_back( m_Animations[i] );
    }

    if ( anims.size() > 0 )
    {
        srand( (unsigned int)Time().GetMilliseconds() );
        const AnimationEntry & entry = anims[ rand() % anims.size() ];
        Log::Debug( "WorkshopFiveGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );

        if ( !m_bWiredPi )
        {
            wiringPiSetup();
            pinMode(m_PinNumber, OUTPUT);
            m_bWiredPi = true;
        }
        for(size_t i=0;i<5;++i)
        {
            digitalWrite(m_PinNumber, HIGH);
            delay(200);
            digitalWrite(m_PinNumber, LOW);
            delay(200);
        }
    }
    else
    {
        Log::Warning( "WorkshopFiveGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
    }
    
    ```

Now press `command + s` to save the edits. 

### Preparing for Windows

1.	**On your local machine** clone or download the self-sdk (make sure you are on the self-sdk master branch) from [Github](https://hub.jazz.net/project/wlabs/self-sdk). If you downloaded this file, unzip it.

2.	Navigate into:
	`wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Start/`
	
	In this directory you should find these two files:
	
	`WorkshopFiveGesture.cpp` 
	`WorkshopFiveGesture.h`
	
	Click on and copy (`Ctrl + C`) these files . 
	
3.	In your computer’s home directory create a new folder called `workshop_five`.
   
	Then within the `workshop_five` folder, create a folder called `gestures`. 
	
	Within the `gestures` directory paste the `WorkshopFiveGesture.cpp` and `WorkshopFiveGesture.h` files you copied, here.

4. Now navigate into `wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Snippets/` and locate the `WorkshopFiveCodeSnippets.txt` file.
   
   Copy all the contents in `WorkshopFiveCodeSnippets.txt`.

   Open the `workshop_five/gesture/WorkshopFiveGesture.cpp` source file (in the `workshop_five/gestures` folder you just created) and locate the method `DoAnimateThread()`. Into the body of this function paste the code from the `WorkshopFiveCodeSnippets.txt`. Paste this inside the function body **{}** of **DoAnimateThread()** in `WorkshopFiveGesture.cpp` located inside your **gestures** directory.(This pasted into the brackets under `void WorkshopFiveGesture::DoAnimateThread(WorkshopFiveGesture::Request * a_pReq)` ). The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues. Copy the code and overwrite the all code already in **DoAnimateThread()**.

  ```
    std::vector<AnimationEntry> anims;
    for(size_t i=0;i<m_Animations.size();++i)
    {
        anims.push_back( m_Animations[i] );
    }

    if ( anims.size() > 0 )
    {
        srand( (unsigned int)Time().GetMilliseconds() );
        const AnimationEntry & entry = anims[ rand() % anims.size() ];
        Log::Debug( "WorkshopFiveGesture", "Gesture %s is running behavior %s.", m_GestureId.c_str(), entry.m_AnimId.c_str() );

        if ( !m_bWiredPi )
        {
            wiringPiSetup();
            pinMode(m_PinNumber, OUTPUT);
            m_bWiredPi = true;
        }
        for(size_t i=0;i<5;++i)
        {
            digitalWrite(m_PinNumber, HIGH);
            delay(200);
            digitalWrite(m_PinNumber, LOW);
            delay(200);
        }
    }
    else
    {
        Log::Warning( "WorkshopFiveGesture", "No valid animations found for gesture %s", m_GestureId.c_str() );
    }
    
    ```

5.	Navigate to `/workshop_five` folder (that you created in your home directory) and create a text file called `CMakeLists.txt` and open this file. 

    In this file add in the following text:

```
include_directories(. wiringPI)
SET(GCC_COVERAGE_LINK_FLAGS "-lwiringPi")
add_definitions(${GCC_COVERAGE_LINK_FLAGS})

file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
qi_create_lib(workshop_five_plugin SHARED ${SELF_CPP})
qi_use_lib(workshop_five_plugin self wdc)
qi_stage_lib(workshop_five_plugin)

target_link_libraries(workshop_five_plugin wiringPi)

```
	
	
 And save the file. 

## Updating Raspberry Pi with LED gesture.
6.	Now move your `wlabs_self-sdk-master/examples/workshop_five` directory from your local machine to your Pi. 
   * For Mac, use Terminal. Run `cd wlabs_self-sdk-master/examples` and then:
 `scp -r workshop_five pi@[IPaddress]:~/self/self-sdk-develop/examples`

   * For PC, use Filezilla (if don't already have Filezilla you can [download it here](https://filezilla-project.org/) ).
	Navigate to `self/self-sdk-develop/examples/` on the remote site side of the screen.
	Navigate to the `self/wlabs_self-sdk-master/examples/` directory that you just created on the local site side of the screen.
	Drag your examples directory from the local side to the remote site to copy the directory to your Pi.

7.	Now ssh to the Pi using Terminal for Mac (or Putty for Windows):

	`ssh pi@[ip address]`	

  	Run `cd /home/pi/self/self-sdk-develop/examples`

8.	Now edit the `CMakeLists.txt` file in this directory by using:

	`nano CMakeLists.txt`

   and add the line at the end of the file:

	`add_subdirectory(workshop_five)`

   Now save this `CMakeLists.txt` file.

## Updating the body.json configuration.
10. Note: If you have updated the body.json with `EmbodimentCreds` in Workshop 1, please skip and proceed to Step 3. 

	On your local machine, open a browser and sign into [Intu Gateway](rg-`gateway.mybluemix.net`).
 		
 Select `View Credentials` from the link on the left hand side of the page
 		* On the View Credentials page, use the filters to select your organization and group. Click the Get Credentials button.
Click the Copy link to save the entire contents to your clipboard.

9. Retrieve the body.json from your Pi to your local machine.
		 `scp pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/body.json ~/`

	And open this `body.json` file. 

	* Edit the `body.json` that you opened for editing by adding the credentials you just retrieved from the gateway to it. The following steps describe how to do this.
	* In the `body.json`, search on `"m_EmbodimentCreds"` to jump to the correct section.
	* Then highlight/select everything from `"m_EmbodimentCreds"` up to and including `"m_OrgId": ""},`.Note: Don’t forget to select the comma.
	* Click your delete key on your keyboard.
	* Then paste the credentials that you copied to your clipboard into the section just deleted. 

11. 
	In the `body.json` search for the `m_Libs` variable and change it to read:
 `"m_Libs":["platform_raspi", "workshop_five_plugin"]`
   * Save the revised body.json file to your machine and transfer to the Pi with the following instructions.
		* On a Mac
			* i. Close the `body.json` file.
			* ii.	Open a terminal and use the following command to copy your saved body.json from your local directory back to your pi
`scp ~/body.json pi@[ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/`
		* On a PC
			* i. Exit the window. If you are using Filezilla, you will be prompted in a window to upload the file back on the server. 
			* ii.	Click the Yes button. This action saves your changes to your Raspbery Pi.

10.	Build Self on your Raspberry Pi with the following steps:

	a.	In your session ssh’ed into the Raspberry Pi, navigate into the self-sdk-develop directory: `cd self-sdk-develop`
	
	b.	Mark the build script as executable by running: 
`chmod +x scripts/build_raspi.sh`

	c.	Now build by using: 
		`scripts/build_raspi.sh`
		(*NOTE: If you have any build errors, build `scripts/clean.sh`)

## Run Self on your Raspberry Pi
Run Self on your Raspberry Pi by completing the following steps in your ssh window (Terminal for Mac or Putty for Windows). 

Note: The following steps will need to be repeated each time you power your Raspberry Pi back up (i.e. unplug power and plug it back in).

1.	First, verify that if you have a HDMI cable plugged, that the sound in turned to analog. This can be done by right clicking the speaker at the top right hand corner on the Raspberry Pi homescreen and clicking `analog`. 
2.	Verify that you have a microphone and speaker plugged into your pi. Your speaker may need to be charged first. It will need to be plugged in and on before proceeding to the next step.
4.	Navigate to the raspi directory using: `cd /home/pi/self/self-sdk-develop/bin/raspi`.
5.	Run: `export LD_LIBRARY_PATH=./`
6.	Run: `export WIRINGPI_GPIOMEM=1`
7.	Run: `./self_instance `

NOTE: Each time you want to run Intu you must run

```
	`export LD_LIBRARY_PATH=./`
	`export WIRINGPI_GPIOMEM=1`
	`./self_instance `
```

You have now added a gesture for the LED light.  When you say, "can you laugh" or "tell me a joke" to the robot, the LED light should blink i.e. you have added the blinking of the LED to Intu as a gesture. 


### What did we learn?
When Intu is asked "can you laugh" or "tell me a joke" and the Blackboard receives a [emote=show_laugh], how does Intu know that the LED gesture should be executed?

It is from the configuration file `raspi.anims`, in `Intu/wlabs_self-sdk-master/bin/raspi/etc/gestures`. (More to come).

# APPENDIX

##Steps for putting Intu on your Raspberry Pi using an SD card with the Intu image

**Note:** In this workshop, commands are issued from **Terminal** on **Mac** or **PuTTY** on **Windows**. For **Windows** users, if you do not have **PuTTY** installed already, you can download it using this [link](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).

1.	Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Raspberry Pi.

2.	You should see a window open on your monitor. Click on the **Wifi networks (w)** icon at the top of the window, select your network (at DevCon, it will be **ROBOT_PED1**), and enter your password (**panda$123**).

3.	Get the IP address of your Raspberry Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in **ifconfig** and hit Enter.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your Raspberry Pi (e.g. 10.0.1.2). 

4.	Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.
	
	For **Mac** users, open a new Terminal window, and connect to the Raspberry Pi using: `ssh pi@{ip_address}` (e.g. ssh pi@10.0.1.2)

	You will be prompted for a password. The default password for the Raspberry Pi is **raspberry**.

	For **Windows** users, open **PuTTY**, type in the Raspberry Pi's IP address, and when prompted, type in the username (**pi**) and password (**raspberry**) for the Raspberry Pi.

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. 

6. Check that your laptop is on the same network as the Raspberry Pi. Reboot the Raspberry Pi by running `sudo reboot` in your Terminal window or PuTTY. 

1.	Now skip down and complete the following sections from this workshop:
	1.	**Edit your Body.json with your Intu credentials**
	2.	**Run Self on your Raspberry Pi**

## Install Raspbian Operating System onto your Raspberry Pi

1. Navigate to the [NOOBS Download page](https://www.raspberrypi.org/downloads/noobs/).

2. Click on the **Download ZIP** button under ‘NOOBS (offline and network install)’. **Note:** This may take an hour or more depending on your network speed. 

3. Extract the files from the zip to a local directory. 

4. Open the "Read Me" text file in the extracted folder and follow the instructions to format your SD card.

5. Once your SD card has been formatted, drag all the files within the extracted NOOBS folder and drop them onto the SD card.

6. The necessary files will then be transferred to your SD card.

7. When this process has finished, safely eject the SD card from your computer, remove the mini SD card from it, and insert it into your Raspberry Pi.

8. Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Raspberry Pi.

9.	You should see a window open on your monitor. Click on the **Wifi networks (w)** icon at the top of the window, select your network (at DevCon, it will be **ROBOT_PED1**), and enter your password (**panda$123**).

10. As this is the first time your Raspberry Pi and SD card have been used, you'll have to select an operating system and let it install. Select **Raspbian [RECOMMENDED]**.

11. At the bottom of your screen, you will see a **Language** panel for your keyboard. Select **English (US)**. 

12.	Click on the **Install** icon on the top left of the window. A **Confirm** window will appear; select **Yes**. The installation may take up to 30-40 mins. 

	**Note:** If your installation freezes at any point (as indicated by a frozen progress bar), **unplug** the Raspbberry Pi from its power source and **plug it back in**, and repeat from step 8.

13.	Check that you are connected to the network. On the right of the Bluetooth icon in the top right corner of the screen, you will see a **successful connection** to your wifi network as shown by a **blue wifi icon**, or an **icon with two red crosses** if you have been **disconnected**. To reconnect, click on the two red crosses, select your network, and type in the password when prompted.

14. Get the IP address of your Raspberry Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in **ifconfig** and hit Enter. **Note:** If this command does not work, **unplug** the Raspberry Pi from its power source and **plug it back in**, and repeat this step.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your Raspberry (e.g. 10.0.1.2). 

4.	Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.
	
	For **Mac** users, open a new Terminal window, and connect to the Pi using: `ssh pi@{ip_address}` (e.g. ssh pi@10.0.1.2)

	You will be prompted for a password. The default password for the Raspberry Pi is **raspberry**.

	For **Windows** users, open **PuTTY**, type in the Raspberry Pi's IP address, and when prompted, type in the username (**pi**) and password (**raspberry**) for the Raspberry Pi.

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. 

6. Check that your laptop is on the same network as the Raspberry Pi. Reboot the Raspberry Pi by running `sudo reboot` in your Terminal window or PuTTY. 

## Set up your Raspberry Pi for Builds

**Note:** If any step below fails or errors, run: `sudo apt-get update`, then repeat the step.

1.	Open up a new browser window on your laptop and download [**Anaconda 4.2.0 For Linux Python 2.7 version**](https://www.continuum.io/downloads).

	**Make sure you download the correct version.** You need the LINUX version no matter what operating system you have. Windows users may have to right click and select **Save as** to save the download locally.

2.	Copy Anaconda from your laptop over to the Raspberry Pi. **Windows** users may need to [download](https://filezilla-project.org/) and use something like **Filezilla** to copy files over to the Raspberry Pi.
	1. Navigate to the directory where you downloaded Anaconda locally. 
	
		
		
	2. Copy Anaconda from your laptop over to the Raspberry Pi using the following command: `scp Anaconda2-4.2.0-Linux-x86.sh pi@{ip}:/home/pi` 
	
		(e.g. scp Anaconda2-4.2.0-Linux-x86.sh pi@10.0.1.2:/home/pi)
		
		If prompted, the username is **pi** and password is **raspberry**
		

3.	Install Anaconda on your Raspberry Pi and set up qiBuild.
	1. In a new Terminal/PuTTY window, ssh into your Raspberry Pi: `ssh pi@{ip_address}`. When prompted, the username is **pi** and password is **raspberry**.

	2.	Run: `bash Anaconda2-4.2.0-Linux-x86.sh`.
	3. Follow the steps on the screen to install Anaconda. When you get to the license, keep hitting Enter to jump to the bottom. Type **yes** to approve the license.
	4.	Hit Enter to install Anaconda in the default location. **Note**: It may take a while for the progress to update, and if you get the following error, please ignore it.

		```
Anaconda2-4.2.0-Linux-x86.sh: line 484: /home/pi/anaconda2/pkgs/python-3.5.2-0/bin/python: cannot execute binary file: Exec format error
ERROR:
cannot execute native linux-32 binary, output from 'uname -a' is:
Linux raspberrypi 4.4.21-v7+ #911 SMP Thu Sep 15 14:22:38 BST 2016 armv7l GNU/Linux

		```

	5. Once Anaconda has successfully installed, run: `sudo apt-get install python-pip cmake`. 
	
		**Note:** If this fails, run `sudo apt-get update`, and then rerun: `sudo apt-get install python-pip cmake`.

	6.	Run: `sudo pip install qibuild`
 

4.	Install the wiringPi library on the Raspberry Pi.
	1. In a new Terminal/PuTTY window, ssh into your Raspberry Pi: `ssh pi@{ip_address}`. 
	2.	Navigate to your Raspberry Pi's **home directory** by running: `cd /home/pi`. 
	3.	Run: `git clone git://git.drogon.net/wiringPi`
	4.	Now navigate into the wiringPi directory by running: `cd wiringPi/`
	5.	Run: `./build`

	You should see a list of classes compiled and "All Done" at the end.
	
Finally, in a new Terminal/PuTTY window, ssh into your Raspberry Pi, navigate to its home directory by running `cd /home/pi`, and in here, create a new directory called **self**: `mkdir self`. This directory will be used in the steps below.

##Retrieve the latest self-sdk and build Self on your Raspberry Pi

1. [Download the Self SDK](https://github.ibm.com/watson-labs-austin/self-sdk). Click on **Clone or download** and select **Download Zip**.

2. Copy the zip file from your local machine across to the Raspberry Pi.

	**For Mac users:**

	1. In a Terminal window, navigate to the directory where you downloaded the zip file, and copy it across to the newly created **self** directory on the Raspberry Pi using: `scp self-sdk-develop.zip pi@{ip}:/home/pi/self/`
	
	**For Windows users:**

	1.	Open Filezilla and connect to your Raspberry Pi.
	2. In the **Local Site**** side of the screen, navigate to the `self-sdk-develop.zip` file.
	3.	In the **Remote Site** side of the screen, navigate to the directory: **/home/pi/self**
	4.	Click on the file `self-sdk-develop.zip` on the **Local Site** side of the screen and drag it to the **Remote site** side of the screen. 
	5.	You can monitor the progress of the transfer in the panel located at the bottom of the Filezilla screen.


4. Unzip the `self-sdk-develop.zip` file into the **self** directory of your Raspberry Pi. 
 	1.	Navigate to the **self** directory on your Raspberry Pi. You can open a new Terminal/PuTTY window as before, ssh into your Raspberry Pi, and run: `cd /home/pi/self`. If your prompt reads: `pi@raspberrypi:~/self $`, this confirms that you are in the **self** directory.
	2.	Run the following command: `unzip self-sdk-develop.zip`

5. Build Self on your Raspberry Pi with the following steps:

	1.	Navigate into the **self-sdk-develop** directory on your Raspberry Pi: `cd self-sdk-develop`
	2.	Mark the build script as executable by running: `chmod +x scripts/build_raspi.sh`
	3.	**Note:** If you have run the **below** command before, first run: `scripts/clean.sh`. 
	
		Run: `scripts/build_raspi.sh`

## Edit your `body.json` file with your Intu Credentials

1. Retrieve the `body.json` file from your Raspberry Pi for editing.

	**For Mac users:**
	1. Open a new Terminal window and run: `scp pi@{pi_ip_address}:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/body.json ~/`. This copies across the `body.json` file to your **home** directory.
	
	2. Open the local copy of this `body.json` file with your favourite text editor.
	
	**For Windows users:**

	1. Open Filezilla and connect to your Raspberry Pi.
	2.	On the **Remote site** side of the Filezilla screen, navigate to **/home/pi/self/self-sdk-develop/bin/raspi/etc/profile**.
	3. Locate the `body.json` file in the **profile** directory, right click and select **View/Edit**.

2. Retrieve the credentials for your Organization in the Intu Gateway.
	1. Log in to the [Intu Gateway](https://rg-gateway.mybluemix.net/). You should already have completed Workshop 1 and created an Organization and Group.
	2.  Click on **VIEW CREDENTIALS** in the left hand navigation bar.

	3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.

	4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file in your text editor.

 
3. Edit `the body.json` that you opened for editing by adding the credentials you just retrieved from the Intu Gateway.
	1.	In the `body.json` file, search for `"m_EmbodimentCreds"` to jump to the correct section.
	2.	Highlight/Select the entire section of the file from `"m_EmbodimentCreds"` to `""m_OrgId": ""},`
	3.	Delete this section of the file.
	4.	Now paste the credentials you saved from the Intu Gateway into this section of the `body.json` file. 

4. Copy the edited `body.json` file across to the Raspberry Pi.

	**For Mac users:**

	1. After saving your changes to the `body.json` file, open a new Terminal window and run the following command to copy it across to the Raspberry Pi: `scp ~/body.json pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/`

	**For Windows users:**
	
	1.	Exit the window you were using to edit the `body.json` file. If you are using Filezilla, you will be prompted to upload the file back on the server. 
	2.	Click the **Yes** button. This action saves your changes to your Raspberry Pi.

