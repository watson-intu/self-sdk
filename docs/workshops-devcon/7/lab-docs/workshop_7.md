# Workshop 7: Installing, configuring, and running the Intu Avatar

In this workshop, you install, configure, and run the Intu Avatar. You can use the avatar when you do not have a visual embodiment for Intu. Additionally, the avatar is useful in noisy environments because you can interact with it by using the keyboard instead of the microphone.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!

Complete the following tasks:

1. [Installing the Intu Avatar and configuring it to your instance](#installing-the-intu-avatar-and-configuring-it-to-your-instance)
2. [Running the Intu Avatar](#running-the-intu-avatar)

## Installing the Intu Avatar and configuring it to your instance

1. Return to the Intu Gateway, and click **Downloads** on the left side of the page.
2. Download the Intu Avatar and extract the files into a working directory.
3. On the Gateway page, click **View Credentials** on the left side of the page.
4. In the **Filter By** fields, select your Organization and Group, and click **Get Credentials**. In the next steps, you replace the credentials in the avatar configuration file with your own credentials.
5. Open the avatar configuration file.
  * If you're using a Mac, right-click `intu_avatar` and select **Show Package Contents**. Open **Contents**->**Resources**->**Data**->**StreamingAssets**->, and open `Config.json` in your favorite text editor.
  * If you're using Windows, open **Self-Avatar-Win64**->**intu_avatar_Data**->**StreamingAssets**, and open `Config.json` in your favorite text editor.
6. Locate the `SelfID` parameter, and delete its value. **Important**: Do not delete the set of quotation marks.
7. Replace the value of the `BearerToken` parameter in `Config.json` with the value of `m_BearerToken` from your credentials.
8. Replace the value of the `GroupID` parameter in `Config.json` with the value of `m_GroupId` from your credentials.
9. Replace the value of the `OrgID` parameter in `Config.json` with the value of `m_OrgId` from your credentials.
10. Save your changes.
  
## Running the Intu Avatar

1. Open the `intu_avatar` application. If a security warning is displayed, continue to run the application.
2. Select the **Windowed** option, and click **Play!**.
3. Test Intu by saying hello into the microphone or by typing hello in the **Input Message Here** field.

## After DevCon ends

If you want to test Self after the trial period ends, you must create your own instances of these services and configure Self to use them.

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
