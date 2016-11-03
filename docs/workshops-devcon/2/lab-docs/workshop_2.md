# Workshop 2: Customizing the conversation 

In this workshop, you customize the greeting in the dialog from the Conversation service workspace and extend the conversation by adding new intents, new entities, and new dialog.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!

Complete the following tasks:

1. [Setting up the Conversation service](#setting-up-the-conversation-service)
2. [Customizing the greeting](#customizing-the-greeting)
3. [Adding a new conversation flow with a new intent, new entities, and new dialog](#adding-a-new-conversation-flow-with-a-new-intent,-new-entities,-and-new-dialog)

## Setting up the Conversation service

You must create your own instance of the Conversation service, configure Intu to use the service credentials, and import a workspace for the service.

### Creating your own instance of the Conversation service

To complete this workshop, you must create your own instance of the Conversation service and use its service credentials.

1. Open a new browser window, and [log in to Bluemix](https://idaas.iam.ibm.com/idaas/mtfim/sps/authsvc?PolicyId=urn:ibm:security:authentication:asf:basicldapuser).
2. On the Bluemix dashboard, click **Catalog** in the top-right navigation bar.
3. In the Categories menu on the left, under Services, click **Watson**.
4. Click the **Conversation** tile.
  1. Keep all the default values, and click **Create**.
  2. Click the **Service Credentials** tab.
  3. Click **View Credentials** for the new service instance. You'll return for these later.
5. Configure Intu to use your Conversation service instance.
  1. Return to the Watson Intu Gateway window in your browser. Expand **All Organizations** by clicking the arrow icon.
  2. Click the name of your organization.
  3. Expand your organization by clicking the arrow icon.
  4. Click the name of your group.
  5. Click **Services** in the navigation bar.
  6. For your instance of the Conversation service, click **Edit**.
  7. Return to the window where your Conversation service credentials are displayed.
  8. Copy the user ID and password values, and paste them in the appropriate fields for the Conversation service in the Gateway window.
  8. Do not close the Conversation service credentials tab in your browser. You will return to it later.

**Important:** Do not change the service endpoint unless you are an enterprise user.

### Importing a workspace into the Conversation service

To complete this task, you must download a Intu Starter Kit, which contains a complete Conversation service workspace that you need to complete this workshop.

1. Return to the Watson Intu Gateway, and click **Downloads** on the left hand navigation bar.
2. Download the Intu Starter Kit and extract it into a working directory.
3. Return to the Conversation service in your browser, and select **Manage**.
4. Click **Launch tool**. You are directed to the Watson Conversation page. (Do not create a new workspace)
5. Click **Import** icon next to the create button.
6. Click **Choose a file**, navigate to the directory where you extracted the Intu Starter kit in step 2, select the `intu-workspace-full.json` file, and click **Import**. Keep the default settings so that you import everything. The workspace titled Intu-Dialog is imported and is displayed on the Workspaces page.

## Customizing the greeting

**Before you begin:** Turn on the microphone on your device.

1. On the Self-Dialog workspace, click **Get started**.
2. Click **Dialog** in the navigation bar at the top.
3. Expand this branch of the dialog tree by clicking through the following dialog nodes: **#dialog** -> **@greeting** -> **@greeting:hello**.
4. In the **Watson Response** box of **greeting:hello**, in `“values”: [ ]`, the current four responses for when you say "hello" are displayed. You can edit, remove, and add to these. For now, add a comma (,) directly after the final quotation mark (") at the end of "why hello there[emote=wave_anim]" and on a new line directly below it, add "Hello Workshop Two participant!".
5. Click the **conversation bubble** icon on the top right to open a **Try it out** panel where you can test your new response.
6. Test your change. 
  1. In the **Enter something to test your bot** field at the bottom of the **Try it out** panel, type Hello, and press **Enter**. Submit hello until your new response is returned.
  2. Now, say hello to your embodiment. Say hello until your new response is returned through speech.
  
## Adding a new conversation flow with a new intent, new entities, and new dialog

1. Click your Self-Dialog workspace on the top left. The Workspace page is displayed.
2. Click anywhere inside the Intu-Dialog box.
3. Create a new intent.
  1. Click the green **Create new** button on the top left to create a new intent.
  2. The hashtag (#) identifies intents. After the # for intent name, type `dialog_capitals`.
  3. Click **Create** on the top right. The intent #dialog_capitals is at the top of your intents list.
  4. Click **#question** in your intents list, and search for `capital`. (Alternatively, you can use **Ctrl + F** or **Cmd + F**). Four entries for "What is the capital of..." are displayed.
  5. Select the checkbox beside each of the four entries.
  6. Scroll to the top of #question, and select **Move to...**.
  7. In the # intent name box that is displayed, type `dialog_capitals`. The four entries you selected are displayed under #dialog_capitals. This process might take a minute or more and you may need to refresh your page.
  8. Click **#dialog_capitals**, and select **Add a new user example…**.
  9. Type `What is the capital of Australia`, and press **Enter**. This entry is added to the list.
  10. Repeat step 9 for `What is the capital of Canada` and `What is the capital of New Zealand`.
4. Create new entities.
  1. Click **Entities** on the top navigation bar, and click **Create new**.
  2. Type the new entity `@countries`, and click **Create**. The entity @countries is displayed in your Entities list.
  3. Click the @countries entity, and select **Add a new value**.
  4. Type `Australia`, and press **Enter**.
  5. Repeat step 6 to add two new values for Canada and New Zealand.
5. Create a new dialog.
  1. Click **Dialog** on the top navigation bar, and click any of the existing branches where you see a green **+** displayed at the bottom of the box that has expanded.
  2. Click the bottom green **+** and in the new box that opens, type `#dialog_countries`.
  3. Click within the #dialog_countries. It becomes outlined in green. Now, click the green **+** icon on the right side of the box. 
  4. In the new box that is displayed, under **Enter new condition**, type `@countries:Australia`.
  5. Click the three dots in the #dialog_countries box, and select **Continue from**. 
  6. In the white @countries:Australia box, type the following response, `The capital of Australia is Canberra`.
  7. Click the @countries:Australia box where you just typed the `The capital of Australia is Canberra` response. It becomes outlined in green.
  8. Click the green **+** icon on the bottom of the box. 
  9. In the new box that is displayed, under **Enter a condition**, type `@capitals:Canada`, and for the response type `The capital of Canada is Ottawa`.

## After DevCon ends

If you want to test Intu after the trial period ends, you must create your own instances of these services and configure Intu to use them.

### Creating instances of Watson services
To use Intu, you need operational instances of the following services in Bluemix: Conversation, Natural Language Classifier, Speech to Text, and Text to Speech.

**Pro tip:** As you complete this task, you'll receive credentials for each service instance, and you'll need these credentials later. Open a new file in your favorite text editor and create a section for each service so that you can temporarily store its credentials.

1. On the Bluemix dashboard, click **Catalog** in the navigation bar.
2. Click **Watson** in the categories menu.
3. Create an instance of the Natural Language Classifer service.
  1. Click the **Natural Language Classifier** tile.
  2. Keep all the default values, and click **Create**.
  3. Click the **Service Credentials** tab.
  4. Click **View Credentials** for the new service instance.
  5. Copy the values of the `password` and `username` parameters and paste them in your text file.
  6. Click the **Watson** back arrow on the upper left corner. The list of your service instances is displayed.
  7. Add the next service instance by clicking the hexagonal **Create Watson +** button. The Watson service catalog is displayed.
4. Create an instance of **Speech to Text** and **Text to Speech** services by repeating the same steps 1 - 7 that you completed to create the Natural Language Classifier instance.

### Configuring Intu to use your service instances
Your installation is preconfigured to use the Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services. To configure Intu to use your instances of these services, complete the following steps:
 1.  Return to the Watson Intu Gatewa and login.
 2.  Under **MANAGE** on the left hand navigation bar click on **Services**
 3.  To the right of "Filter By:" select your Orgnaization and Group
 4.  For your instances of the Conversation service, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
