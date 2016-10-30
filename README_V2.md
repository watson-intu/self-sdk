# Self C++ SDK

Self enables Watson services in devices that perceive by vision, audition, olfaction, sonar, infrared energy, temperature, and vibration. Self-enabled devices express themselves and interact with their environments, other devices, and people through speakers, actuators, gestures, scent emitters, lights, navigation, and more.

## Supported platforms

* Mac
* Windows
* Aldebaran Nao and Pepper robots

## Getting started with Self
You can configure the Watson services your Self-enabled device uses and monitor the device from remote locations.

Getting started includes the following tasks:

1. [Setting up your IBM Bluemix account](#setting-up-your-ibm-bluemix-account)
2. [Requesting access to the Watson Gateway](#requesting-access-to-the-watson-gateway)
3. [Downloading and installing Self](#downloading-and-installing-self)
4. [Configuring Self](#configuring-self)

**Before you begin**: If you're going to install Self on a robot, connect the robot's power supply, and turn on the device. Some devices take several minutes to warm up.

### Setting up your IBM Bluemix account
You need a Bluemix account to use Self. Bluemix is a development platform that gives you access to the Watson services that Self uses.

1. Log in to Bluemix. Choose one of the following options:
  * If you do not have a Bluemix account, click **[Sign up](https://console.ng.bluemix.net/registration/?Target=https%3A%2F%2Fconsole.ng.bluemix.net%2Flogin%3Fstate%3D%2Fhome%2Fonboard)**. The Sign up for IBM Bluemix page is displayed.
    1. Fill in the required fields. Ensure that your password is 8 - 31 uppercase and lowercase alphanumeric characters. Additional valid characters are the hyphen (-), underscore (_), period (.), and at sign (@). Invalid characters are the hashtag (#) and exclamation point (!). 
    2. Click **Create Account**. A confirmation email is sent to the email address of your new Bluemix account.
    3. Open the email, and click **Confirm your account**. Your account is activated, and the log-in page is displayed.
    4. Click **Log in**.
    5. Specify your IBMid or email address in the field, and click **Continue**.
    6. Specify your password, and click **Log in**. The Create Organization page is displayed.
    7. In the **Name your organization** field, specify a name for your project or team, or click one of the suggested names, and then click **Create**. The Create Space page is displayed.
    8. In the **Name your space** field, specify a name for your environment, or click one of the suggested names, and then click **Create**. The Summary page is displayed.
    9. Click **I'm Ready**. The Bluemix console is displayed.
  * If you already have a Bluemix account, click **[Log In](https://idaas.iam.ibm.com/idaas/mtfim/sps/authsvc?PolicyId=urn:ibm:security:authentication:asf:basicldapuser)**. The Log In to Bluemix page is displayed.
    1. Specify your IBMid or email address in the field, and click **Continue**.
    2. Specify your password, and click **Sign in**. The Bluemix console is displayed.
  
#### Creating instances of Watson services
To use Self, you need operational instances of the following services in Bluemix: AlchemyAPI, Conversation, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the navigation bar.
2. Click **Watson** in the categories menu.
3. Create an instance of the AlchemyAPI service.
  1. Click the **AlchemyAPI** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab. The name of your service instance is in the Key Name column.
  4. Click **View Credentials** in the Actions column.
  5. Copy the value of the `apikey` parameter, and paste it in the text file you opened.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
4. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
5. Create instances of the Speech to Text and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

### Requesting access to the Watson Self Gateway
1. Open [Self Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Self Gateway.
5. Click **Next**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Self Installer page is displayed.

### Downloading and installing Self Manager

1. From the Self Installer page, download the installation package.
  * Mac OS users, click **Download**.
  * Windows users, click the **Choose Windows** link, and click **Download**.
2. Extract the files from the package into your working directory.
3. In your working directory, run the `Self Manager.exe` file. A security warning is displayed.
4. Click **Run**. The Self Manager Configuration window is displayed.
5. Select the **Windowed** checkbox, and click **Play!**. The Self Tooling page is displayed.
6. Click **Install Self**, and log in to Bluemix. 
7. Return to the Self Manager application.
8. Depending on where you want to install Self, select **Local Workstation** or **Device**, and click **Next**.
8. Click **Device**, and click **Next**.
9. Click the name of your device, and click **Continue**. If your device is not connected to the network, complete these steps:
  1. Click **Add device**.
  2. Press the button on your device's chest to retreive its IP address, and store it.
  3. In the **Device Name** field on the Self Gateway page, specify a name. Any name will work.
  4. In the **Device IP** field, specify the device's IP address.
  5. Click **Continue**.
  6. In the **User Name** field, specify the name of your device.
  7. In the **Password** field, specify the password for your device.
  8. Click **Connect**.

Installing Self Manager takes a few minutes. After the installation process is complete, the Remote Monitor opens in a new window.

### Configuring Self Manager
Your installation is preconfigured to use the AlchemyAPI, Conversation, Speech to Text, and Text to Speech services. To configure Self to use your instances of these services, complete the following steps:

1. Expand **All Organizations** by clicking the arrow icon.
2. Click the name of your organization.
3. Expand your organization by clicking the arrow icon.
4. Click the name of your group.
5. Click **Services** in the navigation bar.
6. For your instances of the Conversation service, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.
7. For your instance of the AlchemyAPI service, click **Edit**, specify the API key, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.

## NEW SECTION
In this section, you import a Conversation workspace and then what? What are the next steps?

### Importing a workspace

1. Return to the Gateway in your browser, and click **Downloads**.
2. Click **GET NAME OF FILE** to download the Conversation workspace.
3. In Bluemix, open your instance of the Conversation service.
4. On the Manage tab, click **Launch tool**.
5. Click **Log In**.
6. Click **Import**.
7. Choose the file you downloaded, and click **Import**.

## Feedback

Post your comments and questions and include the `self` tag on 
[dW Answers](https://developer.ibm.com/answers/questions/ask/?topics=watson)
or [Stack Overflow](http://stackoverflow.com/questions/ask?tags=ibm-watson).