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
1.	**On your local machine** clone or download the self-sdk (make sure you are on the self-sdk develop branch) from [Github](https://github.ibm.com/watson-labs-austin/self-sdk). If you downloaded this file, unzip it.

2.	Navigate into:
	`self-sdk-develop/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Start/`
	
	In this directory you should find these two files:
	
	`WorkshopFiveGesture.cpp` 
	`WorkshopFiveGesture.h`
	
	Copy these files (using `command + C` for Mac or `Ctrl + C` for Windows). 
	
3.	In your computer’s home directory create a new folder called `workshop_five`.
   
   Then within the `workshop_five` folder, create a folder called `gesture`. 
Within the `gesture` directory paste the `WorkshopFiveGesture.cpp` and `WorkshopFiveGesture.h` files you copied, here.

4. Now navigate into `self-sdk-develop/docs/workshops-devcon/5/code-snippets/WorkshopFiveGesture_Snippets/` and locate the `WorkshopFiveCodeSnippets.txt` file.
   
   Copy all the contents in `WorkshopFiveCodeSnippets.txt`.

   Open the `self-sdk-develop/examples/workshop_five/gestures/WorkshopFiveGesture.cpp` source file and locate the method `DoAnimateThread()`. Into the body of this function paste the code from the `WorkshopFiveCodeSnippets.txt`. 

5.	Navigate to `self-sdk-develop/examples/workshop_five` and create a text file called `CMakeLists.txt` and open this file. 

    In this file add in the following text:

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
	
	
 And save the file. 

## Updating Raspberry Pi with LED gesture.
6.	Now move your `self-sdk-develop/examples/workshop_five` directory from your local machine to your Pi. 
   * For Mac, use Terminal. Run `cd self-sdk-develop/examples` and then:
 `scp -r workshop_five pi@[IPaddress]:~/self/self-sdk-develop/examples`

   * For PC, use Filezilla.
	Navigate to `self/self-sdk-develop/examples/` on the remote site side of the screen.
	Navigate to the `self/self-sdk-develop/examples/` directory that you just created on the local site side of the screen.
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
		 `scp pi@[pi ip address]:/home/pi/self/self-sdk-develop/bin/raspi/etc/profile/ ~/`

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

