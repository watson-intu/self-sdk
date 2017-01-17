#Frequently Asked Questions: Architecture

###Here is a collection of FAQs specific to the Intu architecture

* Is there an orchestration layer in the backend architecture of Intu? If so, what is it?
  * Sample answer

* For a given topic, can the blackboard handle multiple outputs from a single/multiple agents and render them to the front end?
  * Sample answer

* What exactly is a "parent" vs. a "child"?
  * Sample answer

* When a question is asked, a goal object is posted onto the blackboard for the goal agent to then determine the plan of action. How is the goal object generated, and by whom and where?
  * Sample answer

* What is a goal object? Is there only one or multiple per question?
  * Sample answer

* Why do we need endpoints of services (e.g: conversation) in the avatar's config file when it is the Intu console which handles the thinking and contact the bluemix services via agents? Wouldn't it make more sense that the endpoints are configured on the Intu console?
  * Sample answer

* How does the avatar communicate with the Intu console? Is it using the blackboard somehow? Or is it through a host IP? Or both?
  * Sample answer

* According to the Self architecture document, the different subsystems (sensors, models, actuators, agency) have their own blackboard. Are these blackboard separated? If yes, do their agents talk to one another in the same environment?
  * Sample answer

* What is the relationship and flow between a topic, intent and agents?
  * Sample answer

* Why do we have to edit the body JSON file manually? How would we go about editing 100+ instances of this across users? And/or add agents at a later stage?
  * Sample answer

* How will we go about enabling multiple users, as each user will not have a Bluemix account?
  * Sample answer

* All Internet connection to and from Intu and the avatar need to be proxy aware. Can we specify the proxy settings, and add an exception list (list of URLs that will get ignored by the proxy)
  * Sample answer

* How are we going to install Intu on machines, without using the vanilla install, as we will have modified/updated agents.
  * Sample answer

[Back to the index](../../README.md)
