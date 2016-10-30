# Say Hello!

In this workshop, you install Self on a device and get it to say hello to you.

**Before you begin:** You must have a Mac or Windows laptop.

## Getting started

Getting started includes the following tasks

1. [Requesting access to the Watson Self Gateway](#requesting-access-to-the-watson-gateway)
2. [Downloading and installing Self](#downloading-and-installing-self)

### Requesting access to the Watson Self Gateway

1. Request access to the Watson Self Gateway. Open [Self Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Self Gateway.
5. Click **Next**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Self Installer page is displayed.

### Downloading and installing Self

1. From the Self Installer page, download the installation package.
  * Mac OS users, click **Download**.
  * Windows users, click the **Choose Windows** link, and click **Download**.
2. Extract the files from the package into your working directory.
3. In your working directory, run the `Self Manager.exe` file. A security warning is displayed.
4. Click **Run**. The Self Manager Configuration window is displayed.
5. Select the **Windowed** checkbox, and click **Play!**. The Self Tooling page is displayed.
6. Click **Install Self**, and log in to Bluemix. 
7. Return to the Self Manager application.

Installing Self takes a few minutes. After the installation process is complete, your instance of Self is preconfigured with the following Watson services: AlchemyAPI, Conversation, Speech to Text, and Text to Speech. The preconfiguration is enabled for a trial period of 24 hours. If you want to test Self after the trial period ends, see [After DevCon ends](#after-devcon-ends).

## Say hello!

**Before you begin:** Turn on the microphone on your device.

1. Verify that your embodiment exists. Return to the Self Manager, and click **Embodiments**. Your embodiment is in the list.
2. In the Self Monitor, click **Menu**, and then click **Conversation** so that you can see what happens when you talk to Self. Ensure that the status icon of your embodiment is green. If it is red, talk to a moderator of the workshop.
3. Say "hello" into your device's microphone. Self returns a greeting.
  
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
4. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
5. Create instances of the Speech to Text and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

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
