#The WebSocket Interface in Intu

WebSockets are a technology to maintain full-duplex connectivity between two
peer devices. Once the connection is established, there is no need for any
further handshakes or reconnection. This allows for seamless communication.

*Intu* relies on WebSockets for connections between devices (which could be a
computer, an Intu Manager instance, a robot, a Raspberry Pi, or another kind of
robot) as well as connections between those devices and their parent Intu
instances that run in the cloud. The parent Intu instances facilitate, among
other things, a way for all devices in an organization to share data, so that
teaching one device a skill enables all your devices registered to your
organization to automatically learn the skill.

*Intu* follows a certain methodology in its WebSocket implementation. The
preliminary connection to a host requires hitting the corresponding WebSocket
endpoint, for example

_ws://yourparentintu.mybluemix.net/stream?groupId=yourGroupId&selfId=yourSelfId&orgId=yourOrgId_

Here the *groupId* refers to the group ID of the particular group in your
organization that you are targeting. The *selfId* is the embodiment ID that your
device (that is trying to connect to the other device) receives once it
registers with the gateway. The *orgId* is your organization's ID, that you can
obtain from the gateway. But your devices also know your organization ID.

*Intu* maintains a list of *topics* that the client can then subscribe to [see below for more detail]. The
data model that is used to establish the connection and then to subscribe to
the various topics looks like this:

```
{
	"targets": [""],
	"msg": "query",
	"request": "1",
	"origin": "your Self Id followed by /."
}
```

Different programming languages have the freedom to use any WebSocket library
to connect to the hosts. *Intu* supports many SDKs, and many more are in the
works. Some of these languages provided WebSocket support out of the box, such
as *JavaScipt*. Some of these can use any third party WebSocket libraries.
Examples would be `okhttp3` for Java and `Twisted` for Python. It is also
possible to write your own WebSocket implementation from scratch.

A thin WebSocket client can be written to communicate with any Intu instance. The
main components of such an SDK would be the WebSocket component, and a few
custom agents, gestures, or sensors that can be added to the remote/host Intu
instance.

For a concrete example implementation of a SDK that allows a Unity 3D application
to connect to a running Intu instance and extend Intu with sensors, gestures,
or agents, see [here](https://github.com/watson-intu/self-unity-sdk)

##Publish-Subscribe

_Intu_ supports the publish/subscribe messaging pattern that enables clients to _publish_ messages that a designated  _Intu_ instance can then _subscribe_ to. The clients can also _subscribe_ to certain _topics_ that the _Intu_ instance is _publishing_.

The endpoint for the _Intu_ `TopicManager`, that supports this pattern, is `/topics/`.

A _topic_ is effectively a path. For example if you wanted to subscribe to the `sensors` on your parent _Intu_ instance from a child instance, you would use the _topic_ `../sensors` when you call _subscribe_. If you wanted to _subscribe_ to the same _topic_ on a child called **self01**, the _topic_ would have to be `self01/sensors`. If you wanted to _subscribe_ to **all** the children, you would use the `+` wildcard that _Intu_ supports, and say `+/self`.

The _Intu Blackboard_ acts as the central publish/subscribe system for all agents. Here is a list of _topics_ that you can subscribe to:

**agent-society**

**audio-out**

**blackboard**

**blackboard-stream**

**body**

**gesture-manager**

**graph-skills**

**log**

**sensor-manager**

**topic-manager**

Here are some examples of subscribing to various topics. You can either subscribe to topics on the host you are connecting to, or you can subscribe to those corresponding topics on a child Intu instance of a common parent.

```
{
	"targets": ["blackboard-stream"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

{
	"targets": ["log"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

{
	"targets": ["your parent Intu embodiment Id followed by /log"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

```

You can also unsubscribe from the topics if you so desire. A sample data blob to do so is as follows:

```
{
	"targets": ["log"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}
```


Here are some sample data blobs that can be used to _publish_ to the respective _topics_:

```
{
	"targets": ["gesture-manager"],
	"msg": "publish_at",
	"data": "{\"event\":\"execute_done\",\"gestureId\":\"tts\",\"instanceId\":\"f1540690-f89a-45bd-eeb2-9ea24f22ba7a\"}",
	"binary": false,
	"persisted": false
}

{
	"targets": ["blackboard"],
	"msg": "publish_at",
	"data": "{\"event\":\"subscribe_to_type\",\"type\":\"Text\",\"event_mask\":1}",
	"binary": false,
	"persisted": false
}

```

The `targets` above refer to a list of _topics_ from above that you want to publish to. The `message` is always going to be `publish_at`. If you supply `true` for `persisted`, the data will get persisted in the _Intu_ instance so that future _subscribers_ will get all the persisted data the moment they _subscribe_, rather than only getting live _published_ data.

For local testing (i.e. an _Intu_ instance running on the same physical device on which you are also running the _TopicClient_ through an SDK, you may leave the _SelfID_, _token_, and _orgId_ etc. empty in the data model. As is evident from the selective examples above, you see that the _publish_ and _subscribe_ can happen on textual data as well as binary data. For more information on what the actual data would look like for the individual _topics_, please refer to the open source [Intu Unity SDK](https://github.com/watson-intu/self-unity-sdk).

