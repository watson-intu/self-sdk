# Workshop 5.5 – Using Intu to Make a TJBot's Arm Wave

In this workshop, you will assemble your own Raspberry Pi, which is a credit card-sized computer. You will then program an Move Joint gesture for Intu, and run Intu off of your Raspberry Pi and see your TJBot's arm in action.

**Before you begin:** 

1. You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!. After completing Workshop 1, you will have:
2. Your own account, Organization, Group and Parent on the [Intu Gateway](rg-`gateway.mybluemix.net`)
3. Your own [Intu Gateway](rg-`gateway.mybluemix.net`) credentials 
4. You must have the following to complete Workshop 5.5:
    1. Raspberry Pi 3 with power cable
    2. Anker Bluetooth Speaker with power cable and 3.5mm audio cable
    3. USB Mini Microphone
    4. Monitor (with a HDMI connection)
    5. Keyboard and Mouse (with USB connections)
    6. An imaged 32 GB SD card (16 GB would also work)
5. You will notice that Intu and Self are used interchangeably. Self is the code name for Intu.  

**Notes:** 

1. If you **do not** have an pre-imaged card we released at the Watson Devcon, please go to the **Appendix** after assembling your Raspberry Pi. (This steps are not optional for an new SD card)	
2. In this workshop, commands are issued from **Terminal** on **Mac** or **PuTTY** on **Windows**. For **Windows** users, if you do not have **PuTTY** installed, you can download it using this [link](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html). **Windows** users will also require a file management tool to copy files over a network between their local machine and the Raspberry Pi. You can use a stand-alone tool like **Filezilla**, or you may prefer scp via Putty. **Filezilla** can be downloaded using this [link](https://filezilla-project.org/).

In this lab you will complete the following tasks:

1. [Assembling the Raspberry Pi](#assembling-the-raspberry-pi)
2. [Set up the Wi-Fi connection for your Raspberry Pi](#set-up-the-wi-fi-connection-for-your-raspberry-pi)
3. [Download the Self SDK onto your computer and add in the code for waving the arm gesture](#download-the-self-sdk-onto-your-computer-and-add-in-the-code-for-the-led-gesture)
4. [Updating your Raspberry Pi with the Move Joint Gesture](#updating-your-raspberry-pi-with-the-led-gesture)
5. [Updating the body.json configuration](#updating-the-body.json-configuration)
6. [Run Intu on your Raspberry Pi](#run-intu-on-your-raspberry-pi)

## 1. Assembling the Raspberry Pi

### A. Speaker

1. Before assembling your Raspberry Pi, charge your speaker by connecting it to your laptop using the USB to micro-USB cable. The indicator light will be red while the speaker is charging and blue when fully charged.

![Speaker charging.](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5/lab-docs/speaker_charging_in_laptop.png?raw=true)

2. After your speaker is charged, connect it to your Raspberry Pi using the 3.5mm audio cable.

![Speaker plugged into your Raspberry Pi.](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5/lab-docs/speaker_in_pi.png?raw=true)

### C. Microphone

Plug the USB microphone into any one of the USB ports of your Raspberry Pi.

![Microphone](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5/lab-docs/microphone.png?raw=true)

### D. Connecting the Servo Motor

As part of this lab we will be using the Tower Pro SG90 micoservo. You can see the pin out of this servo motor ![here](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5.5/lab-docs/SG90Servo.pdf). You will need to connect the servo motor to the Raspi Board as below:

![Board Layout for Servo](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5.5/lab-docs/sevo_pin_layout.png?raw=true)

### E. Power

To power up your Raspberry Pi, connect the power cable to your Raspberry Pi as shown in the image below. 

![Power cable for Raspberry Pi.](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5/lab-docs/pi_charger.png?raw=true)

### F. Connecting the Raspberry Pi to an external monitor, keyboard and mouse

Connect your Raspberry Pi to an external monitor, keyboard and mouse as shown in the image below.

![Raspberry Pi and external connections.](https://github.ibm.com/watson-labs-austin/self-sdk/blob/develop/docs/workshops-devcon/5/lab-docs/external_monitor_keyboard_to_pi.png?raw=true)

## 2. Set up the Wi-Fi connection for your Raspberry Pi
1. Insert your SD card into your Raspberry Pi if you have not done so already.
2. Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Raspberry Pi.
3. You should see a window open on your monitor. Sometimes it might so happen that your power strip might not work correctly. If your Pi does not start, plug it directly into a wall socket. Click on the **Wifi networks (w)** icon at the top of the window, select your network.
4.	Get the IP address of your Raspberry Pi.
5.	Click on the black **Terminal** icon on the top left toolbar.
6.	Type in `ifconfig` and hit Enter.
7.	Look for the **wlan0** section. The **inet addr** gives you the IP address of your Raspberry Pi (e.g. 10.0.1.2). 
8.	**Important:** Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.

**For Mac users:**
1. Open a new Terminal window, and connect to the Raspberry Pi using: `ssh pi@{pi's_IP_address}` (e.g. ssh pi@10.0.1.2)
2. You will be prompted for the Raspberry Pi's password. The default password is **raspberry**.

**For Windows users:** 

1. Open a new PuTTY window and type in the Raspberry Pi's IP address. 
2. You will be prompted for the Raspberry Pi's username and password. The default username is **pi** and default password is **raspberry**.

6. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. **Do not disconnect the Raspberry Pi from its power source.**

7. Check that your laptop is still on the same network as the Raspberry Pi, and reboot the Raspberry Pi by running `sudo reboot` in your SSH window (Terminal for Mac and PuTTY for Windows). Try SSHing back into the Pi - when you can, the Pi is back up and running.

**Note:** Unless explicitly stated, all of the following steps are to be run on your local computer.

## 3. Download the Self SDK onto your computer and add in the code for the move joint gesture

### A. Download the Self SDK

1. [Download the Self SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on the **download icon** next to the default **master** branch selected.

2. Create a new directory named **intu** in your **home** directory.

3. Unzip the **wlabs_self-sdk-master.zip** file into **intu**, making sure that you retain the folder structure, i.e. your intu directory should now contain the unzipped **wlabs_self-sdk-master** folder. This may take some time.

### B. Creating  the Waving Arm Gesture with INTU
1. To create this in the Raspberry Pi we are going to recreate the folder structure under **examples** that is found in this repo under self-sdk/docs/workshops-devcon/5.5/code-snippets/TJBotWave_Final. 
2. We have found the CLion IDE to be an nice environment for this type of work. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/). (Alternatively, You may also use your favorite text editor and just follow the steps below.)
3. Inside the **wlabs_self-sdk-master** project navigate to the **examples** directory. Inside here make an new directory called **move_arm_joint**
4. Edit the `examples/CMakeLists.txt` file to look like:
```
include_directories(".")

add_subdirectory(move_arm_joint)

```
5. Next we will move the files from **docs/workshops-devcon/5.5/code-snippets/TJBotWave_Final/examples/move_arm_joint/** into the **move_arm_joint** directory. There should be 3 new files: CMakeLists.txt, RaspiMoveJointGesture.cpp, and RaspiMoveJointGesture.h now inside the **move_arm_joint** folder.
6. Take a few moments to look at all three of these files as they are the core of the code that acts as a plugin to INTU allowing it to control the Raspberry Pi's GPIO pins (with PWM) allowing the arm to move. 
 
## 4. Updating your Raspberry Pi with the move joint gesture

1.	Copy the entire **examples** directory from your local machine over to your Raspberry Pi. (This includes the move_arm_joint directory)

**For Mac users:** 
1. Open a new terminal window and navigate to the **examples** directory (the parent directory of move_arm_joint) by running: `cd intu/wlabs_self-sdk-master/`
2. Run: `scp -r examples pi@{IPaddress}:~/self/self-sdk-master/`

**For Windows users:** 

1. Open Filezilla and connect to your Raspberry Pi. 
1. In the **Host** field, specify your Raspberry Pi's IP address.
2. In the **Username** field, specify your Raspberry Pi's username (**pi**).
3. In the **Password** field, specify your Raspberry Pi's password (**raspberry**).
4. In the **Port** field, specify **22**.  	
2. Navigate to **self/self-sdk-master/** on the **Remote site** side of the screen.

3. Navigate to the **intu/wlabs_self-sdk-master/** directory on the **Local site** side of the screen.

4. Drag your **examples** directory from the **Local site** to the **Remote site** to copy the directory across to your Raspberry Pi. You can monitor the progress of the transfer in the panel located at the bottom of the Filezilla screen.

## 5. Updating the `body.json` configuration

### A. Retrieving the credentials for your Organization in the Intu Gateway

**Note:** If you have updated your `body.json` file with your `EmbodimentCreds` from the [Intu Gateway](rg-`gateway.mybluemix.net`) in Workshop 1, please skip down to **Configuring your `body.json` file**. 

1. [Log in to the Intu Gateway](https://rg-gateway.mybluemix.net/). 

2. Click on **VIEW CREDENTIALS** in the left hand navigation bar.

3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.

4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file using your favorite text editor.

### B. Configuring your `body.json` file

**For Mac users:**

1. Copy the `body.json` from your Raspberry Pi to your local machine by running the following command in a new SSH window:
`scp pi@[IPaddress]:/home/pi/self/self-sdk-master/bin/raspi/etc/profile/body.json ~/`

Note that this copies the `body.json` file to your **home** directory.

2. Open your `body.json` file using your favorite text editor. 

3. Locate the `m_Libs` variable, and change it to read: `"m_Libs":["platform_raspi", "move_joint_plugin"]`

4. Locate `"m_EmbodimentCreds":{ ... }`, and replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in step 4 of the previous section.

5. Save your changes and close the file.

6. Now copy your newly edited `body.json` from your local machine to the Raspberry Pi by running the following command from a **new** Terminal window: 
`scp ~/body.json pi@{IPaddress}:/home/pi/self/self-sdk-master/bin/raspi/etc/profile/`

**For Windows users:**

1. Open Filezilla and connect to your Raspberry Pi.

2. On the **Remote site** side of the Filezilla screen, navigate to **/home/pi/self/self-sdk-master/bin/raspi/etc/profile**.

3. Locate the `body.json` file in the profile directory, and right click and select **View/Edit**.

4. Locate the `m_Libs` variable, and change it to read: `"m_Libs":["platform_raspi", "move_joint_plugin"]`

5. Locate `"m_EmbodimentCreds":{ ... }`, and replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in the previous section.

6. Exit the window you were using to edit the `body.json` file, upon which you will be prompted to upload the file back onto the server.

7. Click **Yes**. This action saves your changes to your Raspberry Pi.

### C. Building the Self SDK on your Raspberry Pi

1. In your current (or a new) SSH session to the Raspberry Pi, navigate to the **self-sdk-master** directory: `cd self-sdk-master`

2.	Mark the build script as executable by running: 
`chmod +x scripts/build_raspi.sh`

3.	Now build the Self SDK by running: `scripts/build_raspi.sh` This may take a while the first time, but after that it will build faster.

**Note:** If you have any **persistent** build errors, run: `scripts/clean.sh` and then rerun: `scripts/build_raspi.sh` You should not need this often. As it takes a while to get it compile again after the clean.sh script.

## 6. Run Intu on your Raspberry Pi

Run Intu on your Raspberry Pi by completing the following steps in your SSH window. 

**Note:** The following steps will need to be repeated each time you power up your Raspberry Pi (i.e. unplug and plug back in the power source to your Raspberry Pi).

1.	If you have a HDMI cable plugged into your Raspberry Pi, verify that the sound is set to **analog**. This can be done by right clicking the **speaker icon** at the top right hand corner of the Raspberry Pi's homescreen, and selecting **analog**. 

2.	Verify that you have a microphone and speaker plugged into your Raspberry Pi. Note that your speaker may need to be charged before use. Make sure that it is turned on before proceeding with the next step.

3.	Navigate to the **raspi** directory using: `cd /home/pi/self/self-sdk-master/bin/raspi`.

4.	Run: `export LD_LIBRARY_PATH=./`

5.	Run: `export WIRINGPI_GPIOMEM=1`

6.	Run: `./self_instance `

**Note:** Each time you want to run Intu, you must follow steps 4-6 each time:

```
export LD_LIBRARY_PATH=./
export WIRINGPI_GPIOMEM=1
./self_instance 
```

You have now added a gesture for moving a joint with INTU.  When you say, "Raise your right arm?" or "Lower your right arm" to the robot, the TJBot should move it arm. 


### Wait, wait, wait, but how does it work?
When Intu is asked "Raise your right arm?" Blackboard receives a [r_hand_raise] by looking at the configuration file located in self-sdk/tree/develop/docs/workshops-devcon/5.5/code-snippets/TJBotWave_Reference/. Under this folder you will find **two** raspi-joints.json files. First a **Natural Language Classifier** instance will decide that the r_hand_raise gesture is being called by some spoken input. Then under **skills/rasip-joints.json** you will see that r_hand_raise maps to a gesture with the same name, r_hand_raise. Under **gestures/rasip-joints.json** we will see concretely how we parameterize the arm movement. 

It is from the configuration file `raspi.anims`, in `Intu/wlabs_self-sdk-master/bin/raspi/etc/gestures`. (More to come).

# Appendix: Instructions for Running Intu on a Raspberry Pi (Without a Pre-Imaged SD Card)

**Note:** Commands are assumed to be issued from **Terminal** on **Mac** or **PuTTY** on **Windows**. For **Windows** users, if you do not have **PuTTY** installed, you can download it using this [link](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html). **Windows** users will also require a file management tool to copy files over a network between their local machine and the Raspberry Pi. You can use a stand-alone tool like **Filezilla**. **Filezilla** can be downloaded using this [link](https://filezilla-project.org/).

## 1. Set up the Wi-Fi connection for your Raspberry Pi

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

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. **Do not disconnect the Raspberry Pi from its power source.**

6. Check that your laptop is on the same network as the Raspberry Pi. Reboot the Raspberry Pi by running `sudo reboot` in your SSH window. 

## 2. Install the Raspbian Operating System on your Raspberry Pi

1. Navigate to the [NOOBS Download page](https://www.raspberrypi.org/downloads/noobs/).

2. Click on the **Download ZIP** button under ‘NOOBS (offline and network install)’. **Note:** This may take an hour or more depending on your network speed. 

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

15.	**Important:** Ensure that your laptop is on the **same** network as your Raspberry Pi from this point onwards.

**For Mac users:**
1. Open a new Terminal window, and connect to the Raspberry Pi using: `ssh pi@{pi's_IP_address}` (e.g. ssh pi@10.0.1.2)
2. You will be prompted for the Raspberry Pi's password. The default password is **raspberry**.

**For Windows users:** 

1. Open a new PuTTY window and type in the Raspberry Pi's IP address. 
2. You will be prompted for the Raspberry Pi's username and password. The default username is **pi** and default password is **raspberry**.

16. At this point, you can disconnect the external monitor, mouse and keyboard from the Raspberry Pi. 

17. Check that your laptop is on the same network as the Raspberry Pi. Reboot the Raspberry Pi by running `sudo reboot` in your SSH window. 

## 3. Setting up your Raspberry Pi for Builds

**Note:** If any step below fails or errors, run: `sudo apt-get update`, then repeat the step.

1.	Open up a new browser window on your laptop and download [**Anaconda 4.2.0 For Linux Python 2.7 version**](https://www.continuum.io/downloads).

**Make sure you download the correct version.** You need the **Linux** version regardless of the operating system that you have. Windows users may have to right click and select **Save as** to save the download locally.


2.	Copy Anaconda from your laptop over to the Raspberry Pi. You will be prompted for the username (**pi**) and/or password (**raspberry**) for the Raspberry Pi.

**For Mac users:**

1. Navigate to the directory where you downloaded Anaconda on your local machine. The file should be named: `Anaconda3-4.2.0-Linux-x86.sh`.

2. Run: `scp Anaconda2-4.2.0-Linux-x86.sh pi@{pi's_IP_address}:/home/pi` 

**For Windows users:**

1. Open Filezilla and connect to your Raspberry Pi. 
1. In the **Host** field, specify your Raspberry Pi's IP address.
2. In the **Username** field, specify your Raspberry Pi's username (**pi**).
3. In the **Password** field, specify your Raspberry Pi's password (**raspberry**).
4. In the **Port** field, specify **22**. 

2. In the **Local site** side of the screen, navigate to the `Anaconda2-4.2.0-Linux-x86.sh` file.

3.	In the **Remote site** side of the screen, navigate to the directory: **/home/pi**

4.	Click on the file `Anaconda2-4.2.0-Linux-x86.sh` on the **Local site** side of the screen and drag it to the **Remote site** side of the screen. You can monitor the progress of the transfer in the panel located at the bottom of the Filezilla screen.

3.	Install Anaconda on your Raspberry Pi and set up the qiBuild.

1. In a new Terminal/PuTTY window, SSH into your Raspberry Pi: `ssh pi@{ip_address}`. You will be prompted for the username (**pi**) and/or password (**raspberry**) for the Raspberry Pi.

2.	Run: `bash Anaconda2-4.2.0-Linux-x86.sh`

3. Follow the steps on the screen to install Anaconda. When you get to the license, keep hitting **Enter** to jump to the bottom. Type **yes** to approve the license.

4.	Hit **Enter** to install Anaconda in the default location. **Note**: It may take a while for the progress to update, and if you get the following error, please ignore it. Proceed with the next step.

```
Anaconda2-4.2.0-Linux-x86.sh: line 484: /home/pi/anaconda2/pkgs/python-3.5.2-0/bin/python: cannot execute binary file: Exec format error
ERROR:
cannot execute native linux-32 binary, output from 'uname -a' is:
Linux raspberrypi 4.4.21-v7+ #911 SMP Thu Sep 15 14:22:38 BST 2016 armv7l GNU/Linux

```

5. Once Anaconda has successfully installed, run: `sudo apt-get install python-pip cmake` 

**Note:** If this fails, run `sudo apt-get update` and then rerun: `sudo apt-get install python-pip cmake`

6.	Run: `sudo pip install qibuild`


4.	Install the wiringPi library on the Raspberry Pi.

1. In a new Terminal/PuTTY window, SSH into your Raspberry Pi: `ssh pi@{ip_address}`. You will be prompted for the username (**pi**) and/or password (**raspberry**) for the Raspberry Pi.

2.	Navigate to your Raspberry Pi's **home directory** by running: `cd /home/pi` 

3.	Run: `git clone git://git.drogon.net/wiringPi`

4.	Now navigate into the wiringPi directory by running: `cd wiringPi/`

5.	Run: `./build`

You should see a list of classes compiled and "All Done" at the end.

5. Finally, in a new Terminal/PuTTY window, SSH into your Raspberry Pi, navigate to its home directory by running: `cd /home/pi` and in here, create a new directory called **self** by running: `mkdir self`. This directory will be used in the steps below.

## 4. Download the Self SDK and build Self on your Raspberry Pi


### A. Download the Self SDK

1. [Download the Self SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on the **download icon** next to the default **master** branch selected.

2. Copy the zip file from your local machine across to the Raspberry Pi.

**For Mac users:**

1. In a Terminal window, navigate to the directory where you downloaded the zip file, and copy it across to the newly created **self** directory on the Raspberry Pi using: `scp self-sdk-master.zip pi@{IPaddress}:/home/pi/self/`

**For Windows users:**

1. Open Filezilla and connect to your Raspberry Pi.

1. In the **Host** field, specify your Raspberry Pi's IP address.

2. In the **Username** field, specify your Raspberry Pi's username (**pi**).

3. In the **Password** field, specify your Raspberry Pi's password (**raspberry**).

4. In the **Port** field, specify **22**. 

2. In the **Local site** side of the screen, navigate to the `self-sdk-master.zip` file.

3.	In the **Remote site** side of the screen, navigate to the directory: **/home/pi/self**

4.	Click on the file `self-sdk-master.zip` on the **Local site** side of the screen and drag it to the **Remote site** side of the screen. You can monitor the progress of the transfer in the panel located at the bottom of the Filezilla screen.

3. Unzip the `self-sdk-master.zip` file into the **self** directory of your Raspberry Pi. 

1.	Navigate to the **self** directory on your Raspberry Pi. You can open a new Terminal/PuTTY window as before, SSH into your Raspberry Pi, and run: `cd /home/pi/self`. If your prompt reads: `pi@raspberrypi:~/self $`, this confirms that you are in the **self** directory.

2.	Run the following command: `unzip self-sdk-master.zip`

4. Build Self on your Raspberry Pi with the following steps:

1.	Navigate into the **self-sdk-master** directory on your Raspberry Pi: `cd self-sdk-master`

2.	Mark the build script as executable by running: `chmod +x scripts/build_raspi.sh`

3. Run: `scripts/clean.sh` 

4. Run: `scripts/build_raspi.sh`

###You are now ready to proceed from **Section 3: Download the Self SDK onto your computer and add in the code for the move joint gesture.** 

As you have already downloaded the `wlabs_self-sdk-master.zip` file, your first step will be **Section 3.A.2:** Create a new directory named **intu** in your **home** directory
.

**Code Overview**

1. All classes in Intu inherit from the ISerializable interface. All objects can be deserialized from the Deserialize function and serialized from the Serialize function. All deserialization and serialization occurs from the body.json file found in the etc/profiles directory.

2. The flow of how this gesture gets executed has led up to working on the previous workshops. In Workshop 2, you have configured a Conversation service to have it respond to questions in a variety of ways. One thing that can be done in Conversation is to add an `[emote=show_laugh]` tag after a response. The text that gets returned from the Conversation service eventually makes its way to the SpeakingAgent. The SpeakingAgent will see the `[emote=show_laugh]` tag, and create an Emotion object and place that on the Blackboard. The agent that was developed in Workshop 3 subscribes to Emotion objects on start up. In the callback, the OnEmotion() function will execute an AnimateGesture skill.

3. All gestures inherit from the IGesture interface. Therefore, all gestures have a Start(), Stop(), Execute(), and Abort():

* **Start():** This function is called from the GestureManager class. If the function returns false, then the GestureManager will not register the gesture.

* **Stop():** This function is called when the GestureManager is stopped

* **Execute():** The main implementation on how to carry out the execution of the gesture. If we look at the top of the cpp file, you will see two macros defined: REG_SERIALIZABLE and RTTI_IMPL. REG_SERIALIZABLE will serialize the object type to our system so we can get a handle on it using reflection, while RTTI_IMPL states that our implementation of the class WorkshopFiveGesture will override our base AnimateGesture class. The power of this allows us to have platform specific code to carry out execution of gestures while still keeping the core Intu platform agnostic. Therefore, when AnimateGesture is called to execute, our WorkshopFiveAnimation execute function will be called.

* **Abort():** Will stop the execution of the gesture if the gesture is still in progress.



