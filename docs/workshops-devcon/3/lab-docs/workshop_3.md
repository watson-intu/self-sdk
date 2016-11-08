# Workshop 3: Creating an emotion agent

In this workshop, you create an emotion agent. Agents make decisions about how Intu operates and responds. The emotion agent uses the Tone Analyzer service on Bluemix, which analyzes a person's tone and determines whether that tone is positive or negative.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!.

Complete the following tasks:

1. [Setting up the Tone Analyzer service](#setting-up-the-tone-analyzer-service)
2. [Understanding some Intu terminology](#understanding-some-intu-terminology)
3. [Building the Intu SDK](#building-the-intu-sdk)
4. [Creating an emotion agent](#creating-an-emotion-agent)
5. [Configuring Intu to include your emotion agent](#configuring-intu-to-include-your-emotion-agent)

## Setting up the Tone Analyzer service

To set up the service, you must create an instance of it, and configure Intu to use it.

### Creating your own instance of the Tone Analyzer service

1. [Log in to Bluemix](https://idaas.iam.ibm.com/idaas/mtfim/sps/authsvc?PolicyId=urn:ibm:security:authentication:asf:basicldapuser).
2. On the Bluemix dashboard, click **Catalog** in the top-right navigation bar.
3. In the Categories menu on the left, under Services, click **Watson**.
4. Click the **Tone Analyzer** tile.
  1. Keep all the default values, and click **Create**.
  2. Click the **Service Credentials** tab.
  3. Click **View Credentials** for the new service instance. Leave this window open. You'll need the credentials in the next steps.
5. Configure Intu to use your Tone Analyzer service instance.
  1. Return to the Watson Intu Gateway. Expand **All Organizations** by clicking the arrow icon.
  2. Click the name of your organization.
  3. Expand your organization by clicking the arrow icon.
  4. Click the name of your group.
  5. Click **Services** in the navigation bar.
  6. Click **Add Service**.
  7. In the **Service Name** field, specify Tone Analyzer.
  8. Return to your Tone Analyzer credentials window, and copy the user ID value.
  8. Return to the Gateway, and paste your Tone Analyzer user ID value in the **User ID** field.
  9. Return to your Tone Analyzer credentials window, and copy the password value.
  10. Return to the Gateway, and paste your Tone Analyzer password value in the **Password** field.
  11. Return to your Tone Analyzer credentials window, and copy the url value.
  12. Return to the Gateway, and paste your Tone Analyzer url value in the **Service Endpoint** field.
  13. Click **Add**.

## Understanding some Intu terminology

Before you create an emotion agent, become familiar with the following terminology:

  * **Blackboard**: The central message broker on which all the agents post data and listen for incoming data.
  * **Publish**: To push data onto the blackboard under a particular topic. 
  * **Subscribe**: Subscribing to a topic on the blackboard means to listen to the blackboard and wait for any other agents to post to the blackboard under a particular topic.
  * **Topic**: A channel to which Agents publish and subscribe.

## Building the Intu SDK

Follow the instructions for your platform.

**Before you begin**:

1. [Download the Intu SDK](https://github.ibm.com/watson-labs-austin/self-sdk).
2. Create a new directory named `intu` in your home directory, and unzip the SDK package into it.

### Building the SDK for OS X

1. Install Homebrew by completing the following steps:
   1. Open a new terminal window, and run the command:
   ```
   ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
   ```
   The following message is displayed:
   ```
   Press RETURN to continue or any other key to abort
   ```
   2. Press **Return** or **Enter**. A prompt for your laptop's password is displayed.
   3. Specify your password, and press **Return** or **Enter**. If you're using Sierra OS X, the following message is displayed:
   ```
   HEAD is now at 89fd34b Merge pull request #1368 from MikeMcQuaid/build-options-file
   Error: /usr/local/Cellar is not writable. You should change the
   ownership and permissions of /usr/local/Cellar back to your
   user account:
   sudo chown -R $(whoami) /usr/local/Cellar
   Failed during: /usr/local/bin/brew update --force
   ```
   4. Run `sudo chown -R $(whoami) /usr/local/Cellar`.
2. Install CMake by using Homebrew. Run `brew install cmake`.
3. Download Anaconda 4.2.0 Python 2.7 version by using the Graphical Installer. It is required to correctly configure pip in the following step.
   1. Open (https://www.continuum.io/downloads).
   2. Click the solid blue Graphical Installer button for Python V2.7. It should be 403 MB. The .pkg file downloads.
   3. After the file is downloaded, double-click it, and follow the prompts to install Anaconda.
4. Set up qiBuild.
   1. Run the following commands:
   * `pip install qibuild`
   * `qibuild config --wizard`. Be sure to include the two hyphens before the word wizard in the last command.
   2. When the "Please choose a generator" prompt is displayed, specify **1**, and press **Enter**.
   3. When the "Please choose an IDE" prompt is displayed, specify **1**, and press **Enter**.
5. Navigate to the `self-sdk-develop` directory in the Intu directory you created at the very beginning of this workshop. Open a new terminal window, and run the following commands:
  * `cd intu/self-sdk-develop`
  * `./scripts/build_mac.sh`
  
If you're running this script for the first time and you see the following message, don't worry. You don't have the toolchain.
```
[ERROR]: Exception No such toolchain: mac-self
Known toolchains are:
```

### Building the SDK for Windows

1. Install [Visual Studio 2015](https://www.visualstudio.com/downloads/).
2. Open the solution found in vs2015/self-sdk.sln
3. Right click on the "self-sdk" project and select "Set as Startup Project".
4. Right click on the "self-sdk" projet, open properties. In the Debugging tab of the properties, you will need to change "Working Directory" to "$(TargetDir)".
5. Select Build->Build Solution
6. Select Debug->Start Debugging to run the project with debugging

**Important**: If you use SourceTree, the process might get stuck when trying to pull by using SSH. Run the following commands on the command line to fix the problem with the git client that's trying to be interactive:
* cd "C:\Program Files (x86)\Atlassian\SourceTree\tools\putty"
* plink git@github.ibm.com

## Creating an emotion agent

1

**Windows users do the following five steps.**

1. In Visual Studio, in the `examples` directory, add a new Win32 Project called `workshop_three_plugin`, and click **OK**.
2. Click **Next**, select **Application Type as DLL**, and uncheck **Precompiled header and Security Development Lifecycle (SDL) checks** under **Additional options**.
3. Click **Finish**.
4. Remove the Header Files, Resource Files, and Source Files directories that were newly created in the solution. 
5. Right-click the project, open **Properties**, and make the following changes. Before you begin, make sure Configuration at the top left is set to **All Configurations**.

* Change the value of **General->Character Set** to **Use Multi-Byte Character Set**.
* Go to **Linker->General->Additional Library Directories->**, and add `../../lib/$(Configuration);../../lib/boost_1_60_0/stage/lib/`.
* Replace the value of **Linker->Input->Additional Dependencies** with the following value: `jsoncpp.lib;self.lib;wdc.lib;%(AdditionalDependencies)`.
* Go to **Build Events->Post-Build Event->Command Line**, and add `copy /Y "$(TargetPath)" "$(ProjectDir)....\bin\$(Configuration)\"`.

**OS X and Windows users do the following four steps.**

4. Make a folder in the sdk/examples folder (in filesystem, not Visual Studio) and call it workshop_three 
5. Right click on the created workshop solution, and add a new Filter and call it agent
  ```
3. Navigate back to the `workshop_three` directory, create a new directory, and name it `agents`.
4. Locate the Workshop 3 code snippet files in `self-sdk-develop/docs/workshops-devcon/3/code-snippets/WorkshopThreeAgent_start`. Copy the `WorkshopThreeAgent.cpp` and the `WorkshopThreeAgent.h` files, and paste them into the `agents` directory that you created.
5. Open the `WorkshopThreeAgent.cpp` file, which contains the following functions that enable the emotion agent you'll create. All Agents in Intu inherit from the IAgent interface. Agent's are responsible for listening for messages that are placed on the Blackboard and react accordingly. All agents will have a Serialize and Deserialize function (from the ISerializable interface). Deserialize function will deserialize JSON text (text from the body.json file) to instantiate an object, while Serialize will serialize the the data back into JSON to update the body.json file. Additionally, all agents will have an OnStart and OnStop function which is inherited from the IAgent interface. See below for more information on what these functions are doing.

  * **OnStart()**: Starts the Emotion Agent. The manager that starts all agents is in the AgentSociety class. The OnStart function is called on the main thread, so we will do as little processing as necessary in this function. Mainly, all agents should initialize any services that they will use in this function (i.e., Tone Analyzer), as well as subscribe to any blackboard objects. In OnStart, look at line number 66. There we are getting an instance of the BlackBoard and declaring to notify us whenever an Emotion object has been placed there. When data is received (in this case a ThingEvent object), we set our callback to be the OnEmotion function. The macro TE_ALL states that we are only interested when the object has been added to the Blackboard. Another example of a macro we may be interested in is the TE_STATE, which says tha we are interested in only when the Blackboard object state has changed (i.e., from processing to finished). Look in the Blackboard.h file to see other macros. Additionally, we can start timers in the OnStart function like we do on line 71. A timer will be executed after a predefined float value (in this case it's m_EmotionTime). The VOID_DELEGATE argument is our callback function to allow functions to be called on different threads. There are Delegate calls, which call functions that take arguments, and VOID_DELEGATES which call functions with no arguments. When the timer has been reached, the OnEmotionCheck function will be called. The last two boolean parameters state if the function should be invoked on the main thread and if the timer should continually repeat. In this case, we want the function OnEmotion to be executed on the main thread and to continually repeat.
  * **OnStop()**: Stops the emotion agent. The manager that stops all agents is in the AgentSociety class. Functionally, we want to unsubscribe from any blacboard objects that we subscribed to in the OnStart function, as well as reset any timers that may still be in use, like the m_spEmotionTimer. 
  * **OnEmotion()**: Is the callback when the Emotion object is posted to the blackboard. 
  * **OnText()**: The callback when a Text object is placed on the blackboard. A Text object is created from the TextExtractor with the transcription of what the user has said. When a text object is received, we will send it off to the Tone Analyzer service to process the tone of the user.
  * **OnLearningIntent()**: Updates the EmotionalState variable when Positive or Negative feedback have been posted to the blackbaord. An example of positive feedback is when the user says something like "good job!" while negative feedback could be "Bad job!". Initially, the EmotionalState is 0.5 and must be 0 - 1. Each time the agent receives a piece of positive or negative feedback, the OnLearningIntent() function increases for positive feedback or decreases for negative feedback the EmotionalState variable score by 0.1. 
  * **OnTone()**: The callback when we receive the results from the Tone Analyzer service. We define positve and negative tone from the body, or can be declared when we deserialize the class. If positive tone is identified, then we will increment our emotional state, while if tone is considiered negative, then we will decrement it.
  * **OnEmotionCheck()**: Restores the EmotionalState to a basel level of 0.5. For every 30 seconds the OnEmotionCheck() increases when EmotionalState is less than 0.5 and decreases when EmotionalState is more than 0.5 the EmotionalState variable. This ensures that the EmotionalState will trend back to neutral over time. 
  * **PublishEmotionalState()**: Formats the current EmotionalState value, formats it into the json value, and adds it to the blackboard. Currently, the TextClassifier is subscribed to listen for EmotionalState blackboard objects. The state will be added as pre-context that can then be useful when modifying conversation (see Workshop 2 on how to update the conversational service).

**Windows users do the following step.**

1. Right-click on the project, go to **Properties**, and make the following changes:

* Go to **C/C++->General->Additional Include Directories->**, and add `..\..\examples\workshop_three;..\..\include\self;..\..\include\wdc;..\..\lib\boost_1_60_0;..\..\lib;%(AdditionalIncludeDirectories)`.
* Go to **C/C++->Precompiled Headers->Confirm Precompile Header**, and delete the value. Make sure it's blank.

**OS X and Windows do the following two steps.**
  
The Serialize, Deserialize, OnStart, OnStop, OnEmotion, OnLearningIntent, OnEmotionCheck, and PublishEmotionalState functions are already completely built out.

In the next step, you build the OnText and OnTone function bodies yourself.

1. Write the OnText and OnTone function bodies.
  1. For OnText(), copy the following code and paste it into the function body {}:
  ```
  Text::SP spText = DynamicCast<Text>(a_ThingEvent.GetIThing());
    if (spText)
    {
        ToneAnalyzer * tone = SelfInstance::GetInstance()->FindService<ToneAnalyzer>();
        if (tone != NULL)
        {
            tone->GetTone(spText->GetText(), DELEGATE(WorkshopThreeAgent, OnTone, DocumentTones *, this));
        }
    }
  ```
      This code implements the logic for what to do when a Text object is placed on the blackboard. First, we cast the ThingEvent object to a Text object. All Blackboard objects inherit from the IThing interface. If we are successful in casting to a Text object, we will invoke the Tone Analysis service, which can be read from the SelfInstance class. If the service is not null (i.e., it has been declared in the body.json file), then we will send a REST request to Tone Analysis and provide the callback to be the OnTone function.
  2. For OnTone(), copy the following code and paste it into the function body:
  ```
      if (a_Callback != NULL)
    {
        double topScore = 0.0;
        Tone tone;
        for (size_t i = 0; i < a_Callback->m_ToneCategories.size(); ++i)
        {
            for (size_t j = 0; j < a_Callback->m_ToneCategories[i].m_Tones.size(); ++j)
            {
                Tone someTone = a_Callback->m_ToneCategories[i].m_Tones[j];
                if (someTone.m_Score > topScore)
                {
                    topScore = someTone.m_Score;
                    tone = someTone;
                }
            }
        }
        Log::Debug("WorkshopThreeAgent", "Found top tone as: %s", tone.m_ToneName.c_str());
        bool toneFound = false;
        for (size_t i = 0; i < m_PositiveTones.size(); ++i)
        {
            if (tone.m_ToneId == m_PositiveTones[i])
            {
                toneFound = true;
                if (m_EmotionalState < 1.0f)
                    m_EmotionalState += 0.1f;
            }
        }

        if (!toneFound)
        {
            for (size_t i = 0; i < m_NegativeTones.size(); ++i)
            {
                if (tone.m_ToneId == m_NegativeTones[i])
                {
                    if (m_EmotionalState > 0.0f)
                        m_EmotionalState -= 0.1f;
                }
            }
        }

        PublishEmotionalState();
    }
  ```
First, this code iterates over the response to find the emotion that has the highest probability. Then, it check whether the emotion is positive, and, if it is, the EmotionalState variable is incremented by 0.1. The EmotionalState variable cannot exceed one. If the highest probability tone is negative, the EmotionalState variable is decreased by 0.1. The EmotionalState variable cannot be less than zero.

2. Rebuild this project in the SDK.

**Congratulations!** You just built all the functions required for the emotion agent. This process created the `libworkshop_three_plugin.dylib` in the `bin` directory for your platform in the `self-sdk-develop` directory. If you're using OS X, the path is `Self/self-sdk-develop/bin/mac`.

In the next task, you update the `body.json` file to include the new plugin so that Intu can use it.

## Retrieving embodiment credentials for you organisation. 
To get the embodiment credentials for your organisation, without closing the terminal you are in, go back to the Robot Gateway: https://rg-gateway01.mybluemix.net/Downloads 

On the left hand side of the page, locate and click on VIEW CREDENTIALS.

This should direct to a new frame on the right hand side of the page. Under Filter By: find the organisation you created in Workshop 1 and find the group you created in the scroll down. 

Now click "Get Credentials" and then copy the credentials by clicking the copy button. 

Now go back to the terminal you had the Intu directory.

## Configuring your Intu instance to include the emotion agent

1. Navigate to the **self**->**etc**->**profile** directory, and open the `body.json` file (For Windows, this will be in the sdk/bin/Debug).
2. Locate the `m_Libs` variable.
  * If you're using OS X, the variable is `"m_Libs" : [ "platform_mac" ],`
  * If you're using Windows, the variable is `"m_Libs" : [ "platform_win" ],`
3. Add the information for the new plugin to the end of the `m_Libs` variable for your platform:
  * If you're using OS X, the variable is `"m_Libs" : [ "platform_mac", "workshop_three_plugin"],`
  * If you're using Windows, the variable is `"m_Libs" : [ "platform_win", "workshop_three_plugin"],`
4. Locate `EmotionAgent` in the `body.json` file, and notice the `m_NegativeTones` and `m_PositiveTones` strings. To understand the tone of the input, these strings are compared to OnTone().
5. Change EmotionAgent to WorkshopThreeAgent or the name you gave your class. The instructions use WorkshopThreeAgent, so the Type_ field is `"Type_" : "WorkshopThreeAgent"`.
6. Now find the `"m_EmbodimentCreds":{ ... }`, and replace this with the `"m_EmbodimentCreds":{ ... }` copied from the robot gateway.
7. Save your changes.
8. Return to the directory for you platform in the `/bin` directory, and run one of the following commands:
  * If you're using OS X, run `pwd`.
  * If you're using Windows, run `cd`.
9. Run the following commands:
  * `export LD_LIBRARY_PATH={$HOME}/Self/self-sdk-develop/bin/mac`
  * `export LD_LIBRARY_PATH=*the path returned in Step 7*`
10. ** For OSX** In the `mac` directory, run Self by issuing the following command: `./self_instance -c 0 -f 0`.
11. ** For Windows**  Run Self by clicking Local Windows Debugger in Visual Studio

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
  4. Click **View Credentials** for the new service instance, and keep this window open.
  5. Click the **Watson** breadcrumb. The list of your service instances is displayed.
  6. Add the next service instance by clicking the hexagonal **+** button. The Watson service catalog is displayed.
5. Create instances of the Natural Language Classifier, Speech to Text, and Text to Speech services by repeating the same steps 1 - 7 that you completed to create the Conversation service instance.

### Configuring Intu to use your service instances

Your installation is preconfigured to use the Conversation, Natural Language Classifier, Speech to Text, and Text to Speech services. To configure Intu to use your instances of these services, complete the following steps:

1. Expand **All Organizations** by clicking the arrow icon.
2. Click the name of your organization.
3. Expand your organization by clicking the arrow icon.
4. Click the name of your group.
5. Click **Services** in the navigation bar.
6. For your instances of the Conversation service, Natural Language Classifier, Speech to Text, and Text to Speech services, click **Edit**, specify the user ID and password, and click **Save**.

**Important:** Do not change the service endpoint unless you are an enterprise user.
