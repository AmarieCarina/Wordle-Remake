<h1 align="center"> Wordle Clone - SFML C++ </h1>
 
<p align="center">
 <img width="400" height="450" alt="image" src="https://github.com/user-attachments/assets/520692a7-4cfa-424d-8e1a-f46fe04b9c88" />
</p>

A modern and interactive implementation of the famous **Wordle** game, developed in C++ using the **SFML** graphics library. 
The project emphasizes a clean architecture and efficient resource management.


## Game Description
The objective is simple: guess the secret 5-letter word in a maximum of **6 attempts**.
* After each guess, the cells change color to provide feedback:
  * 🟩 **Green**: The letter is correct and in the correct position.
  * 🟨 **Yellow**: The letter is part of the word, but in a different position.
  * ⬛ **Gray**: The letter is not part of the word.
* The game features an interactive virtual keyboard that updates in real-time, along with custom Pop-Up windows for instructions and game status.
<p align="center">
 <img width="200" height="220" alt="image" src="https://github.com/user-attachments/assets/e1ac5a14-cb1e-4d83-bcb9-adc1733c097c" />
</p>


## Resources
- [NYT WORDLE - Daily Word Game](https://www.nytimes.com/games/wordle/index.html)
- [SFML library](https://www.sfml-dev.org/tutorials/3.0/graphics/draw/)
- [GITHUB: shmookey - 3102 common 5-letter words](https://gist.github.com/shmookey/b28e342e1b1756c4700f42f17102c2ff)
- [DINOPIXEL: icon joc](https://dinopixel.com/wordle-logo-pixel-art-42618)



## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).
