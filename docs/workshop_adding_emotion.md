# Adding emotion to Self

In this workshop, you add emotion to a response in the dialog that Self uses from workspace you imported into your instance of the Conversation service. Emotion is added to the response through the Text to Speech service.

**Before you begin:** You must have a Mac or Windows laptop.

Complete the following tasks:

1. [Getting started](#getting-started)
2. [Adding emotion to your greeting](#adding-emotion-to-your-greeting)

## Getting started

If you completed the "Say Hello" workshop, go to [Setting up the Conversation service](#setting-up-the-conversation-service). 

If you completed the "Customize the Conversation" workshop, go to [Adding emotion to your greeting](#adding-emotion-to-your-greeting).

If this workshop is your first, complete the following tasks:

1. [Requesting access to the Watson Self Gateway](#requesting-access-to-the-watson-gateway)
2. [Downloading and installing Self](#downloading-and-installing-self)
3. [Setting up the Conversation service](#setting-up-the-conversation-service).

### Requesting access to the Watson Self Gateway

1. Request access to the Watson Self Gateway. Open [Self Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Self Gateway.
5. Click **Next**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Self Installer page is displayed.

### Downloading and installing Self

1. From the Self Installer page, download the installation package.
2. Extract the files from the package into your working directory.
3. In your working directory, run the `Self Manager.exe` file. A security warning is displayed.
4. Click **Run**. The Self Manager Configuration window is displayed.
5. Select the **Windowed** checkbox, and click **Play!**. The Self Tooling page is displayed.
6. Click **Install Self**, and log in to Bluemix. 
7. Return to the Self Manager application.

Installing Self takes a few minutes. After the installation process is complete, your instance of Self is preconfigured with the following Watson services: AlchemyAPI, Conversation, Speech to Text, and Text to Speech. The preconfiguration is enabled for a trial period of 24 hours. If you want to test Self after the trial period ends, see [After DevCon ends](#after-devcon-ends).

### Setting up the Conversation service

You must create your own instance of the Conversation service, configure Self to use the service credentials, and import a workspace for the service.

#### Creating your own instance of the Conversation service

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

#### Importing a workspace for the Conversation service

To complete this task, you must download a Conversation service workspace that we made for you and import it into the Conversation service tool.

1. Return to the Watson Self Gateway, and click **Downloads**.
2. Download the workspace file.
3. Return to the Conversation service in your browser, and select **Manage**.
4. Click **Launch tool**. The **Create workspace** page is displayed.
5. Click **Import**.
6. Click **Choose a file**, navigate to the workspace file, select it, and click **Import**. The workspace is imported and is displayed on the Workspaces page.

## Adding emotion to your greeting

**Before you begin:** Turn on the microphone on your device.

1. On the Workspaces page of the Conversation tool, click **Get started**.
2. Click **Dialog** in the navigation bar.
3. Expand this branch of the dialog tree: **#dialog** -> **@greeting** -> **@greeting:hello**.
4. Change the **fill in the blank** message of the `text` parameter to `<speak><express-as type="GoodNews">Nice to see you!</express-as></speak>`. The `GoodNews` type expresses a positive, upbeat message.
5. Click the conversation bubble icon to open a window where you can test your new message.
6. Test your change. 
  1. In the Conversation tool, click the conversation bubble icon to open a window where you can test your new message. In the **Enter something to test your bot** field, type Hello, and press **Enter**. Your new greeting enabled with Expressive Synthesis from the Text to Speech service is returned.
  2. Say hello. Your new greeting is returned as speech with Expressive Synthesis from the Text to Speech service.

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
5. Create instances of Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

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
