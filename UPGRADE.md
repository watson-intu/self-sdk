## Upgrading from RC3 to RC4
* Platform specific configuration data has been moved out of the body and into one of the **platform.json** files located in **etc/shared/platforms**. If you have customized your body.json you should diff your old body.json against the new one at **etc/profile/body.json**.
* Credentials have been moved out of the **body.json** into a **config.json** file. You may remove the **m_EmbodimentCreds** from your existing **body.json**.
* If you have customized your **m_IntentClasses** in the **TextClassifier** object, you will need to copy those intents into your new **body.json**.
*  If you have not customized the core conversation or NLC, then you can skip this section. 
	* Intu is phasing out the use of **Natural Language Classifier** to understand how to handle requests from the user to execute simple requests (e.g. Raise your right hand). Intu should continue to be backwards compatible for simple requests. If you would like to upgrade to the new method which uses a graph search and **Conversation Service** to handle those requests please follow these steps:
	* NLCProxy should be removed from your body.json .
	* You may remove the **NaturalLanguageClassifierV1** service credentials from both the body.json and the Intu Gateway.
	* You may remove the **NaturalLanguageClassifier** service from the body.json.
	* RequestAgent in the body should be updated to the following:
	```
	{
		 "Type_" : "RequestAgent",
		 "m_EntityFilter" : [ "verb", "noun", "directionPerpendicular", "directionParallel", "adjective" ],
		 "m_RequestFailedText" : [ "I do not know how to %s." ],
		 "m_RequestFiles" : [ "shared/self_requests.json" ]
	},
	```
	* You should re-import the default conversation from the starter kit; this is available in the downloads section of the [Intu Gateway](https://rg-gateway.mybluemix.net). This new conversation adds a **#request** intent along with the entities parsed from the speech. The graph is searched using these entities to find an action to execute; your graph is initizlized from the file located at **etc/shared/self_requests.json**. 

	

