
# Maze Tank with OpenGL
## Table of contents
* [General information](#general-information)
* [How to play](#how-to-play)
* [How execute](#how-execute)
* [Intelligence enemy](#intelligence-enemy)
* [Play with Arduino](#play-with-arduino)
* [Contributors](#contributors)



## General information

Project of the subject: Computer Graphics and Multimedia.
It consists of a one player game (Player vs Computer) where the board is a labyrinth. To win the tank must reach the opponent's starting square. If the opponent arrives before you, you lose.
There is also time, so you can't get distracted.

![game_image](https://user-images.githubusercontent.com/10574631/152810071-21123652-898b-4024-9e5f-90d5cfa04755.png)


## How to play

### Move Tank:

<img style="height:160px; width:230px;" src=https://user-images.githubusercontent.com/10574631/152811445-8a0c22da-1c04-4411-bf04-9d8f7566b9fe.png>


### Move Board:
- I: turns upward

- J: turns left

- K: turns downward

- L: turns right

<img style="height:160px; width:230px;" src=https://user-images.githubusercontent.com/10574631/152813770-018733fd-8ade-489e-9863-c02799c23122.png>



### Zoom:
 - O: More
 - P: Less

<img style="height:70px; width:150px;" src=https://user-images.githubusercontent.com/10574631/152814192-1f2d248b-e31a-4aeb-a73c-5c578a829eb5.png>


### Shoot:

<img style="height:70px; width:230px;" src=https://user-images.githubusercontent.com/10574631/152816565-e530711b-2192-4a65-898f-87012802da4e.png>

### Exit:
Press ESC to exit.


## How execute
**On MacOs and Linux:**

Open new terminal in folder of project. Make sure that you have all the dependencies installed. And run:
```
$ make all
```
Once you have executed this command, to play again you can use:
```
$ make test
```
You can make changes to the files but then you must run the first command to delete, compile and run.
Imports are done automatically on linux and MacOs depending on the operating system just like in the MakeFile file that executes some commands or others depending on the operating system.

## Intelligence enemy

The enemy tank has a bit of intelligence to make the game more difficult. It is equipped with a [DFS][1], algorithm to know which directions to take to reach the opponent's opponent's square.
The shooting of the opponent is random, it can move or shoot.

[1]: <https://en.wikipedia.org/wiki/Depth-first_search> "DFS"

## Play with Arduino
To make it more fun we have developed a small Arduino program to use a joystick to control our tank. The files are inside the [JoystickArduino](JoystickArduino/) and also a small [guide](https://www.instructables.com/How-to-Make-a-Arduino-HID-Keyboard/) on how to do it.

If the link to the guide does not work, inside the [folder](JoystickArduino/) there is the guide and the necessary files to be able to perform this part.

## Contributors
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<!-- markdownlint-disable -->
<center>
<div align="center">
<table align="center">
  <tr>
    <td align="center"><a href="https://github.com/arnaubena97"><img src="https://avatars0.githubusercontent.com/u/10574631?s=460&v=4" width="100px;" alt=""/><br /><sub><b>Arnau Benavides</b></sub></a></td>
    <td align="center"><a href="https://github.com/MarcFelip"><img src="https://avatars.githubusercontent.com/u/58440805?v=4" width="100px;" alt=""/><br /><sub><b>Marc Felip</b></sub></a><br /></td>

  </tr>
 </table>
 </div>
 </center>
<!-- ALL-CONTRIBUTORS-LIST:END -->