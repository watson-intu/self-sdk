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

**Note: you should have assembled the Raspberry Pi from the instructions file**

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

At this point, you can disconnect the external monitor, mouse and keyboard from the Pi. Go back to your laptop (make sure it is on the same Wifi network). Now make sure you reboot the Pi: sudo reboot by connecting to your Pi via ssh (or putty ssh) and running the command: `sudo reboot`

**Note: Unless explicitly stated, all the following steps are to be run on your local computer.**

## Download self-sdk onto your computer and add in the LED gesture code.
1.	**On your local machine** clone or download the self-sdk (make sure you are on the self-sdk master branch) from [Github](https://hub.jazz.net/project/wlabs/self-sdk). If you downloaded this file, unzip it.

2.	Navigate into:
	`wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Start/`
	
	In this directory you should find these two files:
	
	`WorkshopFiveGesture.cpp` 
	`WorkshopFiveGesture.h`
	
	Copy these files (using `command + C` for Mac or `Ctrl + C` for Windows). 
	
3.	In your computer’s home directory create a new folder called `workshop_five`.
   
   Then within the `workshop_five` folder, create a folder called `gesture`. 
Within the `gesture` directory paste the `WorkshopFiveGesture.cpp` and `WorkshopFiveGesture.h` files you copied, here.

4. Now navigate into `wlabs_self-sdk-master/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Snippets/` and locate the `WorkshopFiveCodeSnippets.txt` file.
   
   Copy all the contents in `WorkshopFiveCodeSnippets.txt`.

   Open the `wlabs_self-sdk-master/examples/workshop_five/gestures/WorkshopFiveGesture.cpp` source file and locate the method `DoAnimateThread()`. Into the body of this function paste the code from the `WorkshopFiveCodeSnippets.txt`. 
   (This pasted into the brackets under `void WorkshopFiveGesture::DoAnimateThread(WorkshopFiveGesture::Request * a_pReq)` ).

5.	Navigate to `self-sdk-develop/examples/workshop_five` and create a text file called `CMakeLists.txt` and open this file. 

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

   * For PC, use Filezilla.
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

9. Retrieve the body.json from your Pi to your local machine.
		 `scp pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/body.json ~/`

	And open this `body.json` file. 

10. Note: If you have updated the body.json with `EmbodimentCreds` in Workshop 1, please skip and proceed to Step 3. 

	On your local machine, open a browser and sign into [Intu Gateway](rg-`gateway.mybluemix.net`).
 		
 Select `View Credentials` from the link on the left hand side of the page
 		* On the View Credentials page, use the filters to select your organization and group. Click the Get Credentials button.
Click the Copy link to save the entire contents to your clipboard.

	Edit the `body.json` that you opened for editing by adding the credentials you just retrieved from the gateway to it. The following steps describe how to do this.
In the `body.json`, search on `"m_EmbodimentCreds"` to jump to the correct section.
Then highlight/select everything from `"m_EmbodimentCreds"` up to and including `"m_OrgId": ""},`
Note: Don’t forget to select the comma.
Click your delete key on your keyboard
Then paste the credentials that you copied to your clipboard into the section just deleted. 

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
			* ii.	Click the Yes button. This action saves your changes to your pi.

10.	Build Self on your Pi with the following steps:

	a.	In your session ssh’ed into the Pi, navigate into the self-sdk-develop directory: `cd self-sdk-develop`
	
	b.	Mark the build script as executable by running: 
`chmod +x scripts/build_raspi.sh`

	c.	Now run: (*NOTE: If you have done this before run scripts/clean.sh) 
`scripts/build_raspi.sh`

## Run Self on your Raspberry Pi
Run Self on your Pi by completing the following steps in your ssh window (Terminal for Mac or Putty for Windows). 

Note: The following steps will need to be repeated each time you power your Pi back up (i.e. unplug power and plug it back in).

1.	First, verify that you do NOT have HDMI plugged into your pi
2.	Verify that you have a microphone and speaker plugged into your pi. Your speaker may need to be charged first. It will need to be plugged in and on before proceeding to the next step.
4.	Navigate to the raspi directory using: `cd /home/pi/self/self-sdk-develop/bin/raspi`.
5.	Run: `export LD_LIBRARY_PATH=./`.
6.	Run: `export WIRINGPI_GPIOMEM=1`.
7.	Run: `./self_instance `.

You have now added a gesture for the LED light.  When you say, "can you laugh" to the robot, the LED light should blink i.e. you have added the blinking of the LED to Intu as a gesture. 

# APPENDIX

##Steps for putting Intu on your Raspberry Pi using an SD card with the Intu image

**Note:** In this workshop, commands are issued from **Terminal** on **Mac** or **PuTTY** on **Windows**. For **Windows** users, if you do not have **PuTTY** installed already, you can download it using this [link](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).

1.	Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Pi.

2.	You should see a window open on your monitor. Click on the **Wifi networks (w)** icon at the top of the window, select your network (at DevCon, it will be **ROBOT_PED1**), and enter your password (**panda$123**).

3.	Get the IP address of your Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in **ifconfig** and hit Enter.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your pi (e.g. 10.0.1.2). 

4.	Ensure that your laptop is on the **same** network as your Pi from this point onwards.
	
	For **Mac** users, open a new Terminal window, and connect to the Pi using: `ssh pi@{ip_address}` (e.g. ssh pi@10.0.1.2)

	You will be prompted for a password. The default password for the Pi is **raspberry**.

	For **Windows** users, open **PuTTY**, type in the Pi's IP address, and when prompted, type in the username (**pi**) and password (**raspberry**) for the Pi.

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Pi. 

6. Check that your laptop is on the same network as the Pi. Reboot the Pi by running `sudo reboot` in your Terminal window or PuTTY. 

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

8. Connect your Raspberry Pi to a power source, and connect an external keyboard, mouse and monitor to your Pi.

9.	You should see a window open on your monitor. Click on the **Wifi networks (w)** icon at the top of the window, select your network (at DevCon, it will be **ROBOT_PED1**), and enter your password (**panda$123**).

10. As this is the first time your Raspberry Pi and SD card have been used, you'll have to select an operating system and let it install. Select **Raspbian [RECOMMENDED]**.

11. At the bottom of your screen, you will see a **Language** panel for your keyboard. Select **English (US)**. 

12.	Click on the **Install** icon on the top left of the window. A **Confirm** window will appear; select **Yes**. The installation may take up to 30-40 mins. 

	**Note:** If your installation freezes at any point (as indicated by a frozen progress bar), **unplug** the Pi from its power source and **plug it back in**, and repeat from step 8.

13.	Check that you are connected to the network. On the right of the Bluetooth icon in the top right corner of the screen, you will see a **successful connection** to your wifi network as shown by a **blue wifi icon**, or an **icon with two red crosses** if you have been **disconnected**. To reconnect, click on the two red crosses, select your network, and type in the password when prompted.

14. Get the IP address of your Pi.
	1.	Click on the black **Terminal** icon on the top left toolbar.
	2.	Type in **ifconfig** and hit Enter. **Note:** If this command does not work, **unplug** the Pi from its power source and **plug it back in**, and repeat this step.
	3. Look for the **wlan0** section. The **inet addr** gives you the IP address of your pi (e.g. 10.0.1.2). 

4.	Ensure that your laptop is on the **same** network as your Pi from this point onwards.
	
	For **Mac** users, open a new Terminal window, and connect to the Pi using: `ssh pi@{ip_address}` (e.g. ssh pi@10.0.1.2)

	You will be prompted for a password. The default password for the Pi is **raspberry**.

	For **Windows** users, open **PuTTY**, type in the Pi's IP address, and when prompted, type in the username (**pi**) and password (**raspberry**) for the Pi.

5. At this point, you can disconnect the external monitor, mouse and keyboard from the Pi. 

6. Check that your laptop is on the same network as the Pi. Reboot the Pi by running `sudo reboot` in your Terminal window or PuTTY. 

## Set up your Raspberry Pi for Builds

**Note:** If any step below fails or errors, run: `sudo apt-get update`, then repeat the step.

1.	Open up a new browser window on your laptop and download [**Anaconda 4.2.0 For Linux Python 2.7 version**](https://www.continuum.io/downloads).

	**Make sure you download the correct version.** You need the LINUX version no matter what operating system you have. Windows users may have to right click and select **Save as** to save the download locally.

2.	Copy Anaconda from your laptop over to the Pi. **Windows** users may need to [download](https://filezilla-project.org/) and use something like **Filezilla** to copy files over to the Pi.
	1. Navigate to the directory where you downloaded Anaconda locally. 
	
		
		
	2. Copy Anaconda from your laptop over to the Pi using the following command: `scp Anaconda2-4.2.0-Linux-x86.sh pi@{ip}:/home/pi` 
	
		(e.g. scp Anaconda2-4.2.0-Linux-x86.sh pi@10.0.1.2:/home/pi)
		
		If prompted, the username is **pi** and password is **raspberry**
		

3.	Install Anaconda on your Pi and set up qiBuild.
	1. In a new Terminal/PuTTY window, ssh into your Pi: `ssh pi@{ip_address}`. When prompted, the username is **pi** and password is **raspberry**.

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
 

4.	Install the wiringPi library on the Pi.
	1. In a new Terminal/PuTTY window, ssh into your Pi: `ssh pi@{ip_address}`. 
	2.	Navigate to your Pi's **home directory** by running: `cd /home/pi`. 
	3.	Run: `git clone git://git.drogon.net/wiringPi`
	4.	Now navigate into the wiringPi directory by running: `cd wiringPi/`
	5.	Run: `./build`

	You should see a list of classes compiled and "All Done" at the end.
	
Finally, in a new Terminal/PuTTY window, ssh into your Pi, navigate to its home directory by running `cd /home/pi`, and in here, create a new directory called **self**: `mkdir self`. This directory will be used in the steps below.

##Retrieve the latest self-sdk and build Self on your Raspberry Pi

1. [Download the Self SDK](https://github.ibm.com/watson-labs-austin/self-sdk). Click on **Clone or download** and select **Download Zip**.

2. Copy the zip file from your local machine across to the Pi.

	**For Mac users:**

	1. In a Terminal window, navigate to the directory where you downloaded the zip file, and copy it across to the newly created **self** directory on the Pi using: `scp self-sdk-develop.zip pi@{ip}:/home/pi/self/`
	
	**For Windows users:**

	1.	Open Filezilla and connect to your Pi.
	2. In the **Local Site**** side of the screen, navigate to the `self-sdk-develop.zip` file.
	3.	In the **Remote Site** side of the screen, navigate to the directory: **/home/pi/self**
	4.	Click on the file `self-sdk-develop.zip` on the **Local Site** side of the screen and drag it to the **Remote site** side of the screen. 
	5.	You can monitor the progress of the transfer in the panel located at the bottom of the Filezilla screen.


4. Unzip the `self-sdk-develop.zip` file into the **self** directory of your Pi. 
 	1.	Navigate to the **self** directory on your Pi. You can open a new Terminal/PuTTY window as before, ssh into your Pi, and run: `cd /home/pi/self`. If your prompt reads: `pi@raspberrypi:~/self $`, this confirms that you are in the **self** directory.
	2.	Run the following command: `unzip self-sdk-develop.zip`

5. Build Self on your Pi with the following steps:

	1.	Navigate into the **self-sdk-develop** directory on your Pi: `cd self-sdk-develop`
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

4. Copy the edited `body.json` file across to the Pi.

	**For Mac users:**

	1. After saving your changes to the `body.json` file, open a new Terminal window and run the following command to copy it across to the Pi: `scp ~/body.json pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/`

	**For Windows users:**
	
	1.	Exit the window you were using to edit the `body.json` file. If you are using Filezilla, you will be prompted to upload the file back on the server. 
	2.	Click the **Yes** button. This action saves your changes to your Pi.

