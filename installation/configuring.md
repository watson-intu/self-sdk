# Configuring Intu
Your instance of Intu is pre-configured with the following Watson services from <a href="https://console.ng.bluemix.net" target="_blank">Bluemix</a>: Conversation, Weather Company Data, Speech to Text, and Text to Speech. The pre-configuration is enabled for 30 days. If you want to continue to use Intu after 30 days, you must create your own instances of these services and configure Intu to use them.

**This page will walk you through the following:**

1. [Creating and configuring the necessary services for Intu](#required-services)
2. [What extra functionality is available if you create and configure additional services](#additional-functionality)
3. [Creating and configuring optional services for Intu](#creating-additional-services)

## A. <a name="required-services">Creating required instances of Watson services</a>
In order to use Intu, at minimum you need operational instances of the following services in Bluemix: <a href="https://console.ng.bluemix.net/catalog/services/conversation" target="_blank">Conversation</a>, <a href="https://console.ng.bluemix.net/catalog/services/speech-to-text" target="_blank">Speech to Text</a>, and <a href="https://console.ng.bluemix.net/catalog/services/text-to-speech" target="_blank">Text to Speech</a>.

Note: While Weather Company Data is one of the 30 day trial pre-configured services, it is not required to use Intu. It does, however, offer additional functionality ([as described below](#additional-functionality)).

###To create and configure an instance of Watson <u>Conversation</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/conversation" target="_blank">here</a> to be taken to the **Conversation** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **username** and **password** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the Edit button (pencil icon) for **ConversationV1**.
	6. Paste your username and password into the **USER ID** and **PASSWORD** fields.
	7. Click **SAVE**

*Note for Conversation service only*: In addition to configuring your username and password for this service, you also need to update the Workspace IDs.

The **Intu Starter Kit** contains a Conversation service workspace that helps you visualize how intents, entities, and dialog are developed. You can expand on the workspace in the kit or use it as a guide for developing your own later.

1. Log in to Intu Gateway.
2. Click **Downloads**.
3. Download the Intu Starter Kit.
4. Complete the instructions in `readme.txt`.


###To create and configure an instance of Watson <u>Speech to Text</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/speech-to-text" target="_blank">here</a> to be taken to the **Speech to Text** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **username** and **password** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the Edit button (pencil icon) for **SpeechToTextV1**.
	6. Paste your username and password into the **USER ID** and **PASSWORD** fields.
	7. Click **SAVE**

###To create and configure an instance of Watson <u>Text to Speech</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/text-to-speech" target="_blank">here</a> to be taken to the **Text to Speech** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **username** and **password** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the Edit button (pencil icon) for **TextToSpeechV1**.
	6. Paste your username and password into the **USER ID** and **PASSWORD** fields.
	7. Click **SAVE**


## B. <a name="additional-functionality">Additional functionality that can be used with Intu</a>
While only Conversation, Speech to Text, and Text to Speech are required for running Intu, you can add more Bluemix services for even more usability.  In the section below you can read about additional Intu features that can be used if you have the necessary Bluemix services configured in your Intu Gateway.

Note: For the features listed below to work, you need to use the Conversation workspace downloaded from the **Intu Starter Kit** located on the <a href="https://rg-gateway.mybluemix.net/Downloads" target="_blank">Intu Gateway Downloads</a> page. You can use this workspace in addition to any other ones you would like to use.

1. **Greeter Agent**: Intu will greet you!  For this to work you need to have your camera turned on. If you are running Intu on a Mac, Windows, or Nao robot device and are using the Intu Starter Kit Conversation workspace, simply saying _"turn the camera on"_ will turn your camera on.

	* Additional Bluemix services required: **Visual Recognition**
	
	Note: For a more personalized greeting, see **Name Agent** below.
	
2. **Weather Agent**: Get Intu to tell you about the weather! Examples of weather-related questions you can ask: _"What is the weather in Boston"_ or _"What will the weather be like in Las Vegas on Friday"_.

	* Additional Bluemix services required: **Natural Language Understanding** and **Weather Company Data**
	
3. **Time Agent**: Get Intu to tell you what time it is! Examples of time-related questions you can ask: _"What time is it right now"_ or _"What time is it in Tokyo"_.

	* Additional Bluemix services required: **Natural Language Understanding** and **Weather Company Data**
	
4. **Name Agent**: You can rename your Intu device or get Intu to learn your name! Examples of name-related queries you can tell Intu: _"Your name is now Bobby"_ or _"My name is John"_.  Once Intu learns your name, it will greet you by name using the **Greeter Agent** mentioned above.

	* Additional Bluemix services required (for renaming only): **Natural Language Understanding**
	* Additional Bluemix services required (for learning your name): **Natural Language Understanding** and **Visual Recognition**

	
5. **Telephony Agent**: Get Intu to call you or someone else! Please see Workshop 4 for more details on how to configure and use the Telephony Agent.

	* Additional services required: **Nexmo** -- _note: This is **NOT** a Bluemix service_


## C. <a name="creating-additional-services">Creating additional services in Intu</a>

As mentioned in the previous section, adding the Weather Company Data, Visual Recognition, and Natural Language Understanding Bluemix services can add more functionality to your Intu instance.

###To create and configure an instance of <u>Weather Company Data</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/weather-company-data" target="_blank">here</a> to be taken to the **Weather Company Data** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **username** and **password** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the Edit button (pencil icon) for **WeatherCompanyDataV1**.
	6. Paste your username and password into the **USER ID** and **PASSWORD** fields.
	7. Click **SAVE**

###To create and configure an instance of <u>Visual Recognition</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/visual-recognition" target="_blank">here</a> to be taken to the **Visual Recognition** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **api_key** and **url** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the **+ Add Service** button. A pop-up form will appear.
	6. In the **SERVICE NAME** field, put **VisualRecognitionV1**.
	7. In the **USER ID** field, paste the api_key obtained from Bluemix. You will leave the **PASSWORD** field blank as Visual Recognition uses an api key rather than a standard username/password.
	8. In the **SERVICE ENDPOINT** field, paste the url obtained from Bluemix.
	9. Click **SAVE**

###To create and configure an instance of <u>Natural Language Understanding</u> Service:

1. Verify you are logged into <a href="https://console.ng.bluemix.net/" target="_blank">Bluemix</a>.
2. Click <a href="https://console.ng.bluemix.net/catalog/services/natural-language-understanding" target="_blank">here</a> to be taken to the **Natural Language Understanding** Service page in Bluemix.
3. Click the **Create** button located at the bottom of the page.
4. Wait a few moments while your new service is being created. Once it is created you will automatically be taken to your new service's page.
5. Click on the **Service credentials** tab.
6. Click on the **View credentials** button.  The credentials for your new Bluemix service will be displayed.
7. Copy and paste the **username**, **password**, and **url** into the <a href="https://rg-gateway.mybluemix.net" target="_blank">Intu Gateway</a> by completing the following steps:
	1. Click on **MANAGE** on the left-hand sidebar of the Intu Gateway.
	2. Click on **Services**. You will be taken to the Services page on the Gateway.
	3. If not yet selected, select your **Organization** and **Group** towards the top of the page.
	4. You should see all your services displayed.
	5. Click the **+ Add Service** button. A pop-up form will appear.
	6. In the **SERVICE NAME** field, put **NaturalLanguageUnderstandingV1**.
	7. Paste your username and password into the **USER ID** and **PASSWORD** fields.
	8. In the **SERVICE ENDPOINT** field, paste the url obtained from Bluemix.
	9. Click **SAVE**


