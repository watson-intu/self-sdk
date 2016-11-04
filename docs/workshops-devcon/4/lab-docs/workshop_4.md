# Workshop 4: Customizing Intu to call your phone

In this workshop, you will configure Intu's telephony plan to have Intu call your phone by using the Nexmo Cloud Communications API. 

A plan is a set of preconditions, a set of actions, and a set of postconditions to address a given goal. You'll learn more about the importance of plans and how they work later in this workshop.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!

Complete the following tasks:

1. [Signing up for a Nexmo account](#signing-up-for-a-nexmo-account)
2. [Getting your Nexmo credentials](#getting-your-nexmo-credentials)
3. [Adding the telephony service to Intu](#adding-the-telephony-service-to-Intu)
4. [Understanding plans](#understanding-plans)
5. [Modifying plans to have the telephony service call your phone number](#modifying-plans-to-have-the-telephony-service-call-your-phone-number)

## 1. Signing up for a Nexmo account

1. [Sign up for a Nexmo account](https://dashboard.nexmo.com/sign-up)
  
## 2. Getting your Nexmo credentials


1. On the [Nexmo dashboard](https://dashboard.nexmo.com/), click your account name in the top right of the window to expand the account menu.
2. Click **Settings**.
3. Copy the values of the **API key** and **API Secret** parameters, and paste them into a new text file using your favourite text editor.

## 3. Adding the telephony service to Intu

1. Open the [Intu Gateway](https://rg-gateway.mybluemix.net/).
2. Click **MANAGE** on the left side of the page, and click **Services**.
3. In the **Filter By** fields, select your Organization and Group.
4. Click **Add Service**.
5. Specify your Nexmo credentials.
  1. In the **Service Name** field, specify **TelephonyV1**.
  2. In the **User ID** field, specify the **API key** value from your text file.
  3. In the **Password** field, specify the **API Secret** value from your text file.
  4. In the **Service Endpoint** field, specify: **ws://nexmo-watson.mybluemix.net/ws-embodiment**
6. Click the blue **Add** box.
7. If you currently have Intu running, stop it to enable the Telephony service and automatically provision a US phone number for your device.

## 4. Understanding plans 

Plans are the primary driving factor in how Intu can accomplish goals. For example, you can preconfigure a plan that tells Intu how to act in a certain situation without any code changes.

Plans contain two major components:

  * **Preconditions**: A set of conditions that must be met for a particular plan to begin execution.
  * **Actions**: After all preconditions are met, a series of actions are taken to to complete a plan. Intu supports two types of actions: CreateAction, which creates a blackboard object and places it on the blackboard, and UseSkillAction, which tells Intu to execute a specified skill.

All plans are loaded when Intu starts. When a goal object is placed on the blackboard, the GoalAgent finds the best possible plan, executes the plan, and establish whether that goal was completed successfully (i.e., the plan finished with no action failures) or failed (i.e., no plan was found to carry out execution).

## 5. Modifying plans to have the Telephony service call your phone number

1. From the directory where you installed Intu, navigate to the **plans** directory. 
	* If you used the Installer and you are using a **Mac**, the directory is: **/Applications/IBM/Self/latest/etc/shared/plans** 
	* If you are using **Windows**, it is: **C:\Users\{user}\AppData\LocalLow\IBM\Self\{latest_version}\etc\shared\latest\plans**
2.  Open the `default.json` file in **plans**.
3. Browse through the different plans, and notice how plans can have different preconditions based on the data that is represented.
For example, look at the first plan called "dialog_answer". It contains a set of preconditions (key is m_PreConditions) that must be answered for that plan to execute. The first parameter, the array with the key m_Params, in that precondition states that the data being analyzed must have a format as "{"answer" : {"response" : ["some value"] }, }", where the array in response must not be equal to null, while the second precondition states the response array must not have a key of "id" in the response array.
4. Search for a plan called **outgoing_call**, and change the value of **m_ToNumber** in the second action to your phone number (Be sure to include country code first i.e. **1555333213**). Now, look at the actions for this plan. The first action will have Intu "Dialing", while the second action will carry out the execution to call the number specified.
5. **Run Intu**. 
(In the directory where you installed Intu, if you're using a Mac, right-click on the Intu Manager and select Open. If you're using Windows, double-click on the Intu Manager to run it. If a security warning is displayed, accept the risk and open the file.
Select the Windowed checkbox, accept the other default values, and click Play!. If a security warning is displayed, accept the risk. The Intu Manager page is displayed. Click Log In. You are prompted to return to the Intu Manager application.) 
Click **Manage Intu**. A new Intu Tooling sign-in page is displayed. Click Log in. 
6. Click on the **small grey swipe icon** next to your device to **restart Intu**.
7. Ask Intu "Can you call me". When your phone rings, answer it, and have a conversation with Intu. Say "Tell me a joke". You should hear Watson tell you a joke. You can continue to talk or hang up.

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

1. Click **Manage** on the left side of the page, and click **Services**.
2. In the **Filter By** fields, select your Organization and Group.
3. For your instances of the Conversation service, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
