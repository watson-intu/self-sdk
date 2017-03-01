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

The _Intu Blackboard_ acts as the central publish/subscribe system for all agents. Below you will find a list of _topics_ that you can subscribe to, publish to, unsubscribe from, and also details of the data blobs that you need to use to perform those actions. Most of these _topics_ are very versatile, and allow all of the above actions.

You can either subscribe to topics on the host you are connecting to, or you can subscribe to those corresponding topics on a child Intu instance of a common parent.

For all of the `publish` examples, optionally you can add an `origin` entry, but it has a default value which tells the _TopicClient_ that the SDK is running on the same machine. If your _Intu_ instance and the SDK are running on the same physical machine, you can leave the `origin` blank.

The `targets` refer to a list of _topics_ from above that you want to publish to. The `message` is always going to be `publish_at`. If you supply `true` for `persisted`, the data will get persisted in the _Intu_ instance so that future _subscribers_ will get all the persisted data the moment they _subscribe_, rather than only getting live _published_ data.

The `event_mask` in the examples below is a combination of bit flags so that you can subscribe to specific events. The following are the big flags you can use:

```
	TE_NONE			= 0x0,			// no flags
	TE_ADDED		= 0x1,			// IThing has been added
	TE_REMOVED		= 0x2,			// IThing has been removed
	TE_STATE		= 0x4,			// state of IThing has changed.
	TE_IMPORTANCE	= 0x8,			// Importance of IThing has changed.
	TE_GUID			= 0x10,			// GUID has been changed
	TE_DATA			= 0X20,			// data of this thing has been changed

	TE_ALL = TE_ADDED | TE_REMOVED | TE_STATE | TE_IMPORTANCE | TE_GUID | TE_DATA,
    TE_ADDED_OR_STATE = TE_ADDED | TE_STATE
    
```

**agent-society**

*Subscribing*
```
{
	"targets": ["agent-society"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

```

*Unsubscribing*
```
{
	"targets": ["agent-society"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

```
*Publishing*
```
{
	"targets": ["agent-society"],
	"msg": "publish_at",
	"data": "{\"event\":\"add_agent_proxy\",\"agentId\":\"de722486-fece-4a6e-95a9-6dd14cd31a79\",\"name\":\"ExampleAgent\",\"override\":false}",
	"binary": false,
	"persisted": false
}

```

In the above example, the `event` can be `add_agent_proxy` or `remove_agent_proxy`. The `override` flag would override an existing agent of the same name in this agent society.

**blackboard**

*Subscribing*
```
{
	"targets": ["blackboard"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}
```

*Unsubscribing*
```
{
	"targets": ["blackboard"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}
```
*Publishing*
```
{
	"targets": ["blackboard"],
	"msg": "publish_at",
	"data": "{\"event\":\"subscribe_to_type\",\"type\":\"Text\",\"event_mask\":1}",
	"binary": false,
	"persisted": false
}
```

In the above example, the `event` could be `add_object`, `set_object_state` (string) , `set_object_importance` (float), `remove_object`, and `unsubscribe_from_type`.

**blackboard-stream**

*Subscribing*
```
{
	"targets": ["blackboard-stream"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}

```

*Unsubscribing*
```
{
	"targets": ["blackboard-stream"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}

```
*Publishing*
```
{
	"targets": ["blackboard-stream"],
	"msg": "publish_at",
	"data": "{\"event\":\"subscribe_to_type\",\"type\":\"Text\",\"event_mask\":1}",
	"binary": false,
	"persisted": false
}
```

**gesture-manager**

*Subscribing*
```
{
	"targets": ["gesture-manager"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}
```

*Unsubscribing*
```
{
	"targets": ["gesture-manager"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}
```
*Publishing*
```
{
	"targets": ["gesture-manager"],
	"msg": "publish_at",
	"data": "{\"event\":\"execute_done\",\"gestureId\":\"tts\",\"instanceId\":\"f1540690-f89a-45bd-eeb2-9ea24f22ba7a\"}",
	"binary": false,
	"persisted": false
}
```

In the above example, the `event` can be `add_gesture_proxy`, `remove_gesture_proxy`, and `execute_done`. All three would require `gestureId` and `instanceId` to be passed in.

**sensor-manager**

*Subscribing*
```
{
	"targets": ["sensor-manager"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}
```

*Unsubscribing*
```
{
	"targets": ["sensor-manager"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}
```
*Publishing*
```
{
	"targets": ["sensor-manager"],
	"msg": "publish_at",
	"data": "{\"event\":\"add_sensor_proxy\",\"sensorId\":\"asdf\",\"name\":\"keyboard\",\"data_type\":\"TextData\",\"binary_type\":\"KeyboardData\",\"override\":true}",
	"binary": false,
	"persisted": false
}

```
In the above example, the `event` can be (analogous to `gesture-manager`) `add-sensor-proxy` and `remove-sensor-proxy`, and both would in turn require the `sensorId`, `name`, `data_type`, `binary_type`, and `override`.

**graph-skills**

*Subscribing*
```
{
	"targets": ["graph-skills"],
	"msg": "subscribe",
	"origin": "your Self Id followed by /."
}
```

*Unsubscribing*
```
{
	"targets": ["graph-skills"],
	"msg": "unsubscribe",
	"origin": "your Self Id followed by /."
}
```

*Publishing*

```
{
	"targets": ["graph-skills"],
	"msg": "publish_at",
	"data": "{\"event\":\"traverse\",\"payload\":\"{
  "Type_" : "OutTraverser",
  "m_spCondition" : {
     "Type_" : "EqualityCondition",
     "m_EqualOp" : "EQ",
     "m_Path" : "_label",
     "m_Value" : "reports_to"
  },
  "m_spNext" : {
     "Type_" : "FilterTraverser",
     "m_spCondition" : {
        "Type_" : "EqualityCondition",
        "m_EqualOp" : "EQ",
        "m_Path" : "name",
        "m_Value" : "Richard"
     }
  }
}\"}",
	"binary": false,
	"persisted": false
}

```
In this example above the `event` could be `traverse`, `create_vertex`, `delete_vertex`, `create_edge`, etc. for example. The payloads would contain serialized information that the system can then use to create, update, or delete vertices or edges in the graph. You could also supply serialized traversal conditions that the graph can then use to traverse itself looking for all vertices that satisfy the traverser's conditions.

It is possible to register your own _types_ that are not part of the schema. _Intu_ supports generic `Thing` objects that you can register against the system.

Some of the _topics_ such as **log** and **body** are just flows of textual data that you can subscribe to:

**log**

**body**

**audio-out**

**topic-manager**


For local testing (i.e. an _Intu_ instance running on the same physical device on which you are also running the _TopicClient_ through an SDK, you may leave the _SelfID_, _token_, and _orgId_ etc. empty in the data model. As is evident from the selective examples above, you see that the _publish_ and _subscribe_ can happen on textual data as well as binary data. For more information on what the actual data would look like for the individual _topics_, please refer to the open source [Intu Unity SDK](https://github.com/watson-intu/self-unity-sdk).

