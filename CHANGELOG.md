# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0] - 10/06/2019

Added: 
--------
- Barrels and Snowmans destruction animations
- Progressive text display
- New HUD
- Movement arrows
- VideoPlayer
- 3D logo
- Studio video
- Initial game video
- Map legend
- Charge ability bar

Changed:
--------
- Barrels and Snowmans can be destroyed by the basic attack
- Portal shows up after defeating boss
- Helper screen can exit with ESC
- Blizzard disappear when the boss die
- Credits screen

Removed: 
--------
- Compass
- Old HUD

Fixed: 
--------
- #465 If you are the archer or mage you can't sleep to active cutscene to go to quest 3
- #476 This monster goes through walls
- #482 In some menus, button B does not return
- #483 Player can drink potions in Lobby
- #485 When enters in a room that has the doors closed, the doors close before entering
- #486 When skips last cutscene, does not display final screen and you are blocked in lobby
- #487 Sometimes when deactives hud still blitting hp numbers
- #492 If you abort mission it doesn't heal you
- #500 New Game
- #503 Visual HP and MP bars

## [0.9.4] - 19/05/2019

Added: 
--------

Changed:
--------

Removed: 
--------

Fixed: 
--------
- Fixed ability mage and archer
- Fixed ability menu

## [0.9.3] - 19/05/2019

Added: 
--------

Changed:
--------
- Fire balls boss drake 3

Removed: 
--------

Fixed: 
--------
- Fixed cutscenes in boss 3 when player died
- Fixed crash in boss 3 when player died
- Fixed cutscenes in boss 2 when player died
- Fixed crash in boss 2 when player died
- Fixed chests interactions 

## [0.9.2] - 19/05/2019

Added: 
--------
- Shortcut abilities


Changed:
--------
- Faces in tps interactions
- Last Cutscene
- Return from quest 2 cutscene

Removed: 
--------
- 

Fixed: 
--------
- #447 When gets Ability 2, displays girl photo
- #470 First Fountain in Quest 2
- You cannot interact with entities when fading

## [0.9.1] - 19/05/2019

Added: 
--------
- Faces in cutscenes added
- Fxs added
- Portal added

Changed:
--------
- Lobby Map
- Lobby Ice Map
- Lobby Night Map
- Grammar dialog and cutscenes
- Characters images
- Save n load
- Balancing
- Treasure

Removed: 
--------
- 

Fixed: 
--------
- #378 Focus in dialogs is too little
- #380 When you die the player appear with the death animation when revived
- #383 The save game icon appears more than once when you save the game multiple times in a short period
- #387 The game crashes when the controls button is hitted.
- #400 If you die the health bar doesn't update to full bar
- #433 Go over closed door
- #434 Incorrect traduction on fountain
- #435 UI dissappeared after going to the fountain
- #437 Bars, compass and character image blit front ability screen.
- #445 Tutorial screen wrong
- #446 More screen fails
- #451 Guard helmet Visual Bug
- #462 If skip cutscene to go to sleep in the middle you are aground
- #463 You can interact while theres a cutscene

## [0.9.0] - 18/05/2019

Added: 
--------
- Boss 3
- All cutscenes
- Menu congratulations
- All enemies
- Musics and fxs
- Bosses health bars

Changed:
--------


Removed: 
--------

Fixed: 
--------
- #410 If the player open the inventory when recieving last thit the inventory stays in screen
- #415 If you die poisoned the sprite remains green once you revive
- #432 Incorrect traduction


## [0.8.2] - 17/05/2019

Added: 
--------
- Bomber enemy
- Poison visual effects
- Boss 2
- Minimap Icons
- First Phase Boss 3
- Fire State
- Lobby at night
- Water/Lava Animations
- Shop items bought
- Cutscenes Quest 2
- Rupees explosion 

Changed:
--------
- Correct focus image
- Minimap
- Resize buttons

Removed: 
--------
- Old gold bag

Fixed: 
--------
- #379 If an enemy kills you the game crashes 
- #385 Extra room drawn in the fountain room
- #391 If the player dies with a lot of enemies close the game crashes
- #393 The compass doesn't disappear when return to lobby from quest2
- #397 If you go to the quest 2 or 3 from the lobby, the HUD doesn't show up


## [0.8.0] - 12/05/2019

Added: 
--------
- Quest 3 rooms with colliders
- Exp bar
- Loop quest 3
- Bat enemy

Changed:
--------
- Inventory
- Experience

Removed: 
--------
- Old inventory

Fixed: 
--------
- #333 The rectangle of the worktop is too large
- #335 The game doesn't return the player to the lobby once you go to the map for a second time
- #341 Game crash when return to main menu and use the B button in controller
- #381 The fx are a little bit behind than they should
- #382 The game crashes when you try to make the load
- #386 The spawn of the player when return to the lobby from quest is wrong
- #388 The statues dialogs are wrong picked
- #390 The arrow ability disappear too soon

## [0.7.1] - 09/05/2019

Added: 
--------
- New objects descriptions in shop
- Full quest 2 static objects
- Daughter added in lobby as static entity


Changed:
--------
- Shop menu
- Camera Shake
- Cutscenes code
- Cutscenes Adapted to new code

Removed: 
--------

Fixed: 
--------
- Shop scroll fully functional

## [0.7.0] - 05/05/2019

Added: 
--------
- Cassio enemy added
- Blizzard added
- Special cassio venom attack added
- Full event system added
- Statick objects quest 2 added
- Shop scroll added
- More than one object in shop added


Changed:
--------
- Health and mana bars
- Super Frog special attack (pathfinding change)
- Menu's exit splines
- Lobby map (more credible)
- Cursor code
- HUD rework
- Clear code create entities in room

Removed: 
--------

Fixed: 
--------
- #334 If you go to the tutorial map from the lobby the HUD disappear
- #335 The game doesn't return the player to the lobby once you go to the map for a second time
- #341 Game crash when return to main menu and use the B button in controller

## [0.6.1] - 03/05/2019

Added: 
--------
- Save and Load
- 3rd ability
- Minimap (almost done)
- Button to skip cutscene
- Loop Quest 2
- Rooms quest 2
- Purple frog enemy added
- Slime enemy added

Changed:
--------
- Splines in almost all menus
- Menus code

Removed: 
--------

Fixed: 
--------
- #287 General volume of all audios should be way less loud
- #305 Shop bugs when you click fast to buy potions
- #332 The ability panel disable the HUD when you go back to the game
- #334 If you go to the tutorial map from the lobby the HUD disappear

## [0.6.0] - 28/04/2019

Added: 
--------
- Compass
- Potions as drops
- Slime spritesheet

Changed:
--------
- Lobby tmx
- Stats update when level up
- Tutorial tmx
- Code revision

Removed: 
--------

Fixed: 
--------
- Pre-Release 0.5.3 branch merge conflicts into develop branch


## [0.5.3] - 23/04/2019

Added: 
--------
- Death animations enemies
- 2 more tutorial panels

Changed:
--------
- Statue Animation

Removed: 
--------

Fixed: 
--------


## [0.5.2] - 23/04/2019

Added: 
--------
- Debug keys to pass through rooms
- New fountains sprites
- Statue tutorial added
- Idle animations

Changed:
--------
- Abilities blocked from the beginning
- Recover health and mana in the lobby
- Pivots are now in the debug collision key
- Controls of the controller and keyboard

Removed: 
--------
- Blue and greeen quads for the enemy and player positions

Fixed: 
--------
- #305 Shop bugs when you click fast to buy potions
- #300 HUD disappear when you talk to the NPC in the lobby
- #304 Arrow keeps doing damage to the enemies
- #306 If you die in the final battle at the tutorial it goes to the lobby



## [0.5.1] - 21/04/2019

Added: 
--------
- Input text
- Particles
- Cutscenes
- Game loop
- Audio and fxs
- All characters and enemies animations
- Abilities implemented
- Fully controller controls
- Doors logic
- Tutorial screens
- HUD function
- God mode
- Breakeable rocks
- Debug key change between characters
- Experience and levels
- Player selection menu
- Death menu

Changed:
--------
- Enemies fully implented
- Fountain functionality
- Credits


Removed: 
--------

Fixed: 
--------
- #213 If you are in death menu and you wait the game crashes
- #229 Bug report from release ver 0.5.0
- #264 If you use the flash ability the collider of the character gets off center
- #267 Tearing in fullscreen
- #282 Some fxs doesn't mute when the volume is supposedly 0
- #283 The camera in fullscreen follow the player too slow
- #284 Fullscreen button needs to be updated
- #286 Game sometimes crash when using too much the abilities
- #296 When you die if you click in return main menu the game sometimes crashes


## [0.5.0] - 17/04/2019

Added: 
--------
- Use of potions
- NPCs implemented
- Cutscene research implemented
- 2 Cutscenes added
- 3 enemies implemented (not fully)
- HUD added
- 3 characters implemented(not fully)
- Dynamic NPCs added
- Game icon added to exe

Changed:
--------
- Controller fully implemented
- Fountain fully implemented
- Debug screen fully implemented
- Shop logic fully implemented
- Drop sprite added
- Movement Quest
- Controls menu changed and fixed
- Warrior ability animation added

Removed: 
--------

Fixed: 
--------
- #195 Game crash at new game button (Bug photo inside)
- #180 Game crash if you click cancel button and then tutorial button
- #178 The game crash when you click in return main menu when dead
- #177 The game crash when you click Tutorial or cancel button to go to the tutorial map
- #145 Elements from inventory disappear (even the iventory, blocking the players game)
- #138 If you click in return when in control menu it returns to pause or main menu
- #137 If you click the options in the main menu it will click the fullscreen checkbox too

## [0.4.1] - 12/04/2019

Added: 
--------
- First Ability
- Go to quest
- Shop Logic (not finished)
- All tutorial rooms
- Room Logic
- Game over screen
- NPC shop
- Drops
- Debug Screen (not finished)



Changed:
--------
- Credits screen
- Focus improved
- More static entities (trees, rocks)
- Mouse icon changed
- Collision slide
- Movement change
- New controller controls

Removed: 
--------



Fixed: 
--------

- #134 If you try to connect the controller when the game is running it won't work
- #133 If you try to use inventory in option menu in the main menu then in the game you cannot use it
- #132 Fountain room crashes game
- #131 If you return to room 4 from room 5 the game crashes
- #130 Go to shop, pause menu go to main menu = crash
- #94 Options focus doesn't work properly on controls options
- #90 The option menu doesn't "remember visually the changes
- #88 Lobby Fountain incorrectly printed

## [0.4.0] - 07/04/2019

Added: 
--------

- Inventory
- Tutorial rooms
- Death Animation
- New lobby camera
- Death Logic
- Transitions between quest maps (not random)

Changed:
--------

- Lobby Tileset

Removed: 
--------

- Old lobby camera

Fixed: 
--------

- #98 Memory leaks when you enter and leave the home
- #97 The abilities menu appears in the option menu if you click the V key
- #96 The option menu text is too little compared to the focus
- #93 Option menu from main menu doesn't return to main menu
- #89 The lobby map blit sprites out of the map
- #86 New game Main menu loop crash the game
- #49 When you change map from hub - tutorial - hub the game crashes

## [0.3.1] - 05/04/2019

Added: 
--------

- UI focus
- Control change
- Option menu (not finished)
- Lobby camera (not polished)
- Credits Button
- Dialog System added

Changed:
--------

- Lobby Tileset


Removed: 
--------


Fixed: 
--------

- #34 The player sprite doesn't show up when you return to the main menu
- #46 When you enter to the home the screen stays black


## [0.3.0] - 01/04/2019

Added: 
--------

- Hub map
- Free movement works properly
- Colliders (non walkable tiles)
- Sensors
- Shop
- Home
- Transition Hub-Shop



Changed:
--------

- Now the transition between tutorial map and hub are the with the correct maps

Removed: 
--------

- Old hub map

Fixed: 
--------


## [0.2.1] - 29/03/2019

Added: 
--------

- Player attack.
- Now the player can choose the direction he/she wants to look to.


Changed:
--------


Removed: 
--------

Fixed: 
--------

- Issue 34 solved -> The player sprite doesn't show up when you return to the main menu.


## [0.2.0] - 24/03/2019

Added: 
--------

- Main menu
- Pause menu
- Abilities menu
- Enemy now look at the player
- Player's movement animation
- Player can attack
- First tutorial room
- Loop between hub and tutorial map

Changed:
--------

- Main menu button's position

Removed: 
--------

Fixed: 
--------

- Issue 7 solved -> Player's movement stop working properly when enemy is close to him.


## [0.1.0] - 17/03/2019

Added
 -----------
- Player movement 
- Entities management
- Pathfinding (not fully implemented)
- Turns system (not fully implemented)
- Transition between menu and scene

Changed
--------


Removed
--------


Fixed
--------

