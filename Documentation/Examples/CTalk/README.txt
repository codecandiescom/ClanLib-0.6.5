         Name: CTalk
       Status: Windows(Y), Linux(Y)
        Level: Intermediate
      Modules: app, core, gui, network, display
Last modified: 16. october 2001
     Progress: Not complete
   Maintainer: Kenneth (sphair) and Magnus (mbn)
      Summary: Network chat client using GUI

CTalk design
------------

Server:

The server has a DocumentServer class that handles all message routing
between the connected ctalk clients. It has a list of accounts
(username/password) that are allowed to connect to the service, and
then it contain a list of connected clients.

When a client connects to the server, the DocumentServer will wait for
the client to send an initial login message that contain the username &
password for the user. Until it receives this information, the client
will not be added to the list of ctalk logins. This means that the rest
of the document server, as well as other clients, will not know about
the client until it has done a successful login.

When the document server receives a talk message from a ctalk client,
it will broadcast the information to all other registered logins. This
done simply by walking the logins map in the document server, and sending
the message to each individual client.

Whenever a client has done a successful login (when server has received a
login msg from client, and verified the username & password), the server
will add the client to the map of logins. Thereafter it will send a talk
message containing all the text current users are typing. This of course
means that the server must have a small log of the current messages being
written.

All clients receive a "user joined" message when a user connects, and they
also receive a "user left" when a user disconnects again.

Client:

The client's network communication is handled by the Document class. It
exports a list of properties, operations and signals that the ctalk views
use to display information to the user.

When a Document class is constructed, it connects to the document server
and sends a login message. The constructor then returns. The CTalk active
view should then connect itself to the sig_login_ok and sig_login_fail to
wait for a confirmation or denial of the connection attempt.

As soon as the document has been successfully logged in, the document
server will send a serie of user added messages, and it will send a text
update for every user currently typing something. The view should connect
to the signals emitted if they want to respond to this information. The
document class itself will also maintain a list of users connected and last
text updates received. Views should use these attributes and signals to
display the talk channel itself.
