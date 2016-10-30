# Customizing the conversation 

In this workshop, you customize the greeting in the dialog from the Conversation service.

**Before you begin:** You must have a Mac or Windows laptop.

Complete the following tasks:

1. [Getting started](#getting-started)
2. [Customizing the greeting](#customizing-the-greeting)

## Getting started

If you completed the "Say Hello" workshop, go to [Creating your own instance of the Conversation service](#creating-your-own-instance-of-the-conversation-service). Otherwise, complete the following tasks.

Getting started includes the following tasks

1. [Requesting access to the Watson Self Gateway](#requesting-access-to-the-watson-gateway)
2. [Downloading and installing Self](#downloading-and-installing-self)
3. [Setting up the Conversation service](#setting-up-the-conversation-service)

### Requesting access to the Watson Self Gateway

1. Request access to the Watson Self Gateway. Open [Self Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Self Gateway.
5. Click **Next**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Self Installer page is displayed.

### Downloading and installing Self Manager

1. From the Self Installer page, download the installation package.
2. Extract the files from the package into your working directory.
3. In your working directory, run the `Self Manager.exe` file. A security warning is displayed.
4. Click **Run**. The Self Manager Configuration window is displayed.
5. Select the **Windowed** checkbox, and click **Play!**. The Self Tooling page is displayed.
6. Click **Install Self**, and log in to Bluemix. 
7. Return to the Self Manager application.

Installing Self Manager takes a few minutes. After the installation process is complete, your instance of Self is preconfigured with the following Watson services: AlchemyAPI, Conversation, Speech to Text, and Text to Speech. The preconfiguration is enabled for a trial period of 24 hours. If you want to test Self after the trial period ends, see [After DevCon ends](#after-devcon-ends).

### Setting up the Conversation service

You must create your own instance of the Conversation service, configure Self to use the service credentials, and import a workspace for the service.

#### Creating your own instance of the Conversation service

To complete this workshop, you must create your own instance of the Conversation service and use its service credentials.

1. [Log in to Bluemix](https://idaas.iam.ibm.com/idaas/mtfim/sps/authsvc?PolicyId=urn:ibm:security:authentication:asf:basicldapuser).
2. On the Bluemix dashboard, click **Catalog** in the navigation bar.
3. Click **Watson** in the categories menu.
4. Click the **Conversation** tile.
  1. Keep all the default values, and click **Create**.
  2. Click the **Service Credentials** tab.
  3. Click **View Credentials** for the new service instance.
  4. Copy the values of the `password` and `username` parameters and paste them in your text file.
5. Configure Self to use your Conversation service instance.
  1. Return to the Watson Self Gateway. Expand **All Organizations** by clicking the arrow icon.
  2. Click the name of your organization.
  3. Expand your organization by clicking the arrow icon.
  4. Click the name of your group.
  5. Click **Services** in the navigation bar.
  6. For your instance of the Conversation service, click **Edit**, paste the user ID and password values, and click **Save**.
  7. Do not close the Conversation service in your browser. You will return to it later.

**Important:** Do not change the service endpoint unless you are an enterprise user.

#### Importing a workspace into the Conversation service

To complete this task, you must download a Conversation service workspace that we made for you and import it into the Conversation service tool.

1. Return to the Watson Self Gateway, and click **Downloads**.
2. Download the sample conversation starter file.
3. Return to the Conversation service in your browser, and select **Manage**.
4. Click **Launch tool**. The **Create workspace** page is displayed.
5. Click **Import**.
6. Click **Choose a file**, navigate to the sample conversation starter file that you downloaded in step 2, select it, and click **Import**. Keep the default settings so that you import everything. The workspace is imported and is displayed on the Workspaces page.

## Customizing the greeting

**Before you begin:** Turn on the microphone on your device.

1. On the Workspaces page, click **Get started**.
2. Click **Dialog** in the navigation bar.
3. Expand this branch of the dialog tree: **#dialog** -> **@greeting** -> **@greeting:hello**.
4. Change the **fill in the blank** message to **fill in the blank**.
5. Click the conversation bubble icon to open a window where you can test your new message.
6. Test your change. 
  1. In the Conversation tool, click the conversation bubble icon to open a window where you can test your new message. In the **Enter something to test your bot** field, type Hello, and press **Enter**. Your new greeting is returned.
  2. Say hello. Your new greeting is returned as speech.
  
## After DevCon ends

If you want to test Self after the trial period ends, you must create your own instances of these services and configure Self to use them.

### Creating instances of Watson services
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
4. Create an instance of the Speech to Text service.
  1. Click the **Speech to Text** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
5. Create an instance of Text to Speech service by repeating the same steps 1 - 7 that you completed to create the Conversation service and Speech to Text service instances.

### Configuring Self to use your service instances
Your installation is preconfigured to use the AlchemyAPI, Conversation, Speech to Text, and Text to Speech services. To configure Self to use your instances of these services, complete the following steps:

1. Expand **All Organizations** by clicking the arrow icon.
2. Click the name of your organization.
3. Expand your organization by clicking the arrow icon.
4. Click the name of your group.
5. Click **Services** in the navigation bar.
6. For your instances of the Conversation service, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.
7. For your instance of the AlchemyAPI service, click **Edit**, specify the API key, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
