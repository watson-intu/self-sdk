### Release Notes for RC4 (Version 0.1.410, last updated 3/27/2017)

If you are upgrading from RC3, please view [UPGRADE.md](./UPGRADE.md).

* Intu Core
	* Visual recognition integrated into conversation context.
	* OpenCV is now used to detect faces locally.
	* Implemented local cache for conversation.
	* Streaming speech gesture is implemented for Windows reducing latency on speech.
	* GreeterAgent now creates a goal.
	* ISensor based objects now have a serialized sensor name.
	* Implemented default emotions for Mac, Windows, and Linux.
	* Implemented Camera sensor for Windows.
	* Camera is now off by default.
	* Linux now has a native implementation for speech.
	* Intu now uses expressive speech if available.
	* Implemented DiscoveryAgent for discovering other Intu instances on the network.
	* Platform specific configurations are now separated from the body.json.
	* Conversation emotes are now executed based on their position within the text.
	* Implemented MoveJointGesture for Raspberry Pi.
	* Documentation is now a sub-module located in its own repository.
	* Implemented connection pooling for REST calls.
	* Implemented WeatherCompanyData service.

* Intu Models
	* Implemented ModelsAgent for working with the graph.
	* Graphs are now searched asynchronously; locally found data is used immediately if available.
	* Skills & Plans are now stored in the models of the world.
	* The graph is used to parse language, replacing the use of NLC.
	* Models of others is updated when new persons are learned.
	
* Tooling
	* SSO now occurs inside tooling instead of launching an external browser.
	* Tooling flow updated to remove the manage/install screen.
	* Intu Manager can now auto-update itself to new versions of the tooling.
	* You can now disable/enable the camera using tooling.
	* Tooling can now connect directly to an Intu instance without a parent.

* Known Issues
	* Self auto upgrade is currently disabled.
	* Resetting the avatar with a long press doesn't reset the topic subscriptions correctly, resulting in multiple subscriptions to the same topics.
	* Tooling has stability issues if running continuously for more than 24 hours.

### Release Notes for RC3 (Version 0.1.269, last updated 12/1/2016)

* Notes
	* You may need to delete the "wdc" directory to get latest due to that directory being converted into a sub-module.
	* If you use git bash to checkout, run the following command to checkout the submodule `git submodule foreach git pull origin develop`.
* Bug Fixes
	* Fixed an issue where organizations with apostrophes in the Organization Name could not connect to parents.
	* Fixed an issue where previously users could not connect to parents because of scalability problems.
	* Fixed an issue where Intu Gateway users were frequently logged out and redirected to the login page.
	* Fixed an issue where multi-level conversation did not retain context.
	* Included required executable vc_resti.x86.exe for the Windows build.

