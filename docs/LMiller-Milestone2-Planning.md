> Use this worksheet to plan the next phase of your project **before you begin coding**
> Be clear, specific, and intentional—this will guide your development this week.

---
## 📌 Project Overview

**Project Name:**
→   Pac Man/Pac Man Invincible
  
**What does your program currently do? (1–3 sentences)**   
→ My program currently starts and gives the user menu options to choose from which include start game, view high scores, difficulty, and exit game. The game starts up and displays the game map with Pacman, ghost, pellets, and power pellets on screen. I have implemented sound effects into the program and Pacman movement.

---
## 🔍 Current Progress Check  
  
**What is working right now?**   
→   My menu works and cycles through all the selections and each can return user to menu. The game map, player, and enemy display on screen. Character movement works along with sound.
  
**What is NOT working or incomplete?**   
→   While sound works its not fully in game yet. Ghost spawning and AI as well as collision is not yet implemented. scoring and features need to also be added.
  
**What feels confusing or messy in your code?**   
→ I need to let the user know the input keys to use for game play. Also the scale of the game map feels way to small.

---
## 🚀 Feature Planning  
  
List the features you plan to add or improve this week.  
  
### Feature 1  
**Name:**   
→   Bonus score for points and fruit.
  
**What does this feature do?**   
→   give player additional ways to earn points to earn an extra life.
  
**Why is this feature important?**   
→   It extends game play and gives user additional goals and helps reach high score.
  
---
### Feature 2  
**Name:**   
→   Warp Tunnels
  
**What does this feature do?**   
→   Lets the user or ghost enter and teleport to the other side of game map.
  
**Why is this feature important?**   
→   This feature adds another level of fun and a way for the user to escape the ghost in hot pursuit.
  
---
  
### Feature 3 (optional)  
**Name:**   
→   My personal feature and twist to Pacman is to make a Pacman Invincible crossover game.
  
**What does this feature do?**   
→   This feature would transform Pacman into invincible, the young half viltrumite and transform ghost into Sequids, the parasite aliens from the series.
  
**Why is this feature important?**   
→   This feature adds a fun twist to the game and makes the project much more personal but also comes with another level of development challenge as it is like making two games in one. (I do have a safer feature planned as well and that is a taunt that angers ghost and speeds them all up.).
  
---
## 🧩 System Design Updates  
  
**Will you need to create any new classes? If so, which ones?**   
→   I don't believe I will need to create any new classes beyond the ones I have now.
  
**Will you modify any existing classes? How?**   
→   I will need to modify the Pacman and ghost class as well as the map. the ghost will need to be shown as sequids and spawn in waves that keep coming, and Pacman will need to take hits and have an attack button added. I would also like to give interior walls hit points and be destructible.
  
**What data structures will you use (vectors, 2D vectors, etc.)?**   
→   I will need to use vectors for the ghost/sequid spawns.
  
---
## 🔄 Program Flow  
  
**Describe how a user interacts with your program:**  
  
1. Program starts →   User presented with game menu and must make a selection.
2. User chooses →   If user selects game the game starts. User selects high scores, they can view  current 5 high scores. If user selects difficulty they will be able to select from easy, normal, or hard. This is also where I would like to make the secret Invincible game mode as an option that once selected transforms the entire game. User also has the option to exit and leave the game.
3. Program responds →   The program takes the selection and sends the user into the game map.
4. Loop/next step →   In the game the user can hit esc key to leave at any time. The user can also play and clear the map which will trigger the win condition and take them to the next map/level. If user loses game over and sent back to menu screen. Game currently has two maps and planned to cycle through 4 levels before map change is triggered.
  
---
## 🎯 Usability Improvements  
  
How will you make your program easier to use this week?  
  
- Clearer prompts:   
→   I will display key/escape inputs for the user on screen.
  
- Better error handling:   
→   Add the memory leak detection to code.
  
- Improved menu/navigation:   
→   Selection is highlighted so user knows current selection.
  
---
## 🎯 Usability Improvements  
  
How will you make your program easier to use this week?  
  
- Clearer prompts:   
→   Not a prompt but improve map visibility.
  
- Better error handling:   
→   
  
- Improved menu/navigation:   
→   refactor my map to use vectors for improved resizing and so on.
  
---
## ⚠️ Potential Challenges  
  
**What do you think will be the hardest part this week?**   
→   Collision and ghost AI since each ghost acts differently.
  
**What is your plan if you get stuck?**   
→   research potential fixes
  
---
## ⚠️ Potential Challenges  
  
**What do you think will be the hardest part this week?**   
→   For my optional feature the ghost AI changes since sequids are hive minded they will all act as one but also spawn in waves and keep coming and increase in speed which will flip my current game on its head.
  
**What is your plan if you get stuck?**   
→   Complete the base game so that i can begin working on this twist.
  
---
  
## 📈 Level Up Goal  
  
**What skill are you focusing on improving this week?**   
→   Not sure if this is considered a skill but refactoring my code and making the improvements so that things will flow better down the line.
  
**What will you do to improve it?**   
(e.g., tutorial, practice, debugging, office hours)   
→   Look at ways others have done what I am doing and get ideas on how to improve my current understanding. Complete my past class review that I started. 
  
---
## 🗓️ Task Breakdown (GitHub Issues Planning)  
  
List the tasks you plan to create as GitHub Issues:  
  
- [x]  Pacman on the scene 
- [x]   Ghost on Deck
- [x]   Fruit
- [x]   House Keeping - If time permits
  
---
  
## 🔥 Final Check  
  
Before you start coding, ask yourself:  
  
- [x] Do I know what I’m building this week?   
- [x] Do I know where to start?   
- [x] Did I break my work into small tasks?   
  
If yes → start coding 🚀   
If no → refine your plan first   
  
---
## 😈 Final Thought  
  
> Plan it now… or debug it later.