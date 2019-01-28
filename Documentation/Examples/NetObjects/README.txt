         Name: NetObjects
       Status: Windows(Y), Linux(Y)
        Level: Advanced
      Modules: app, core, gl, network, gui, png, jpeg, display, sound
Last modified: 16. october 2001
     Progress: Not complete
   Maintainer: Kenneth (sphair) and Magnus (mbn)
      Summary: A whole game, using most technologies in ClanLib.

Demonstrates a whole lot:
- Using OpenGL to draw 2D
- GUI with custom OpenGL theme
- Client/Server architecture
- Network game using NetChannels
- Lobby mechanism with chat
- GameEngine
- Sprites
- Sound, PNG, JPEG

Note: The gameplay is not complete at all - there is no gameplay, nor any
network communication when playing. So far, the lobby and the GUI is the
only "operational" parts of this example.

-----------------------------------------------------------------------------
Documentation:

NetObjects example game, network implementation.

The purpose of this document is to try and describe how the different
modules of the NetObjects example will seperate their network communication,
and how communication is transfered between the different modules (eg. when
World starts from Lobby).

When a netobjects server is running, initially there will only be a lobby.
The lobby accepts computers joining the session, asks them about their
player properties (name, team, etc.), and then allows them to chat with each
others. All this communication is handled from within the Lobby class and
its helper classes.

At some point all players have agreed to start the game, and this causes the
Lobby to create a World. During this construction, the server side lobby
will transfer player information to the server world. In other words, the
server world construct a new list of players active in the game. This new
list contain additional information than the player structure of lobby. For
instance, it could contain a score, what amount of credits he have left to
buy ships for, etc etc.

The important part here to note is, that Lobby and World have nothing to
with each other, except that Lobby spawns World. The Lobby player structure
should not have any information about in-game player settings, except those
needed when joining the game. One of the reasons for this is that we want
the Lobby to continue running after the game started, allowing new players
to connect to the server and put them in the lobby. Also, if someone is
defeated in World, we want him to stop exist for World, but let him fall
back into the lobby.

Below is a small example of how the network channels and lobby/world
interaction should happen:

1. Server netsession and server lobby boots.

2. A computer connects to the netsession.

3. Computer registers itself to the lobby.

4. Lobby sends back a player list of ppl in the lobby.

5. Computer sends a "player ready" message to lobby.

6. Lobby notices all players in lobby are ready, sends back a message
that game is starting.

7. Lobby creates a server world. Lobby transfers player list to server
world. Lobby registers that players are currently in-game in its own player
list.

8. Server world sends startup data to client worlds (world player list, map
info, initial game objects).

9. Server world runs game, sending replication code to clients.

10. A player is killed, he returns to lobby. Server world informs lobby that
player died. Lobby marks player as in-lobby.

11. A new computer connects to the netsession while game is running. The
lobby does its usual handshaking. World is not informed.

12. Game ends. Server world returns all players to lobby.

The purpose of this 'player transfering' is to let the lobby use its own
dedicated netchannels that only affect the lobby, and the world have its own
netchannels and booting that only affect the game. Its all about code
seperation, the lobby knows minimal about the world (it doesnt care, except
for spawning and getting answers when players return), and the world knows
nothing about the lobby or who is in it (it only cares about those in the
game).

- Magnus Norddahl <mbn@clansoft.dk>
