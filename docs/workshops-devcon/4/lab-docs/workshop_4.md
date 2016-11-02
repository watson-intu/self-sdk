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

## Signing up for a Nexmo account

1. [Sign up for a Nexmo account](https://dashboard.nexmo.com/sign-up).
2. Claim your IBM Watson credit.
  1. Click the .... 
  
## Getting your Nexmo credentials

**Pro tip:** As you complete this task, you'll receive Nexmo credentials that you'll need later. Open a new file in your favorite text editor where you can temporarily store your credentials.

1. On the [Nexmo dashboard](https://dashboard.nexmo.com/), click your account name in the top right of the window to expand the account menu.
2. Click **Settings**.
3. Copy the values of the API key and API Secret parameters and paste them in your text file.

## Adding the telephony service to Intu

1. Open the [Intu Gateway](https://rg-gateway.mybluemix.net/).
2. Click **Manage** on the right side of the page, and click **Services**.
3. In the **Filter By** fields, select your Organization and Group.
4. Click **Add Service**.
5. Specify your Nexmo credentials.
  1. In the **Service Name** field, specify `Telephony B1`.
  2. In the **User ID** field, specify the API key value from your text file.
  3. In the **Password** field, specify the API Secret value from your text file.
  4. In the **Service Endpoint** field, specify `https://nexmo-watson.mybluemix.net/`.
6. Click **Add**.
7. Restart your device to enable the telephony service and automatically provision a US phone number for your device.

## Understanding plans 

Plans are the primary driving factor in how Intu can accomplish goals. For example, you can preconfigure a plan that tells Intu how to act in a certain situation without any code changes.

Plans contain two major components:
  * **Preconditions**: A set of conditions that must be met for a particular plan to begin execution.
  * **Actions**: After all preconditions are met, a series of actions are taken to to complete a plan. Intu supports two types of actions: CreateAction, which creates a blackboard object and places it on the blackboard, and UseSkillAction, which tells Intu to execute a specified skill.

All plans are loaded when Intu starts. When a goal object is placed on the blackboard, the GoalAgent finds the best possible plan, executes the plan, and establish whether that goal was completed successfully (i.e., the plan finished with no action failures) or failed (i.e., no plan was found to carry out execution).

## Modifying plans to have the telephony service call your phone number

1. Navigate to the `./etc/shared/plans` directory, and open the `default.json file`.
2. Browse through the different plans, and notice how plans can have different preconditions based on the data that is represented.
For example, look at the first plan called "dialog_answer". It contains a set of preconditions (key is m_PreConditions) that must be answered for that plan to execute. The first parameter, the array with the key m_Params, in that precondition states that the data being analyzed must have a format as "{"answer" : {"response" : ["some value"] }, }", where the array in response must not be equal to null, while the second precondition states the response array must not have a key of "id" in the response array.
3. Search for a plan called "outgoing_call", and change the value of m_ToNumber in the second action to your phone number. Now, look at the actions for this plan. The first action will have Intu "Dialing", while the second action will carry out the execution to call the number specified.
4. Search for a plan called "ending_call", and change the number parameter to your phone number. When the phone call ends, your phone will be notified via SMS that the call ended.
5. Run Intu, and say "can you call me". When your phone rings, answer it, and have a conversation with Intu. Say "tell me a joke". After you hang up the phone, you'll receive a text message to let you know the phone call ended.

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