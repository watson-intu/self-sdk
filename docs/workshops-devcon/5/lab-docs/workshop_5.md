# Workshop 5 – Raspberry Pi

In this workshop, you ?????. 

**Before you begin:** 

1. You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!. After completing Workshop 1, you will have:

	1. Your own account, Organization, Group and Parent on the [Intu Gateway](rg-`gateway.mybluemix.net`)
	2. Your own [Intu Gateway](rg-`gateway.mybluemix.net`) credentials 

2. You must have the following to complete Workshop 5:
	* Raspberry Pi 3 with power cable

	* Anker Bluetooth Speaker with power cable and 3.5mm audio cable
	* USB Mini Microphone
	* PiCamera for the Raspberry Pi 3
	* LED, diode, 3 female-female jumper wires
	* Monitor (with a HDMI connection)
	* Keyboard and Mouse (with USB connections)
	* An imaged 32 GB SD card  

	**Note:** If you **do not** have an imaged card from Devcon, please go to the Appendix.	

3. You should have **assembled the Raspberry Pi** using the instructions from: [Workshop 5 Lab Docs - Assembling the Raspberry Pi.pdf](https://hub.jazz.net/project/wlabs/self-sdk/overview#https://hub.jazz.net/git/wlabs%252Fself-sdk/contents/master/docs/workshops-devcon/5/lab-docs/Assembling%2520the%2520Raspberry%2520Pi.pdf)

**Note:** In this workshop, commands are issued from **Terminal** on **Mac** or **PuTTY** on **Windows**. For **Windows** users, if you do not have **PuTTY** installed, you can download it using this [link](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html). **Windows** users may also prefer to use a file management tool like **Filezilla** or similar to manage file transfer between their local machine and the Raspberry Pi. Filezilla can be downloaded using this [link](https://filezilla-project.org/).

Complete the following tasks:

1. Set up the Wi-Fi connection for your Raspberry Pi
2. Download the Self Sdk onto your computer and add in the code for the LED gesture
3. Updating your Raspberry Pi with the LED gesture
4. Updating the body.json configuration
5. Run Intu on your Raspberry Pi

## Set up the Wi-Fi connection for your Raspberry Pi

1. Insert your SD card into your Raspberry Pi if you have not done so already.

2. Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Raspberry Pi.

3. You should see a window open on your monitor. Click on the **Wifi networks (w)** icon at the top of the window, select your network (at DevCon, it will be **ROBOT_PED1**), and enter your password (**panda$123** for ROBOT_PED1).

4.	Get the IP address of your Raspberry Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in `ifconfig` and hit Enter.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your Raspberry Pi (e.g. 10.0.1.2). 
        
5.	**Important:** Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.
	
	**For Mac users**: Open a new Terminal window, and connect to the Raspberry Pi using: `ssh pi@{pi's_ip_address}` (e.g. `ssh pi@10.0.1.2`)

	You will be prompted for a password. The default password for the Raspberry Pi is **raspberry**.

	**For Windows users**: Open PuTTY, type in the Raspberry Pi's IP address, and when prompted, type in the username (**pi**) and password (**raspberry**) for the Raspberry Pi.

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. **Do not disconnect the Raspberry Pi from its power source.**

6. Check that your laptop is still on the same network as the Raspberry Pi, and reboot the Raspberry Pi by running `sudo reboot` in your ssh window (Terminal for Mac and PuTTY for Windows). 

**Note:** Unless explicitly stated, all the following steps are to be run on your local computer.

## Download the Self Sdk onto your computer and add in the code for the LED gesture

### Download the Self SDK

1. [Download the Self SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on the **download icon** next to the default **master** branch selected.

2. Create a new directory named **intu** in your **home** directory.

3. Unzip the **wlabs_self-sdk-master.zip** file into **intu**, making sure that you retain the folder structure, i.e. your intu directory should now contain the unzipped **wlabs_self-sdk-master** folder.


### ?????? Preparing the ???? for the LED gesture on OS X
1. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/).

2. In **CLion**, select Open -> home directory -> intu -> wlabs_self-sdk-master and click **OK**. 

	Note that a window may appear prompting you to open your project in a New Window or This Window. Select **New Window**. At the bottom of your CLion window, in the Problems tab, you will see the following Error, which you do not need to worry about:
	
	```
	Error: By not providing "FindSELF.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "SELF", but CMake did not find one.
Could not find a package configuration file provided by "SELF" with any of the following names:
  SELFConfig.cmake   self-config.cmake
Add the installation prefix of "SELF" to CMAKE_PREFIX_PATH or set "SELF_DIR" to a directory containing one of the above files.  If "SELF" provides a separate development package or SDK, be sure it has been installed.
	```

 2i. Inside the CLion **wlabs_self-sdk-master project**, right-click **examples**, select **New**, and select **Directory**. Type in **workshop_five** for the new directory name, and click **OK**.
 
 2ii. Right-click the `CMakeLists.txt` file in the **examples** directory, and click **Copy**. (If you are unsure of the directory you are in, look in the top-left navigation bar.)
  
 2iii. Right-click the **workshop_five** directory, and click **Paste**. This file helps to build the plugin for the the LED gesture.

 2iv. Open the `CMakeLists.txt` file in the **workshop_five** directory, and overwrite all of its contents with the following code:

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

3. Create a new directory inside **workshop_five** called **gestures**.

4. Locate the Workshop 5 code snippet files **to be filled in** in:

 `wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_start`

5. Copy the `WorkshopFiveGesture.cpp` and the `WorkshopFiveGesture.h` files and paste them into the **gestures** directory that you created.


	* Open the `WorkshopFiveGesture.cpp` file, which contains the following functions that enable the gesture you'll create:

	* The Execute, Abort, AnimateThread and AnimateDone functions are already completely built out.

	* In the next step, you will build out the **DoAnimateThread** function using the example code provided.

### ??? Building out the LED gesture function for OS X

1. In **wlabs_****self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFive_Snippets**, you will see the `WorkshopFiveCodeSnippets.txt` file. Open this file and find the **DoAnimateThread** function.

2. Copy the entire contents of `WorkshopFiveCodeSnippets.txt ` for the **DoAnimateThread()** function. Paste this inside the function body **{}** of **DoAnimateThread()** in `WorkshopFiveGesture.cpp` located in your **gestures** directory, directly above the line of code which reads: `Log::Debug("WorkshopFiveGesture", "in DoAnimateThread");`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.


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

3. Save your changes (**Cmd + S**). 

### ???????? Preparing for Windows

1.	Open up File Explorer and navigate to your **home** directory. This should be: **C:\Users\username** ("username" should read your name)

2. In your home directory, create a new directory called **workshop_five**, and a second directory called **gestures** inside **workshop_five**.

3. Navigate to **wlabs_****self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Start/**, where you will find the following two files:
	
	`WorkshopFiveGesture.cpp` 
	
	`WorkshopFiveGesture.h`
	
	
3. Copy `WorkshopFiveGesture.cpp` and `WorkshopFiveGesture.h` from **WorkshopFiveGesture_Start/** to the **gestures** directory which you just created.
	

4. Now navigate to **wlabs_****self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Snippets** and locate the `WorkshopFiveCodeSnippets.txt` file.

 and find the **DoAnimateThread** function.

2. Copy the entire contents of `WorkshopFiveCodeSnippets.txt ` for the **DoAnimateThread()** function. Paste this inside the function body **{}** of **DoAnimateThread()** in `WorkshopFiveGesture.cpp` located in your **gestures** directory, directly above the line of code which reads: `Log::Debug("WorkshopFiveGesture", "in DoAnimateThread");`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.


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

 
5.	Navigate to the **workshop_five** directory you created in your home directory, and create a new text file called `CMakeLists.txt`. 

6. Open this file and add in the following lines of code:

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
	
9. Save your changes (**Ctrl + S**). 

## Updating your Raspberry Pi with the LED gesture

1.	Copy the **workshop_five** directory from your local machine over to your Raspberry Pi. 
   
   **For Mac and Windows users using an ssh window:** 
   1. Open a new ssh window and navigate to the **examples** directory (the parent directory of workshop_five) by running: `cd intu/wlabs_self-sdk-master/examples`
	2. Run: `scp -r workshop_five pi@{IPaddress}:~/self/self-sdk-develop/examples`

	**For Windows users using Filezilla:** 
	
   1. Open Filezilla.
	2. Navigate to **self/self-sdk-develop/examples/** on the **Remote site** side of the screen.
	Navigate to the **intu/wlabs_self-sdk-master/examples/** directory on the **Local site** side of the screen.
	3. Drag your **examples** directory from the **Local site** to the **Remote site** to copy the directory across to your Raspberry Pi.

2.	ssh to the Raspberry Pi in a new ssh window:

	1. Run:`ssh pi@{pi's_ip_address}`	
  	2. Run `cd /home/pi/self/self-sdk-develop/examples`

3.	Edit the `CMakeLists.txt` file in the examples directory you're currently in.

	1. Run: `nano CMakeLists.txt`
   2. Carefully add the following line at the end of the file:		`add_subdirectory(workshop_five)`
   3. Save your changes to the `CMakeLists.txt` file. 
   		1. Use **Ctrl + X** to `Exit`.
   		2. When prompted with: `Save modified buffer (ANSWERING "No" WILL DESTROY CHANGES) ? `, type **Y** for **Yes**. 
   		3. When prompted with: `File Name to Write: CMakeLists.txt`, hit **Return** or **Enter** to finalise your changes.

## Updating the `body.json` configuration

### Retrieving the credentials for your Organization in the Intu Gateway

**Note:** If you have updated your `body.json` file with your `EmbodimentCreds` from the [Intu Gateway](rg-`gateway.mybluemix.net`) in Workshop 1, please skip down to **Configuring your `body.json` file**. 


1. [Log in to the Intu Gateway](https://rg-gateway.mybluemix.net/). 

2. Click on **VIEW CREDENTIALS** in the left hand navigation bar.

3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.

4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file using your favorite text editor.




### Configuring your `body.json` file

**For Mac and Windows users using a ssh window:**

1. Copy the `body.json` from your Raspberry Pi to your local machine by running the following command in a new ssh window:
		 `scp pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/body.json ~/`

	Note that this copies the `body.json` file to your **home** directory.

1. Open your `body.json` file using your favorite text editor. 

2. Locate the `m_Libs` variable, and change it to read: `"m_Libs":["platform_raspi", "workshop_five_plugin"]`

3. Locate `"m_EmbodimentCreds":{ ... }`, and replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in step 4 of the previous section.

4. Save your changes and close the file.

5. Now copy your newly edited `body.json` from your local machine to the Raspberry Pi by running the following command from a **new** Terminal or PuTTY window: 
`scp ~/body.json pi@{pi'sip_address}:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/`


**For Windows users using Filezilla:**

1. Open Filezilla and connect to your Raspberry Pi.

2. On the **Remote site** side of the Filezilla screen, navigate to **/home/pi/self/self-sdk-develop/bin/raspi/etc/profile**.
3. Locate the body.json file in the profile directory, and right click and select View/Edit.
2. Locate the `m_Libs` variable, and change it to read: `"m_Libs":["platform_raspi", "workshop_five_plugin"]`

3. Locate `"m_EmbodimentCreds":{ ... }`, and replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway the previous section.

4. Exit the window you were using to edit the `body.json` file, upon which you will be prompted to upload the file back onto the server.

5. Click **Yes**. This action saves your changes to your Raspberry Pi.

### Building the Self SDK on your Raspberry Pi

1. In your current (or a new) ssh session to the Raspberry Pi, navigate to the **self-sdk-develop** directory: `cd self-sdk-develop`
	
2.	Mark the build script as executable by running: 
`chmod +x scripts/build_raspi.sh`

3.	Now build the Self SDK by running: `scripts/build_raspi.sh`
	
	**Note:** If you have any build errors, run: `scripts/clean.sh` and then rerun: `scripts/build_raspi.sh`

## Run Intu on your Raspberry Pi
Run Intu on your Raspberry Pi by completing the following steps in your ssh window. 

**Note:** The following steps will need to be repeated each time you power up your Raspberry Pi (i.e. unplug and plug back in the power source to your Raspberry Pi).

1.	If you have a HDMI cable plugged into your Raspberry Pi, verify that the sound is set to **analog**. This can be done by **right clicking** the **speaker icon** at the top right hand corner of the Raspberry Pi's homescreen, and selecting **analog**. 

2.	Verify that you have a microphone and speaker plugged into your Raspberry Pi. Note that your speaker may need to be charged before use. Make sure that it is turned on before proceeding with the next step.
4.	Navigate to the **raspi** directory using: `cd /home/pi/self/self-sdk-develop/bin/raspi`.
5.	Run: `export LD_LIBRARY_PATH=./`
6.	Run: `export WIRINGPI_GPIOMEM=1`
7.	Run: `./self_instance `

**Note:** Each time you want to run Intu, you must follow steps 4-6 each time:

```
	export LD_LIBRARY_PATH=./
	export WIRINGPI_GPIOMEM=1
	./self_instance 
```

You have now added a gesture for the LED light.  When you say, "Can you laugh?" or "Tell me a joke" to the robot, the LED light should blink, i.e. you have added the blinking of the LED to Intu as a gesture. 


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

	**Note:** If your installation freezes at any point (as indicated by a frozen progress bar), **unplug** the Raspberry Pi from its power source and **plug it back in**, and repeat from step 8.

13.	Check that you are connected to the network. On the right of the Bluetooth icon in the top right corner of the screen, you will see a **successful connection** to your wifi network as shown by a **blue wifi icon**, or an **icon with two red crosses** if you have been **disconnected**. To reconnect, click on the two red crosses, select your network, and type in the password when prompted.

14. Get the IP address of your Raspberry Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in **ifconfig** and hit Enter. **Note:** If this command does not work, **unplug** the Raspberry Pi from its power source and **plug it back in**, and repeat this step.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your Raspberry (e.g. 10.0.1.2). 

4.	Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.
	
	For **Mac** users, open a new Terminal window, and connect to the Raspberry Pi using: `ssh pi@{ip_address}` (e.g. ssh pi@10.0.1.2)

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

