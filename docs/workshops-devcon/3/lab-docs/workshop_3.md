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

1. Open the `self-sdk-develop` file as a project in the IDE for your platform.
2. Now, let's create and populate directory specifically for this workshop.
** For OSX
  1. Locate the `examples` directory under the `self-sdk-develop` project that you opened. This directory contains a `sensor` directory and a `CMakeLists.txt` file.
  2. Right-click the `examples` directory, and click **New**->**Directory**. Name it `workshop_three`. Your new directory is created.
  3. Right-click the `CMakeLists.txt` file in the `examples` directory, and click **Copy**.
  4. Right-click the `workshop_three` directory, and click **Paste**. This file helps to build the plugin for the emotion agent.
  5. Return to the `examples` directory, open the `CMakeLists.txt` file, and add the following line: `add_subdirectory(workshop_three)` at the end. Your file contains the following three lines:
  ```
    include_directories(".")

    add_subdirectory(sensor)
    add_subdirectory(workshop_three)
  ```
  5. Open the `CMakeLists.txt` file in the `workshop_three` directory, and overwrite its content with this code:
  
  ```
    include_directories(.)

    file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
    qi_create_lib(workshop_three_plugin SHARED ${SELF_CPP})
    qi_use_lib(workshop_three_plugin self wdc)
    qi_stage_lib(workshop_three_plugin)
  ```
3. Navigate back to the `workshop_three` directory, create a new directory, and name it `agents`.
4. Locate the Workshop 3 code snippet files in `self-sdk-develop/docs/workshops-devcon/3/code-snippets/WorkshopThreeAgent_start`. Copy the `WorkshopThreeAgent.cpp` and the `WorkshopThreeAgent.h` files, and paste them into the `agents` directory that you created.
5. Open the `WorkshopThreeAgent.cpp` file, which contains the following functions that enable the emotion agent you'll create:

  * **OnStart()**: Initializes for the emotion agent. It subscribes the emotion agent to the blackboard. After initialization is complete, the emotion agent subscribes to OnEmotion, OnLearningIntent, and OnEmotionCheck functions. 
  * **OnStop()**: Stops the emotion agent. After the emotion agent is called, it is no longer subscribed to the blackboard.
  * **OnEmotion()**: Listens to the blackboard for emotion topics being posted to the blackboard. 
  * **OnText()**: Waits for and receives responses from the Tone Analyzer service.
  * **OnLearningIntent()**: Updates the EmotionalState variable. Initially, the EmotionalState is 0.5 and must be 0 - 1. Each time the agent receives a piece of positive or negative feedback, the OnLearningIntent() function increases for positive feedback or decreases for negative feedback the EmotionalState variable score by 0.1. 
  * **OnTone()**:
  * **OnEmotionCheck()**: Restores the EmotionalState to a basel level of 0.5. For every 30 seconds the OnEmotionCheck() increases when EmotionalState is less than 0.5 and decreases when EmotionalState is more than 0.5 the EmotionalState variable. This ensures that the EmotionalState will trend back to neutral over time. 
  * **PublishEmotionalState()**: Formats the current EmotionalState value, formats it into the json value, and adds it to the blackboard.

** IF ON WINDOWS in Visual Studio:
  Right click on solution and go to Properties and make the following changes:
  C/C++ -> General -> Additional Include Directories -> add: ..\..\examples\workshop_three;..\..\include\self;..\..\include\wdc;..\..\lib\boost_1_60_0;..\..\lib;%(AdditionalIncludeDirectories)
  C/C++ -> Precompiled Headers -> Confirm Precompile Header is left blank
  
The Serialize, Deserialize, OnStart, OnStop, OnEmotion, OnLearningIntent, OnEmotionCheck, and PublishEmotionalState functions are already completely built out.

In the next step, you build the OnText and OnTone function bodies yourself.

6. Write the OnText and OnTone function bodies.
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
      This code accepts the piece of text that the emotion agent is listening for. Then, it finds the Tone Analyzer service and sends the text.
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

7. Rebuild this project in the SDK.

**Congratulations!** You just built all the functions required for the emotion agent. This process created the `libworkshop_three_plugin.dylib` in the `bin` directory for your platform in the `self-sdk-develop` directory. If you're using OS X, the path is `Self/self-sdk-develop/bin/mac`.

In the next task, you update the `body.json` file to include the new plugin so that Intu can use it.

## Configuring Intu to include your emotion agent

1. Navigate to the `self/self-sdk-develop/bin/mac/etc`  directory, and open the `body.json` file.
2. Change EmotionAgent to WorkshopThreeAgent or the name you gave your class. The instructions use WorkshopThreeAgent, so the Type_ field should be changed to `"Type_" : "WorkshopThreeAgent"`.
3. Save your changes to the `body.json` file.
4. In a new terminal window, navigate to the `self/self-sdk-develop/bin/mac` directory. You can run `cd self/self-sdk-develop/bin/mac` to get there.
4. Keep the Intu terminal open, and return to the Gateway. 
7. On the left side of the page, click **View Credentials**. A new frame on the right side of the page is displayed.
8. In the dropdown menus in the **Filter By** section, select the organization and group that you created in Workshop 1.
9. Click **Get Credentials** and copy the this "m_EmbodimentCreds":

 Now go back to the body.json file and locate 

the "m_EmbodimentCreds" field. Delete the this "m_EmbodimentCreds" field and paste the "m_EmodimentCreds" copied from the RG (robot gateway).

6. Save your changes to the `body.json` file.
7. Return to the directory for you platform in the `/bin` directory, and run one of the following commands:
  * If you're using OS X, run `pwd`.
  * If you're using Windows, run `cd`.
8. Run the following commands:
  * `export LD_LIBRARY_PATH={$HOME}/Self/self-sdk-develop/bin/mac`
  * `export LD_LIBRARY_PATH=*the path returned in Step 7*`
9. ** For OSX** In the `mac` directory, run Self by issuing the following command: `./self_instance -c 0 -f 0`.
10. ** For Windows**  Run Self by clicking Local Windows Debugger in Visual Studio

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
