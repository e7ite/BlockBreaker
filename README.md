# BlockBreaker
My final project for my CINS 465 Web Programming Fundamentals class. This is a remake of the classic Breakout game in HTML5 Canvas. THe purpose of this project was to explore modern web development frameworks and technologies. This is my first project that stores a database to keep track of player users.

## Technologies used
Django: Web development framework that allows easy management of project, and handles database of player names and scores.
      
WebAssenbly: Used to write game logic, including ball physics, collision detection, brick movement, and score. The purpose of using WebAssembly here was strictly for learning purposes.
      
Emscripten: Translated C++ game logic to WASM and JS to be used within HTML5.
      
Vue.js: REST API which allows updating of game scores in real-time.
      
Nginx: HTTP server used to allow players to connect.
      
HTML5 Canvas: Renders the game to the web page.

## Preview
![Preview](/preview.gif)

## Sources:
Breakout game template: https://youtu.be/14Vx2lxuWlI 

Emscripten game tutorial: https://blog.logrocket.com/first-game-in-webassembly/ 

Emscripten API: https://emscripten.org/docs/api_reference/index.html
