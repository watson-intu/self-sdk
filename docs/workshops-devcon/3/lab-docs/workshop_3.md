# Workshop 3: Creating an emotion agent

In this workshop, you create an emotion agent. Agents make decisions about how Intu operates and responds. The emotion agent uses the Tone Analyzer service on Bluemix, which analyzes a person's tone and determines whether that tone is positive or negative.

**Before you begin:** You must have a Mac or Windows laptop, and you must have completed Workshop 1: Say Hello!. You will notice that Intu and Self are used interchangeably. Self is the technical name for Intu.

**Note for Windows users:** You will need to have [**Visual Studio Community**](https://www.visualstudio.com/vs/community/) installed. This will take some time to download.

Complete the following tasks:

1. [Setting up the Tone Analyzer service](#setting-up-the-tone-analyzer-service)
2. [Understanding some Intu terminology](#understanding-some-intu-terminology)
3. [Building the Self SDK](#building-the-intu-sdk)
4. [Creating an emotion agent](#creating-an-emotion-agent)
5. [Configuring Intu to include your emotion agent](#configuring-intu-to-include-your-emotion-agent)

## 1. Setting up the Tone Analyzer service

### A. Creating your own instance of the Tone Analyzer service

1. [Log in to Bluemix](https://console.ng.bluemix.net/) if you are not already logged in.

2. On the Bluemix dashboard, click **Catalog** in the top-right navigation bar.

3. In the All Categories menu on the left, under Services, click **Watson**.

4. Click the **Tone Analyzer** tile.
  1. Keep all the default values, and click **Create**.
  2. Click the **Service Credentials** tab.
  3. Click **View Credentials** for the new Tone Analyzer service instance you just created. 
  4. Copy these credentials (everything inside **{ }**) and paste them into a new text file in your favourite text editor. You will need these credentials to configure your Tone Analyzer service in the section below.

### B. Configure Intu to use your Tone Analyzer service instance

1. Open a new browser window and [log in to the Intu Gateway](https://rg-gateway.mybluemix.net/).

2. Click on **MANAGE** on the left hand side navigation bar, and select **Services**. Select your Organization and Group in the top Filter by menu

3. Click the **Add Service** box.
  
4. In the **SERVICE NAME** field, specify **ToneAnalyzerV1**.

5. In the **USER ID** field, copy in the username you pasted into your text file in the above.

6. In the **PASSWORD** field, copy in the password.

7. In the **SERVICE ENDPOINT** field, copy in the url.

8. Click the **Add** button at the bottom right of the window.

## 2. Understanding some Intu terminology

Before you create an emotion agent, become familiar with the following terminology:

  * **Blackboard**: The central message broker on which all the agents post data and listen for incoming data.
  
  * **Publish**: To push data onto the Blackboard under a particular topic. 
  
  * **Subscribe**: Subscribing to a topic on the Blackboard means to listen to the Blackboard and wait for any other agents to post to the Blackboard under a particular topic.
  
  * **Topic**: A channel to which Agents publish and subscribe.

## 3. Building the Self SDK

Follow the instructions for your platform.

**Before you begin**:

1. [Download the Self SDK](https://hub.jazz.net/project/wlabs/self-sdk). Click on the **download icon** next to the default **master** branch selected.

2. Create a new directory named **intu** in a directory of your choosing.

3. Unzip the **wlabs_self-sdk-master.zip** file into the **intu** directory, making sure that you retain the folder structure, i.e. your intu directory should now contain the unzipped **wlabs_self-sdk-master** folder. This may take some time.

### A. Preparing for OS X

**Note**: If you have already completed **Preparing for OS X** in another workshop, you can skip this section.

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
   3. Specify your password, and press **Return** or **Enter**. If you have **macOS Sierra**, the following message is displayed:
   
   		```
   		HEAD is now at 89fd34b Merge pull request #1368 from MikeMcQuaid/build-options-file
   		Error: /usr/local/Cellar is not writable. You should change the
   		ownership and permissions of /usr/local/Cellar back to your
   		user account:
   		sudo chown -R $(whoami) /usr/local/Cellar
   		Failed during: /usr/local/bin/brew update --force
   		```
   
   		Run: `sudo chown -R $(whoami) /usr/local/Cellar`
   4. Now repeat step 1 by running: 
    ```
   ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
   ```
2. Install CMake using Homebrew.
	1. Run the command: `sudo chown -R $(whoami) /usr/local`   
	2. Now run: `brew install cmake`

		**Note**: If you have **macOS Sierra**, you may see the following message if you happen to have an outdated version of **Xcode**: 
	
		```
	Your Xcode (7.3.1) is outdated.
	Please update to Xcode 8.1 (or delete it).
	Xcode can be updated from the App Store.
		```
	
		If you see this message, you will need to update **Xcode**.
	
3. Download **Anaconda 4.2.0 Python 2.7 version** by using the **Graphical Installer**. It is required to correctly configure pip in the following step.
   1. Open a new browser window and [download Anaconda 4.2.0 Python 2.7 version](https://www.continuum.io/downloads).
   2. Click the solid blue GRAPHICAL INSTALLER button for Python 2.7 Version. It should be 403 MB. The .pkg file downloads.
   3. After the file is downloaded, double-click it, and follow the prompts to install Anaconda.
   4. Open a **new** Terminal window and make sure your version of Python has been successfully updated by running the command: `python --version` 
 	
 		You should see: `Python 2.7.12 :: Anaconda 4.2.0 (x86_64)`
  
4. Set up qiBuild.
   1. Run: `pip install qibuild`
   2. Run: `qibuild config --wizard` and be sure to include the two hyphens before **wizard** in the last command.
   3. When the "Please choose a generator" prompt is displayed, specify **1**, and press **Enter**.
   4. When the "Please choose an IDE" prompt is displayed, specify **1**, and press **Enter**.

### B. Building the Self SDK for OS X

1. In a new Terminal window, navigate to the **wlabs_self-sdk-master** directory inside **intu**. You should just be able to run: `cd intu/wlabs_self-sdk-master` 

	**Note:** If you have **already built the Self SDK for OS X** in another workshop, run: `./scripts/clean.sh`    

2. Run:
`./scripts/build_mac.sh`  
If you are running this script for the first time and you see the following message, do not worry as you do not have the toolchain. Let the script run and proceed with the next step.
```
[ERROR]: Exception No such toolchain: mac-self
Known toolchains are:
```

### C. Building the Self SDK for Windows

1. Preparing Windows only requires installing **[Visual Studio Community](https://www.visualstudio.com/vs/community/)**. **Note:** This will take some time to download.

2. **Important:** When you install Visual Studio, in **Features**, inside the **Select features** box, make sure that **Visual C++** is selected under **Programming Languages**. 

3. Visual Studio should launch automatically after installing. If it does not, open Visual Studio.

4. Select **Open solution**.

5. Open the solution found in **vs2015/self-sdk.sln**.

6. Right click on the **self-sdk** project and select **Set as Startup Project**.

7. Right click on the **self-sdk** project, and open **properties**. In the **Debugging tab** of the properties, you will need to change **Working Directory** to **$(TargetDir)**.

8. The first time you open your Visual Studio project, Visual Studio will want to compile for the **x86** architecture. In order to build Self, you will need to change this to **Win32**. Next to **Debug** in the task menu bar (directly below **File**), select the dropdown arrow and change **x86** to **Win32**.

9. Select **Build -> Build Solution**.

10. Select **Debug -> Start Debugging** to run the project with debugging.

**Important:** If you use SourceTree, the process might get stuck when trying to pull by using SSH. This is because SourceTree will try to launch an interactive window that will be hidden. Run the following commands on the command line to fix the problem:

1. `cd "C:\Program Files (x86)\Atlassian\SourceTree\tools\putty"`
	
2. `plink git@github.ibm.com`

## 4. Creating an emotion agent

### A. Preparing your directories and files for the emotion agent

**For OS X users:**

1. If you do not have it installed already, download the trial version of the [CLion C++ IDE](https://www.jetbrains.com/clion/download/).

2. In **CLion**, select **Open** -> **your home directory** -> **intu** -> **wlabs_self-sdk-master** and click **OK**. 

	Note that a window may appear prompting you to open your project in a New Window or This Window. Select **New Window**. At the bottom of your CLion window, in the Problems tab, you will see the following Error, which you do not need to worry about:

	Error: By not providing "FindSELF.cmake" in CMAKE_MODULE_PATH this project has asked CMake to find a package configuration file provided by "SELF", but CMake did not find one.
Could not find a package configuration file provided by "SELF" with any of the following names:
  SELFConfig.cmake   self-config.cmake
Add the installation prefix of "SELF" to CMAKE_PREFIX_PATH or set "SELF_DIR" to a directory containing one of the above files.  If "SELF" provides a separate development package or SDK, be sure it has been installed.

	2i. Inside the CLion **wlabs_self-sdk-master project**, right-click **examples**, select **New**, and select **Directory**. Type in **workshop_three** for the new directory name, and click **OK**.
 
	2ii. Right-click the `CMakeLists.txt` file in the **examples** directory, and click **Copy**. (If you are unsure of the directory you are in, look in the top-left navigation bar.)
  
	2iii. Right-click the **workshop_three** directory, and click **Paste**. This file helps to build the plugin for the emotion agent.

	2iv. Return to the **examples** directory, open the `CMakeLists.txt` file, and add the following line: `add_subdirectory(workshop_three)` at the end. Your file contains the following three lines:

  ```
    include_directories(".")

    add_subdirectory(sensor)
    add_subdirectory(workshop_three)
  ```
	2v. Open the `CMakeLists.txt` file in the **workshop_three** directory, and overwrite its content with this code:

  ```
    include_directories(.)

    file(GLOB_RECURSE SELF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
    qi_create_lib(workshop_three_plugin SHARED ${SELF_CPP})
    qi_use_lib(workshop_three_plugin self wdc)
    qi_stage_lib(workshop_three_plugin)
  ```

3. Create a new directory inside this called **agents** in the **workshop_three** directory.

4. Locate the Workshop 3 code snippet files **to be filled in** in:

 `wlabs_self-sdk-master/docs/workshops-devcon/3/code-snippets/WorkshopThreeAgent_start`

5. Copy the `WorkshopThreeAgent.cpp` and the `WorkshopThreeAgent.h` files and paste them into the **agents** directory that you created.

**For Windows users:**

1. Open up File Explorer and navigate to **intu/wlabs_self-sdk-master/examples**.

2. Create a new directory in examples called **workshop_three**. 

3. Copy across `WorkshopThreeAgent.cpp` and `WorkshopThreeAgent.h` located under **self-sdk/docs/workshops-devcon/3/code-snippets/WorkshopThreeAgent_Start** into the **workshop_three** directory.

4. In **Visual Studio**, in the **examples** directory, add a new **Win32 Project** called `workshop_three_plugin`, and click **OK**.

5. Click **Next**, select **Application Type as DLL**, and uncheck **Precompiled header and Security Development Lifecycle (SDL) checks** under **Additional options**.

6. Click **Finish**.

7. Inside of `workshop_three_plugin`, remove the **Header Files**, **Resource Files**, and **Source Files** directories that were newly created in the solution.

8. Inside the **Solution Explorer** window, right click **workshop_three****_plugin**, and select **Add -> New Filter**.

9. Name the filter **agent**.

10. Right-click on **agent**, and select **Add -> Existing Items**.

11. Navigate to **wlabs_self-sdk-master/examples/workshop_three** and select the files: `WorkshopThreeAgent.cpp` and `WorkshopThreeAgent.h`.

12. Right-click the **workshop_three_plugin** solution, open **Properties**, and make the following changes, but **before you begin, make sure Configuration at the top left is set to "All Configurations"**.

* Change the value of **General -> Character Set** to **Use Multi-Byte Character Set**.

* Go to **C/C++ -> General** **-> Additional Include Directories** **->**, and add: `..\..\examples\workshop_three;..\..\include\self;..\..\include\wdc;..\..\lib\boost_1_60_0;..\..\lib;%(AdditionalIncludeDirectories)`

* Go to **C/C++ -> Precompiled Headers -> Confirm Precompile Header**, and delete the value. Make sure it's blank.

* Go to **Linker -> General -> Additional Library Directories ->**, and add: `../../lib/$(Configuration);../../lib/boost_1_60_0/stage/lib/`

* Replace the value of **Linker -> Input -> Additional Dependencies** with the following value: `jsoncpp.lib;self.lib;wdc.lib;%(AdditionalDependencies)`

* Go to **Build Events -> Post-Build Event -> Command Line**, and add: `copy /Y "$(TargetPath)" "$(ProjectDir)..\..\bin\$(Configuration)"`


### B. Building out the OnText, OnTone and OnLearningIntent functions for your emotion agent

Open the `WorkshopThreeAgent.cpp` file, which contains the following functions that enable the emotion agent you'll create:

  * **Serialize()**: All classes in Intu inherit from our base interface ISerializable. We have the ability to serialize any object back into json format, which usually will get stored in the body.json (found in same directory as self_instance).

  * **Deserialize()**: All classes in Intu inherit from our base interface ISerializable. We have the ability to deserialize any object from json into memory, which is usually from the body.json (found in the same directory as self_instance).

  * **OnStart()**: Starts the agent. The OnStart() function is called from the AgentSociety, which will start and stop all agents on start up and shut down. Since the OnStart()
  function is called on the main thread, we want to do as little processing as possible. For agents, we want to subscribe to objects that are placed on the Blackboard, start
  timers, or potentially instantiate a service. To subscribe to a Blackboard object, look at the BlackBoard.h file for SubscribeToType function. There we can specify the type of the object we want to subscribe to, our callback information, and a macro stating what type of event we are interested in. For example, TE_ADDED specifies to let the agent know when the Blackboard object has been added to the Blackboard, while TE_STATE specifies to let the agent know when the Blackboard object state has changed (i.e., from processing to finished).
  
  * **OnStop()**: Stops the agent. The OnStop() function is called from the AgentSociety, which will start and stop all agents on start up and shut down. The OnStop() function is called on the main thread, so it is important that this call blocks and waits for all lingering processes to finish in the agent. Here, we typically will unsubscribe from Blackboard objects and make sure to reset any timers that may be currently active.
  
  * **OnEmotion()**: Is our callback function when an emotion object was placed on the Blackboard. All Blackboard objects inherit from the IThing interface. That means when we receive a ThingEvent, we grab the IThing object and dynamically cast it to the object we believe we have. Always make sure to check the object is not null before proceeding with your logic.
  
  * **OnText()**: Is our callback function when a Text object is placed on the Blackboard. Text objects typically are created from the TextExtractor class, which subscribes to Audio sensors, takes in raw audio, sends it to the STT service, and instantiates a Text object with the final transcription. Here, we are interested in text objects so we can determine the tone of the user. Once a text object is received, then we want to send the transcript to the Tone Analyzer service.
  
  * **OnLearningIntent()**: This is our callback for when we receive a learning intent. A learning intent could be many things, but for this particular case we are only interested in positive and negative feedback. An example of each could be "Good Job!"/"Bad Job!" respectively. Initially, the EmotionalState is 0.5 and must be 0 - 1. Each time the agent receives a piece of positive or negative feedback, the OnLearningIntent() function increases for positive feedback or decreases for negative feedback the EmotionalState variable score by 0.1. 
  
  * **OnTone()**: This is our callback for when we receive data from the Tone Analyzer service. Depending on the response, we iterate through pre-determined vectors of positive and negative moods. If there is a match with a positive mood (i.e., "Joy"), then we will increment the emotional state, while if there is a negative tone (i.e., "Disgust") then we will decrement the emotional state.
  
  * **OnEmotionCheck()**: Restores the EmotionalState to a basal level of 0.5. For every 30 seconds when the EmotionalState is less than 0.5, the OnEmotionCheck() increases by 0.1. Similarly, for every 30 seconds when the EmotionalState is more than 0.5, the OnEmotionCheck() decreases by 0.1. This ensures that the EmotionalState will trend back to neutral over time.

  * **PublishEmotionalState()**: Formats the current EmotionalState value, formats it into the json value, and adds it to the Blackboard.

  
The Serialize, Deserialize, OnStart, OnStop, OnEmotion, OnLearningIntent, OnEmotionCheck, and PublishEmotionalState functions are already completely built out.

In the next step, you will build out the **OnText**, **OnTone** and **OnLearningIntent** functions using the example code provided.

**For OS X and Windows users:**

1. In **wlabs_self-sdk-master****/docs/workshops-devcon/3/code-snippets/WorkshopThree_Snippets**, you will see the `WorkshopThreeCodeSnippets.txt` file. Open this file and find the OnText, OnTone and OnLearningIntent functions.

2. From `WorkshopThreeCodeSnippets.txt`, for the **OnText()** function, copy the code directly below **//Code for OnText()**. Now paste this inside the function body **{}** of **OnText()** in `WorkshopThreeAgent.cpp`, which is located inside your **agents** directory. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.
  
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
            
3. For the **OnTone()** function, copy the code directly below **//Code for OnTone()**. Now paste this inside the function body **{}** of **OnTone()** in `WorkshopThreeAgent.cpp`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.
  
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
  
4. For the **OnLearningIntent()** function, copy the code directly below **//Code for OnLearningIntent()**. Now paste this inside the function body **{}** of **OnLearningIntent()** in `WorkshopThreeAgent.cpp`. The code which you need is displayed below for completeness; however, it is **not** recommended for you to copy it from here due to formatting issues.  
  
 ```

  LearningIntent::SP spLearningIntent = DynamicCast<LearningIntent>(a_ThingEvent.GetIThing());
    if (spLearningIntent && spLearningIntent->GetVerb().compare("feedback") == 0)
    {
        if (spLearningIntent->GetTarget().compare("positive_feedback") == 0)
        {
            if(m_EmotionalState < 1.0)
                m_EmotionalState += 0.1f;
        }
        else
        {
            if (m_EmotionalState > 0.0)
                m_EmotionalState -= 0.1f;
        }

        PublishEmotionalState();
    }
  
	```
  
First, this code iterates over the response to find the emotion that has the highest probability. Then, it checks whether the emotion is positive, and, if it is, the EmotionalState variable is incremented by 0.1. The EmotionalState variable cannot exceed one. If the highest probability tone is negative, the EmotionalState variable is decreased by 0.1. The EmotionalState variable cannot be less than zero.


**Additional steps for OS X users:**

1. Open a **new** Terminal window and navigate to **intu/wlabs_self-sdk-master** by running: `cd intu/wlabs_self-sdk-master`.

	**Note:** If you have built the SDK in a previous workshop, make sure you run: `./scripts/clean.sh`.

2. Run the build script: `./scripts/build_mac.sh`. **Do not close this Terminal window.**

**Congratulations!** You just built all the functions required for the emotion agent. This process created the `libworkshop_three_plugin.dylib` in the **intu/wlabs_self-sdk-master/bin/mac** directory.

In the next task, you will update the `body.json` file also located in the **intu/wlabs_self-sdk-master/bin/mac/etc/profile** directory to include the new plugin so that Intu can use it.


## 4. Configuring your Intu instance to include the emotion agent

### A. Retrieving the credentials for your Organization in the Intu Gateway
1. [Log in to the Intu Gateway](https://rg-gateway.mybluemix.net/). 

2. Click on **VIEW CREDENTIALS** in the left hand navigation bar.

3. Select your Organization and Group in the top Filter by menu, and click on the **Get Credentials** box.
4. Copy these credentials by clicking the **Copy** icon in the top right of the window, and paste this into a new text file using your favourite text editor.

### B. Configuring your `body.json` file

1. Open your `body.json` file. 

	* For **OS X**, this will be in **wlabs_self-sdk-master/bin/mac/etc/profile**.
	* For **Windows**, in **Visual Studio**, in the **Solution Explorer**, go to **sdk -> bin -> Debug**. 
	
2. Locate the `m_Libs` variable.
  * If you're using **OS X**, the variable is `"m_Libs" : [ "platform_mac" ],`
  * If you're using **Windows**, the variable is `"m_Libs" : [ "platform_win" ],`
3. Add **workshop****_three****_plugin** to the end of the `m_Libs` variable for your platform, **as shown below**:
  * If you're using **OS X**, the variable is `"m_Libs" : [ "platform_mac", "workshop_three_plugin"],`
  * If you're using **Windows**, the variable is `"m_Libs" : [ "platform_win", "workshop_three_plugin"],`
4. Locate `EmotionAgent` in the `body.json` file, and notice the `m_NegativeTones` and `m_PositiveTones` strings. To understand the tone of the input, these strings are compared to OnTone().

5. Change `EmotionAgent` to `WorkshopThreeAgent` or the name you gave your class. As the instructions used `WorkshopThreeAgent`, the `"Type_"` field becomes `"Type_" : "WorkshopThreeAgent"`.
6. Now find `"m_EmbodimentCreds":{ ... }` in your `body.json` file. Replace this with the complete set of credentials you copied over into your text editor from the Intu Gateway in step 4 of the previous section.
7. Save your changes.

### 3. Building Intu

**For OS X users:**

1. Return to your most recent Terminal window, where you should already be in the **mac** directory. Otherwise, open a **new** Terminal window and navigate to this directory by running: `cd intu/wlabs_self-sdk-master/bin/mac`

2. Run: `export LD_LIBRARY_PATH=./`
3. Run Intu by issuing the following command: `./self_instance`

**For Windows users:**

1. From the Visual Studio Menu, select **Build -> Build Solution**.

2. Run Intu by clicking **Local Windows Debugger** in Visual Studio.

Now that you have added an Emotion Agent, Intu will start to adapt to you. First, ask Intu “How are you?”, and listen to the response. Now feed Intu some positive emotion statements like "Good job!”, and then ask “How are you?” again. Intu should now give a "happier" response than the one it gave before. Try the same thing for some negative emotion statements. Say “Wrong answer" a number of times and then ask “How are you?”. Intu should respond with a "sadder" response.

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
