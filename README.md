# Intu
 
Intu is an architecture that enables Watson services in devices that perceive by vision, audition, olfaction, sonar, infrared energy, temperature, and vibration. Intu-enabled devices express themselves and interact with their environments, other devices, and people through speakers, actuators, gestures, scent emitters, lights, navigation, and more.

## Before you begin

  * To use Intu, you must have a Bluemix account. To register for a Bluemix account, go to https://console.ng.bluemix.net/registration/. 
  * Ensure your hardware meets the following requirements:
  
    * Windows
    
      * Intel® Core 2 or AMD Athlon® 64 processor; 2 GHz or faster processor
      * Microsoft Windows 7 with Service Pack 1, Windows 8.1, or Windows 10, Windows 2013
      * 2 GB of RAM (8 GB recommended)
      * 1 GB of available hard-disk space for 32-bit installation; 1 GB of available hard-disk space for 64-bit installation; additional free space required during installation (cannot install on a volume that uses a case-sensitive file system)
      * 1024 x 768 display (1280x800 recommended) with 16-bit color and 512 MB of dedicated VRAM; 2 GB is recommended
      * OpenGL 2.0–capable system
      * CPU: SSE2 instruction set support
      * Graphics card: DX9 (shader model 3.0) or DX11 with feature level 9.3 capabilities.
      * Internet connection and registration are necessary for required software activation, validation of subscriptions, and access to online services.

    * Mac OS
    
      * Multicore Intel processor with 64-bit support
      * Mac OS 10.12, 10.11, 10.10, 10.9
      * 2 GB of RAM (8 GB recommended)
      * 1GB of available hard-disk space for installation; additional free space required during installation (cannot install on a volume that uses a case-sensitive file system)
      * 1024 x 768 display (1280x800 recommended) with 16-bit color and 512 MB of dedicated VRAM; 2 GB is recommended
      * OpenGL 2.0–capable system (This is must have for Unity Application)
      * CPU: SSE2 instruction set support
      * Graphics card: DX9 (shader model 3.0) or DX11 with feature level 9.3 capabilities.
      * Internet connection and registration are necessary for required software activation, membership validation, and access to online services.

## Getting started

Getting started includes the following tasks:

1. [Requesting access to the Watson Intu Gateway](#requesting-access-to-the-watson-intu-gateway)
2. [Downloading Intu](#downloading-intu)
3. [Installing Intu](#installing-intu)

### Requesting access to the Watson Intu Gateway

1. Request access to the Watson Intu Gateway. Open [Intu Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Intu Gateway.
5. Click **Submit**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Intu Gateway Log In page is displayed again.
7. Click **Log In**. The Intu Downloads page is displayed.

### Downloading Intu

1. On the Intu Downloads page, download the appropriate installation package for your platform.
2. Extract the files from the package into your working directory.

### Installing Intu

1. In your working directory, double-click Intu Manager. If a security warning is displayed, accept the risk and open the file.
2. Select the **Windowed** checkbox, and click **Play!**. The Intu Tooling page is displayed.
3. Click **Install Intu**, and a new Intu Tooling sign-in page is displayed.  
4. Click **Log In**. You are prompted to return to the Intu Manager application.
5. A page displays options for where you can choose to install Intu. For this workshop, select local machine, and click **Next**. A page displays your organization in the dropdown menu, and defaultGroup is selected.
6. Click **Install**. Installing Intu takes a few minutes. During the installation process, if you see one or more security prompts, allow access. After the installation process is complete, your instance of Intu is preconfigured with the following Watson services: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech. The preconfiguration is enabled for a trial period of 24 hours. If you want to test Intu after the trial period, see [After DevCon ends](#after-devcon-ends).

After Intu is installed, the Intu Manager window is displayed, and you're prompted to select your group. Your organization and group should be preselected in the dropdown menu.

3. Click **Next**. A "Connecting to parent..." message is displayed while your Intu embodiment tries to establish a connection. During this part of the process, the box beside your embodiment is red and labeled with Off. After the connection is established, the box is green and labeled On.
4. Doubleclick your embodiment. The Menu option is displayed.

## Configuring Intu
Your installation is preconfigured to use the Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services. To configure Intu to use your instances of these services, complete the following steps.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. [Log in to Bluemix](http://www.ibm.com/cloud-computing/bluemix/).
2. On the Bluemix dashboard, click **Catalog** in the navigation bar.
3. Click **Watson** in the categories menu.
4. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb.
  7. Add the next service instance by clicking the hexagonal **+** button.
5. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.
6. Specify your service credentials in Intu Gateway.
  1. Expand **All Organizations** by clicking the arrow icon.
  2. Click the name of your organization.
  3. Expand your organization by clicking the arrow icon.
  4. Click the name of your group.
  5. Click **Services** in the navigation bar.
  6. For your instances of the Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.

### Installing the Intu Starter Kit

The Intu Starter Kit contains a Conversation service workspace that helps you visualize how intents, entities, and dialog are developed. You can expand on the workspace in the kit or use it as a guide for developing your own later.

1. Log in to Intu Gateway.
2. Click **Downloads**.
3. Download the Intu Starter Kit.
4. Complete the instructions in `readme.txt`.

## Compiling Intu for various platforms

### Getting the files

Download the code to your computer. You can do download the code in either of the following ways:

  * Download the .zip file of this repository to a local directory.
  * Clone this repository locally.

### Release Notes
* You may need to delete the "wdc" directory to get latest due to that directory being converted into a sub-module.
* If you use git bash to checkout, run the following command to checkout the submodule `git submodule foreach git pull origin develop`.

#### For Intu version 0.1.269 - Update on 12/1/2016
1. Fixed an issue where orgs with apostrophes in the Org Name could not connect to parents.
2. Fixed an issue where previously users could not connect to parents because of scalability problems.
3. Fixed an issue where Intu Gateway users were frequently logged out and redirected to the login page.
4. Fixed an issue where multi-level conversation did not retain context.
5. Included required executable vc_resti.x86.exe for the Windows build.

### Windows

1. Set up [Visual Studio 2015](https://www.visualstudio.com/downloads/).
2. Open `/vs2015/self.sln` in this project.
3. Select self-sdk as your startup project, compile, and run.

### OS X

1. Set up [CMake](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#required-buidsys). To install CMake by using Homebrew, run `brew install cmake`.
  * To install Homebrew, run the following command in your terminal: ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
2. Set up [qiBuild](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#qibuild-install).
  * `pip install qibuild` (NOTE: it is highly recommended to download [anaconda python version 2.7](https://www.continuum.io/downloads) to have pip correctly configured)
  * `qibuild config --wizard` (use default setup for steps by pressing 1 twice)
3. Run the following commands:
  * `cd {self root directory}`
  * `./scripts/build_mac.sh [profile]`
  
This process stages the executables in the `bin/mac` directory on your local computer. You can change into that directory and run the unit_test and self_instance executables.

PS: If you run into issues with the build, you might have to change a couple of Boost header files, as described here: https://github.com/Homebrew/legacy-homebrew/issues/27396 (specifically, you might have to replace your copy of Boost's boost/atomic/detail/cas128strong.hpp and boost/atomic/detail/gcc-atomic.hpp with the latest available in the Boost directory)

### Linux

1. Set up qibuild and CMake. You can use your Linux package manager to install CMake, and any distribution of Python (2.7 recommended) to install qibuild through pip.
2. Run the following commands:
  * `./scripts/build_linux.sh [profile]`

### Raspberry Pi

**Note:** If any step below fails or errors, run: `sudo apt-get update`, then repeat the step.

1.	Install Raspbian Operating System onto your Raspbery Pi

2.	Open up a new browser window on your laptop and download [**Anaconda 4.2.0 For Linux Python 2.7 version**](https://www.continuum.io/downloads).

	**Make sure you download the correct version.** You need the LINUX version no matter what operating system you have. Windows users may have to right click and select **Save as** to save the download locally.

3.	Copy Anaconda from your laptop over to the Pi. **Windows** users may need to [download](https://filezilla-project.org/) and use something like **Filezilla** to copy files over to the Pi.
	1. Navigate to the directory where you downloaded Anaconda locally.
	2. Copy Anaconda from your laptop over to the Pi using the following command: `scp Anaconda2-4.2.0-Linux-x86.sh pi@{ip}:/home/pi` 
		(e.g. `scp Anaconda2-4.2.0-Linux-x86.sh pi@10.0.1.2:/home/pi`)
		If prompted, the username is **pi** and password is **raspberry**

4.	Install Anaconda on your Pi and set up qiBuild.
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
 
5.	Install the wiringPi library on the Pi.
	1. In a new Terminal/PuTTY window, ssh into your Pi: `ssh pi@{ip_address}`. 
	2.	Navigate to your Pi's **home directory** by running: `cd /home/pi`. 
	3.	Run: `git clone git://git.drogon.net/wiringPi`
	4.	Now navigate into the wiringPi directory by running: `cd wiringPi/`
	5.	Run: `./build`

	You should see a list of classes compiled and “All Done” at the end.

6.	Build Intu on the Pi
	1. In your current (or a new) ssh session to the Raspberry Pi, navigate to the self directory: `cd {self root directory}`
	2. Mark the build script as executable by running: `chmod +x scripts/build_raspi.sh`
	3. Now build the Self SDK by running: `scripts/build_raspi.sh`

	**Note:** If you have any build errors, run: `scripts/clean.sh` and then rerun: `scripts/build_raspi.sh`

7. 	Run Intu on the Pi
	1. If you have a HDMI cable plugged into your Raspberry Pi, verify that the sound is set to analog. This can be done by right clicking the speaker icon at the top right hand corner of the Raspberry Pi's homescreen, and selecting analog.

	2. Verify that you have a microphone and speaker plugged into your Raspberry Pi. Note that your speaker may need to be charged before use. Make sure that it is turned on before proceeding with the next step.

	3. Navigate to the raspi directory using: `cd {self root directory}/bin/raspi`.
	
	4. Run: `export LD_LIBRARY_PATH=./`
	
	5. Run: `export WIRINGPI_GPIOMEM=1`
	
	6. Run: `./self_instance`
    
This process installs Intu on the remote device whose user name and IP address you provide. You can go to the `~/self/latest` directory on that device and run `run_self.sh`. This process was tested on Red Hat Enterprise 6.6 and 6.7.

### Aldebaran Nao and Pepper robots using OS X

1. Set up [CMake](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#required-buidsys).
2. Set up [qiBuild](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#qibuild-install).
3. Run the following commmands:
  * `./scripts/build_nao.sh [profile]`
4. Run the following command to install into on the given robot using scp:
  * `./scripts/install_nao.sh [user@host]`
  
## Feedback

Post your comments and questions and include the `project-intu` and `intu` tags on 
[dW Answers](https://developer.ibm.com/answers/questions/ask/?topics=watson)
or [Stack Overflow](http://stackoverflow.com/questions/ask?tags=project-intu).
