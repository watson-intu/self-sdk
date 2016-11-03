# Workshop 1: Say Hello!

In this workshop, you install Intu on your laptop and get it to say hello to you.

**Before you begin:** You must have a Mac or Windows laptop.

## Getting started

Getting started includes the following tasks:

1. [Requesting access to the Watson Intu Gateway](#requesting-access-to-the-watson-intu-gateway)
2. [Downloading Intu](#downloading-intu)
3. [Installing Intu](#installing-intu)
[Challenge: Configuring the Intu avatar for your instance](#configuring-the-intu-avatar-for-your-instance)

### Requesting access to the Watson Intu Gateway

1. Request access to the Watson Intu Gateway. Open [Intu Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Log In** and specify your IBM Bluemix credentials.
3. In the **Organization** field, specify the name of the organization that you represent.
4. In the **Business Justification** field, briefly explain why you need access to the Intu Gateway.
5. Click **Submit**. After your request for access is approved, you receive a confirmation email.
6. Open the confirmation email, and click the link. The Intu Gateway Log In page is displayed again.
7. Click **Log In**. The Intu Downloads page is displayed.

### Downloading Intu

1. On the Intu Downloads page, download the appropriate Intu Tooling installation package for your platform.
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

## Say hello!

**Before you begin:** Turn on the microphone on your device.

1. Return to the Intu Gateway, and click **Embodiments** to verify that your embodiment exists. Your embodiment is in the list.
2. In the Intu Manager, click **Menu**, and then click **Conversation** so that you can see what happens when you talk to Intu. Ensure that the status icon of your embodiment is green. If it is red, talk to a moderator of the workshop.
3. Say "hello" into your device's microphone. Intu returns a greeting.

## Challenge: Configuring the Intu avatar for your instance

To complete this challenge, do the tasks in Workshop 7: Installing, configuring, and running the Intu Avatar.
  
## After DevCon ends

If you want to test Intu after the trial period ends, you must create your own instances of these services and configure Intu to use them.

### Creating instances of Watson services
To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the navigation bar.
2. Click **Watson** in the categories menu.
3. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
4. Create instances of the Natural, Language Classifier, Speech to Text, and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

### Configuring Intu to use your service instances

Your installation is preconfigured to use the default Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services. To configure Intu to use your instances of these services, complete the following steps:

1. Expand **All Organizations** by clicking the arrow icon.
2. Click the name of your organization.
3. Expand your organization by clicking the arrow icon.
4. Click the name of your group.
5. Click **Services** in the navigation bar.
6. For your instances of the Conversation service, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
