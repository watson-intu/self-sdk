## Compiling Intu for various platforms

### Getting the files 

You may want to use git to clone the repository, so that the self-docs submodule is downloaded for easy referal. In your home directory, do the following:

**Option 1:** [Download the Self SDK](https://github.com/watson-intu/self-sdk) using git commands. Issue following commands on your terminal in Mac:

1. `git clone https://github.com/watson-intu/self-sdk.git`
2. `cd self-sdk`
3. `git submodule update --init --recursive`

**Important:** If you use SourceTree, the process might get stuck when trying to pull by using SSH. This is because SourceTree will try to launch an interactive window that will be hidden. Run the following commands on the command line to fix the problem:

1. `cd "C:\Program Files (x86)\Atlassian\SourceTree\tools\putty"`
	
2. `plink git@github.ibm.com`


**Option 2:** [Download the Self SDK](https://github.com/watson-intu/self-sdk) and seperately download [Download the Self Docs](https://github.com/watson-intu/self-docs). Unzip it in a place you prefer.

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
	
	4. Run: `./run_self.sh`
    

### Aldebaran Nao and Pepper robots using OS X

1. Set up [CMake](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#required-buidsys).
2. Set up [qiBuild](http://doc.aldebaran.com/2-1/dev/cpp/install_guide.html#qibuild-install).
3. Run the following commmands:
  * `./scripts/build_nao.sh [profile]`
4. Run the following command to install into on the given robot using scp:
  * `./scripts/install_nao.sh [user@host]`
