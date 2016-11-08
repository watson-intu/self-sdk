# Workshop 1: Saying Hello!

In this workshop, you install Intu on your laptop and get it to say hello to you.

**Before you begin:** 

* You must have a Mac or Windows laptop.
* Turn on your laptop's microphone.
* You will notice that Intu and Self are used interchangeably. Self is the technical name for Intu.

Complete the following tasks:

1. [Requesting access to the Intu Gateway](#requesting-access-to-the-watson-intu-gateway)
2. [Downloading Intu](#downloading-intu)
3. [Installing Intu](#installing-intu)
4. [Saying hello to Intu](#saying-hello-to-intu)

## 1. Requesting access to the Intu Gateway

1. Request access to the [Intu Gateway](https://rg-gateway.mybluemix.net/).

2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Intu Gateway.
5. Click **Submit**. After your request for access is approved, you will receive a confirmation email.
6. Open the confirmation email, and **click the link**. The Intu Gateway Log In page is displayed again.
7. Click **Log In**. The Intu Downloads page is displayed.

## 2. Downloading Intu

1. On the Intu Downloads page, click on the the topmost download arrow for **Download Intu Tooling**. **Intu-Tooling-OSX64.zip** or **Intu-Tooling-Win64.zip** will start to download, depending on your machine.

2. Extract the Intu-Tooling-OSX64 or Intu-Tooling-Win64 directory (it may do so automatically), and copy the entire directory into your **home** directory. This is:


	**For Windows users:** C:\Users\username ("username" should read your name)
	
	**For Mac users:**
	 /Users/username ("username" should read your name) 


## 3. Installing Intu

1. Open the Intu-Tooling-OSX64 or Intu-Tooling-Win64 directory, where you will find the **Intu Manager**. Open Intu Manager.
	
	**For Mac users:** Right-click on the Intu Manager and select **Open**
	
	**For Windows user**, double-click on the Intu Manager to open it. 
	
	If a security warning is displayed, accept the risk and open the file.
2. Select the **Windowed** checkbox, accept the other default values, and click **Play!**. If a security warning is displayed, accept the risk. The Intu Manager page is now displayed. 
3. Click **Install Intu**. Intu Tooling will start to install, and a new page will open in your browser for the Intu Tooling Log In page.
4. Click **Log In**. Wait until you see the prompt to return to the Intu Manager application. At that point, return back to the Intu Tooling application.
5. A page displays options for where you can choose to install Intu. For this workshop, select **Local Machine**, and click **Next**. A page displays your Organizations and Groups in the dropdown menus. Default values are already selected. If you have multiple Organizations, Groups or both and want to use them, manually select them from the menus.
6. Click **Install**. Installing Intu takes a few minutes. During the installation process, if you see one or more security prompts, make sure to allow access.

7. After Intu installation is **Done!** and **100%** installed, if your microphone is on, say "hi,” or “how are you?", and hear Intu return a greeting. You will see a message "Intu install complete, loading Monitor in..." You can wait for the Intu Monitor to load automatically or click **Intu Monitor**.

8.	The Intu Manager window is displayed, and you're prompted to select your Group. The Organization and Group should be preselected in the dropdown menu. 

3. Click **Next**. A "Connecting to parent..." message is displayed while your Intu Manager tries to establish a connection. When the connection is made, the status icon of the device you installed Intu on turns **green**. 
4. Click on your device. A representation of a brain is displayed, and you will also see a **Menu** on the bottom left of the window.

Now that Intu is installed successfully, explore how you can test conversations in speech and text with Intu in the next task.

## 4. Saying hello to Intu

1. In the Intu Manager, click **Menu**, and then click **Conversation** so that you can see what happens when you talk to Intu. Ensure that the status icon of the device you have installed Intu on is green. If it is red, talk to a moderator of the workshop.

2. Test Intu through speech and text:
 * Say "hello" into your microphone. Intu speaks a greeting.
 * In the empty field on the right side of the window, type "hello," and click **Ask**. Intu returns a text greeting in the window.

## Challenge: Configuring the Intu avatar for your instance

To complete this challenge, do the tasks in Workshop 7: Installing, configuring, and running the Intu Avatar.

  
## After DevCon ends
Your instance of Intu is preconfigured with the following Watson services: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech. The preconfiguration is enabled for 30 days. If you want to test Intu after 30 days, you must create your own instances of these services and configure Intu to use them.

### 1. Creating instances of Watson services
To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the top right navigation bar.

2. In the All Categories menu on the left, under Services, click on **Watson**.

3. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new Conversation service instance.
  5. Copy the values for your password and username and paste them into a new text file in your favorite text editor.
  6. Click the **<--Watson** breadcrumb near the top left (directly above your Conversation service name). The list of your service instances is displayed.
  7. Add the next service instance by clicking the **Create Watson** **+** button. The Watson service catalog is displayed.

4. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same substeps 1 - 7 that you completed to create the Conversation service instance.


### 2. Configuring Intu to use your service instances

To configure Intu to use your instances of these Watson services, log in to the [Intu Gateway](https://rg-gateway.mybluemix.net/) and complete the following steps:

1. Click on **MANAGE** on the left hand side navigation bar, and select **Services**. 

2. Select your Organization and Group in the top Filter by menu, if not already selected.

3. For your instances of the Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, and specify the user ID and password (saved in your text file in the previous section **Creating instances of Watson services**), and click **Save**.

4. To configure your instance of **Conversation**, navigate to **DOWNLOADS** on the left of your Intu Gateway browser page, download the **Intu Starter Kit**, and follow the instructions in the `readme.txt` file. Alternatively, go to the instructions for **Workshop 2**, and follow the steps in: **1. Setting up the Conversation service**.
 

**Important:** Do not change the service endpoint for your services unless you are an enterprise user.
