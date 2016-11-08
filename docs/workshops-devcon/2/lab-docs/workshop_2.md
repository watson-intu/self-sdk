# Workshop 2: Customizing the Conversation Service

In this workshop, you customize the greeting in the dialog from the Conversation service workspace and extend the conversation by adding new intents, new entities, and new dialog.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!. You will notice that Intu and Self are used interchangeably. Self is the technical name for Intu.

Complete the following tasks:

1. [Setting up the Conversation service](#setting-up-the-conversation-service)
2. [Customizing the greeting](#customizing-the-greeting)
3. [Adding a new conversation flow with a new intent, new entities, and new dialog](#adding-a-new-conversation-flow-with-a-new-intent,-new-entities,-and-new-dialog)
4. [Configuring Self to use your new Conversation instance](#configuring-Self-to-use-your-new-Conversation-instance) 

## 1. Setting up the Conversation service

You must create your own instance of the Conversation service in Bluemix, configure Intu to use this service instance, and import a Conversation workspace.

### A. Creating your own instance of the Conversation service

1. Open a new browser window, and [log in to Bluemix](https://console.ng.bluemix.net/dashboard/applications/).

2. On the Bluemix dashboard, click **Catalog** in the top-right navigation bar.
3. In the All Categories menu on the left, under Services, click on **Watson**.
4. Click on the **Conversation** tile.
  1. Keep all the default values, and click **Create**.
  2. Click the **Service Credentials** tab.
  3. Click **View Credentials** for your newly created Conversation service.
  4. Copy the values for your `password` and `username` and paste them into a new text file in your favourite text editor. You will need these credentials in Section 4 below. **Leave this browser window open.**

### B. Importing a workspace into the Conversation service

To complete this task, you must download the **Intu Starter Kit**, which contains a complete Conversation service workspace **Self-Dialog** that you need for this workshop.

1. Open a new browser window and [log in to the Intu Gateway](https://rg-gateway.mybluemix.net/).

1. Click **DOWNLOADS** on the left hand side navigation bar.
2. Click on the big download arrow for **Download Intu Starter Kit**. Extract the contents of the zip file into a working directory and take note of this so that you know where to find it in step 7 below. **Note:** The directory that you select for this starter kit does not matter; it contains only the `intu-workspace-full.json` file required in step 7 below and a `readme.txt` file. 
3. Return to the Conversation service window in Bluemix (left open in your browser in step 4 above), and select **Manage**.
4. Click **Launch tool**. You are directed to the Watson Conversation page. Click **Log in with IBM ID** if you see this window. 
 
5. **Do not create a new workspace.** Click the **Import** icon next to the green **Create +** button.
6. Click **Choose a file**, navigate to the directory where you extracted the Intu Starter Kit in step 2, select the `intu-workspace-full.json` file, and click **Import**. Keep the default settings so that you import everything. The workspace titled **Self-Dialog** is imported and is displayed on the Workspaces page.



## 2. Customizing the greeting

**Before you begin:** Turn on the microphone on your device.

1. On the Self-Dialog workspace, click **Get started**.

2. Click **Dialog** in the navigation bar at the top.
3. Expand this branch of the dialog tree by clicking through the following dialog nodes: **#dialog** -> **@greeting** -> **@greeting:hello**. You may need to scroll down as you're clicking through.
4. In the **Watson Response** box of **greeting:hello**, in `“values”: [ ]`, the current four responses for when you say "hello" are displayed. You can edit, remove, and add to these. For now, add a comma (,) directly after the final quotation mark (") at the end of "why hello there[emote=wave_anim]" and on a new line directly below it, add "Hello Workshop Two participant!".
5. Click the **Conversation bubble** icon on the top right to open a **Try it out** panel where you can test your new response.
6. Test your response.
  1. In the **Enter something to test your bot** field at the bottom of the **Try it out** panel, type Hello, and press **Enter**. 
  2. Submit hello a few times until your new response is returned. 
  
  		**Note:** Conversation may take a few minutes to train, as indicated by the pop-up message: **Watson is training on your recent changes** in purple.
7. Close the **Try it out** box by clicking on the **X** icon (directly above the  **Clear** icon).
  
## 3. Adding a new conversation flow with a new intent, new entities, and new dialog

### A. Create a new intent

1. Click on **Intents** on the top navigation bar of the Conversation browser window you're currently in.

2. Create a new intent.
  1. Click the green **Create new** button on the top left to create a new intent.
  2. The hashtag (#) identifies intents. After the # for intent name, type **dialog_capitals**.
  3. Click **Create** on the top right. The intent **#dialog_capitals** is at the top of your intents list.
  4. Click **#question** in your intents list, and search for **capital**. (You can use **Ctrl + F** or **Cmd + F**). Four entries for "What is the capital of..." should be displayed.
  5. Select the checkbox beside each of the four entries.
  6. Scroll to the top of **#question**, and select **Move to...**.
  7. In the **# Intent name** box that is displayed, start typing **dialog_capitals**, and select it when it appears in the dropdown list. **Note:** The move of the four entries from **#question** to **#dialog_capitals** may take a minute or more to update, and you may need to refresh your page. 
  8. Scroll to the top of your intents list and click on **#dialog_capitals**, where you should see the four entries that you moved.
  8. Click **#dialog_capitals**, and select **Add a new user example…**.
  9. Type **What is the capital of Australia**, and press **Enter**. This entry is added to the list.
  10. Repeat steps 8 and 9 for **What is the capital of Canada** and **What is the capital of New Zealand**.

### B. Create new entities

  1. Click **Entities** on the top navigation bar, and click **Create new**.
  
  2. Type the new entity **countries**, and click **Create**. The entity **@countries** is displayed in your Entities list.
  3. Click the @countries entity, and select **Add a new value**.
  4. Type **Australia**, and press **Enter**.
  5. Repeat steps 3 and 4 to add two new values for **Canada** and **New Zealand**.

### C. Create new dialog

1. Click **Dialog** on the top navigation bar, and click on any of the existing branches in grey, e.g. **#dialog**. You will see a green **+** displayed at the **bottom** of the box that has expanded.
  
2. Click on this bottom green **+** and in the new box that opens, type **#dialog_capitals**.
  
3. Click within the **#dialog_capitals**. The box becomes outlined in green. Now, click the green **+** icon on the right side of the box. 
  
4. In the new box that is displayed, under **Enter new condition**, type **@countries:Australia**.
  
5. Click the **three dots** in the top right of the #dialog_capitals box, and select **Continue from**. 

6.	Click on the **@countries:Australia box**, then click on **Go to condition** which has just popped up.

7.	In the white **@countries:Australia** box, type in the response: **The capital of Australia is Canberra.**

8.	Click on the **@countries:Australia** where you just typed in the response: The capital of Australia is Canberra. The box will become outlined in green. Click on the green **+** icon on the bottom of the box. 

9.	In the new box that appears, under **Enter a condition**, type in: **@ countries:Canada,** and for the response: **The capital of Canada is Ottawa.**

10.	Repeat for **@capitals:New Zealand**, where the response will be **The capital of New Zealand is Wellington.**

11. Click the **Conversation bubble** icon on the top right to open a **Try it out** panel where you can test your new response.

12.	Test your response.

	1. In the **Enter something to test your bot** field at the bottom of the **Try it out** panel, type: **What is the capital of Australia?** You should see the response: **The capital of Australia is Canberra.** Note that Conversation may take a few minutes to train, as indicated by the pop-up message: **Watson is training on your recent changes** in purple.
	2. Try asking for the capital of Canada and New Zealand.

## 4. Configuring Self to use your new Conversation instance

Your installation of Self is preconfigured to use the default Conversation service. To configure Self to use your new Conversation instance which you just modified, complete the following steps:

1.	After testing your changes in the previous step above, click on the name of your Conversation Workspace **Self-Dialog** in the top left of your browser window.

2.	This returns you to your Workspaces page, where you will see the Self-Dialog Workspace box. Click on the **three dots** in the top right hand corner of this box. Select **View details**.   

3.	Now copy the Workspace ID into the text file you created earlier in Section 1.

4.	Return to the [Intu Gateway](https://rg-gateway.mybluemix.net/).

5.	Click on **MANAGE** on the left hand side navigation bar, and select **Services**. Select your Organization and Group in the top Filter by menu, if not already selected.

6.	For your instance of **Conversation** click **Edit**, and copy in your username and password you saved earlier, and your Workspace ID from your text file. Note that your Workspace ID is to be pasted into the **self_dialog** parameter. **Important:** Do not change the service endpoint unless you are an enterprise user. Click **Save**.

7.	If Intu is already **running** (from Workshop 1), ask it a question, e.g. **What is the capital of Australia?**

8. If Intu is **not running, run Intu Manager**.

	1. Navigate to the directory where you installed the Intu Manager from Workshop 1 and run the application. (If you're using a **Mac**, right-click on the Intu Manager and select Open. If you're using **Windows**, double-click on the Intu Manager to run it.) 
	2. If a security warning is displayed, accept the risk and open the file.
	3. Make sure the **Windowed** checkbox is selected, accept the other default values, and click **Play!**. 
	4. The Intu Manager page is displayed in your browser window. Click **Log In**. You are prompted to return to the Intu Manager. 
	5. Select **Manage Intu**. A new Intu Tooling log in page is displayed. Click **Log in.**
	6. Your Organization and Group should be preselected in the dropdown menu and Intu should now be running.

## After DevCon ends
Your instance of Intu is preconfigured with the following Watson services: Natural Language Classifier, Speech to Text, and Text to Speech. The preconfiguration is enabled for 30 days. If you want to test Intu after 30 days, you must create your own instances of these services and configure Intu to use them.

### A. Creating instances of Watson services
To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favourite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the top right navigation bar.

2. In the All Categories menu on the left, under Services, click on **Watson**.

3. Create an instance of the Conversation service.
  1. Click the **Natural Language Classifier** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new Conversation service instance.
  5. Copy the values for your password and username and paste them into a new text file in your favourite text editor.
  6. Click the **<--Watson** breadcrumb near the top left (directly above your Natural Language Classifier service name). The list of your service instances is displayed.
  7. Add the next service instance by clicking the **Create Watson** **+** button. The Watson service catalog is displayed.

4. Create instances of the Speech to Text and Text to Speech services by repeating the same substeps 1 - 7 that you completed to create the Natural Language Classifier service instance.

### B. Configuring Intu to use your service instances

To configure Intu to use your instances of these Watson services, log in to to the [Intu Gateway](https://rg-gateway.mybluemix.net/) and complete the following steps:

1. Click on **MANAGE** on the left hand side navigation bar, and select **Services**. 

2. Select your Organization and Group in the top Filter by menu, if not already selected.

3. For your instances of the Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, and specify the user ID and password (saved in your text file in the previous section **Creating instances of Watson services**), and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
