# Kontagion
A multi-level video game inspired by the 2011 film, Contagion.



# Description
### Objective 
* Eliminate all of the bacterias as they escape from the holes into the Petri dish  

### Built With
* **Frameworks:**
  * GLUT
  * OpenGL
  
### Game Objects
* **Player**
  * Can only move clockwise and counterclockwise around the perimeter of the Petri dish 
  * Equipped with a poison sprayer and an omni-directional flamethrower  

* **Bacterias**
  * **E. coli (blue)**
    * Super aggressive bacteria that can sense the player almost anywhere in the Petri dish
    * Ignore all food unless they happen to stumble over it
  * **Salmonellas (grey)**
    * **Regular Salmonella**
      * Wander around looking for and eating food, and will attack the player if they come in contact with them
    * **Aggressive Salmonella**
      * Will try to move toward the player and attack them if they're nearby
      * Otherwise, they behave like regular salmonella, hunting for and eating food
  * **NOTE:** All bacterias will undergo cellular division once they've eaten three pieces of food

* **Fungus**
  * Injures the player if it come in contact with the player

* **Goodies**
  * **Restore Health**
    * Restores player to full 100 hit points
  * **Extra Life**
    * Increases the number of lives a player has by 1
  * **Flamethrower**
    * Increases the number of flamethrower charges by 5
 
 

# Design
* Implemented the game using C++, exhibiting concepts such as inheritance, polymorphism, and encapsulation
* Designed the functionality of power ups, sound effects, and keyboard inputs to control player movements and shooting abilities
* Developed an algorithm for the special bacteria to follow the player by actively calculating the direction toward the player



# Getting Started
### Prerequisites
* **macOS:** Need to have XQuartz and freeGLUT installed to run the program
  * **Install [XQuartz](https://www.xquartz.org):**
    * Click the download link, open the downloaded .dmh file, double-click on XQuartz.pkg, and follow the installation instructions
    * Log out and log back in again
    * To verify the installation, open a Terminal window and run the command
    ```
    echo $DISPLAY
    ```
    * That should produce one line of output that ends with
    ```
    org.macosforge.xquartz:0
    ```
  * **Install freeGLUT:**
    * Install the [homebrew](https://brew.sh) package manager
    * Open a Terminal window and run the command
    ```
    brew install freeglut
    ```


# Screenshots
### Start Screen
<img src="https://imgur.com/YEkFnHx.png" width="50%" height="50%">

### Game Screen
<img src="https://imgur.com/q5k8ILa.png" width="50%" height="50%">

### Lost Life Screen
<img src="https://imgur.com/bWO2dr8.png" width="50%" height="50%">

### Game Over Screen
<img src="https://imgur.com/Bn4LWFN.png" width="50%" height="50%"> 


# Acknowledgements
* The files Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp were made by me
* All other files were supplied by the UCLA CS Department for CS32 under the instructors David Smallberg and Carey Nachenberg
  * Request that any distributions or modifications made to these files be made upon appropriate permission from the UCLA CS Department
