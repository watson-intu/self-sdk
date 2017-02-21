#Frequently Asked Questions: Gateway

###Here is a collection of FAQs specific to the Intu gateway

* What is the self.dialog in the services used for, as we seem to never edit it?
  * Sample answer

* Credentials: M_BearerToken expires after a period, how long is this period, and what was the thought behind this as we would have to manually update all Intu users every time this expires?
  * Sample answer

* What is the parent in the gateway? And what would one use it for?
  * Sample answer
  
* What services are required to run Intu?
  * Sign into the Intu Gateway. Select Manage > Services. Select your organization and group. Verify that you configured the following services with these specific names in the "Service Name" field:
   * NATURAL LANGUAGE CLASSIFIER
   * Service Name: NaturalLanguageClassifierV1
   
   * SPEECH TO TEXT:
   * Service Name: SpeechToTextV1
   
   * TEXT TO SPEECH:
   * Service Name: TextToSpeechV1
   
   * CONVERSATION
   * Service Name: ConversationV1
   
   * Additionally for Conversation service, you will need to click the "+" button to add your Conversation workspace(s). For these key/value pairs, the key named "self_dialog" needs to have you workspace value from bluemix. The key named "self_domain" should have the workspace id created when you imported our starter kit. (For more instructions on creating a workspace from our starter kit, see https://github.com/watson-intu/self-sdk/blob/develop/docs/workshops-devcon/2/lab-docs/workshop_2.md),













[Back to the index](../../README.md)
