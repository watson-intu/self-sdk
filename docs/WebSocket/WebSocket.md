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

*Intu* maintains a list of *topics* that the client can then subscribe to. The
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

Here is an example of subscribing to various topics. You can either subscribe
to topics on the host you are connecting to, or you can subscribe to those
corresponding topics on a child Intu instance of a common parent.

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
