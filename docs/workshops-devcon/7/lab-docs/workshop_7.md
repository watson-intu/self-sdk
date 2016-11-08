# Workshop 7: Installing, configuring, and running the Intu Avatar

In this workshop, you install, configure, and run the Intu Avatar. You can use the avatar when you do not have a visual embodiment for Intu. Additionally, the avatar is useful in noisy environments because you can interact with it by using the keyboard instead of the microphone.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!. You will notice that Intu and Self are used interchangeably. Self is the technical name for Intu.

Complete the following tasks:

1. [Installing the Intu Avatar and configuring it to your instance](#installing-the-intu-avatar-and-configuring-it-to-your-instance)
2. [Running the Intu Avatar](#running-the-intu-avatar)

## Installing the Intu Avatar and configuring it to your instance

1. Return to the Intu Gateway, and click **Downloads** on the left side of the page.

2. Download the Intu Avatar and extract the files into a working directory.

3. On the Gateway page, click **View Credentials** on the left side of the page.

4. In the **Filter By** fields, select your Organization and Group, and click **Get Credentials**. In the next steps, you replace the credentials in the avatar configuration file with your own credentials.

5. Open the avatar configuration file.
  * If you're using a Mac, right-click `intu_avatar` and select **Show Package Contents**. Open **Contents** -> **Resources** -> **Data** -> **StreamingAssets** ->, and open `Config.json` in your favorite text editor.
  * If you're using Windows, open **Self-Avatar-Win64** -> **intu_****avatar_Data** -> **StreamingAssets**, and open `Config.json` in your favorite text editor.

6. Locate the `SelfID` key, and delete only its value. **Important**: Do not delete the set of quotation marks.

7. Return to your credentials in the Gateway, and copy the value of `m_BearerToken`.

8. Return to the `Config.json` file in your text editor, and paste the value of `m_BearerToken` over the value of the `BearerToken` key.

9. Return to your credentials in the Gateway, and copy the value of `m_GroupId`.

10. Return to the `Config.json` file in your text editor, and paste the value of `m_GroupId` over the value of the `GroupID` key.

11. Return to your credentials in the Gateway, and copy the value of `m_OrgId`.

12. Return to the `Config.json` file in your text editor, and paste the value of `m_OrgId` over the value of the `OrgID` key.

13. Save the changes you made in the `Config.json` file.
  
## Running the Intu Avatar

1. Navigate back to the directory where you extracted the Intu Avatar, and open the `intu_avatar` application. If a security warning is displayed, continue to run the application.

2. Select the **Windowed** option, accept the other default values, and click **Play!**.

3. Test Intu by saying hello into the microphone or by typing hello in the **Input Message Here** field.

## After DevCon ends

Your instance of Intu is preconfigured with the following Watson services: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech. The preconfiguration is enabled for 30 days. If you want to test Intu after 30 days, you must create your own instances of these services and configure Intu to use them.

### A. Creating instances of Watson services

To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favourite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the top right navigation bar.

2. In the All Categories menu on the left, under Services, click on **Watson**.

3. Create an instance of the Conversation service.
  1. Click the **Conversation** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new Conversation service instance.
  5. Copy the values for your password and username and paste them into a new text file in your favourite text editor.
  6. Click the **<--Watson** breadcrumb near the top left (directly above your Conversation service name). The list of your service instances is displayed.
  7. Add the next service instance by clicking the **Create Watson** **+** button. The Watson service catalog is displayed.

4. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same substeps 1 - 7 that you completed to create the Conversation service instance.


### B. Configuring Intu to use your service instances

To configure Intu to use your instances of these Watson services, log in to the [Intu Gateway](https://rg-gateway.mybluemix.net/) and complete the following steps:

1. Click on **MANAGE** on the left hand side navigation bar, and select **Services**. 

2. Select your Organization and Group in the top Filter by menu, if not already selected.

3. For your instances of the Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, and specify the user ID and password (saved in your text file in the previous section **Creating instances of Watson services**), and click **Save**.

4. To configure your instance of **Conversation**, navigate to **DOWNLOADS** on the left of your Intu Gateway browser page, download the **Intu Starter Kit**, and follow the instructions in the `readme.txt` file. Alternatively, go to the instructions for **Workshop 2**, and follow the steps in: **1. Setting up the Conversation service**.
 

**Important:** Do not change the service endpoint for your services unless you are an enterprise user.
