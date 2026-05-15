# Wordle Clone - SFML C++ (tema 2 POO)

O implementare modernă și interactivă a celebrului joc **Wordle**, dezvoltată în C++ folosind biblioteca grafică **SFML**. 
Proiectul pune accent pe o arhitectură curată și gestionarea eficientă a resurselor.

## Descrierea Jocului
Obiectivul este simplu: ghicește cuvântul secret de 5 litere în maximum **6 încercări**.
* După fiecare încercare, celulele își schimbă culoarea:
  *  **Verde**: Litera este corectă și se află în poziția corectă.
  *  **Galben**: Litera face parte din cuvânt, dar se află în altă poziție.
  *  **Gri**: Litera nu face parte din cuvânt.
* Jocul include o tastatură virtuală interactivă care se actualizează în timp real și ferestre de tip Pop-Up pentru instrucțiuni și starea jocului.

---

## Ierarhie de Clase

### 1. Game Engine `GE`
Gestionează starea jocului, sincronizarea dintre gridul de litere și tastatură, și verificarea logică a cuvintelor. De asemenea, centralizează toate componentele într-o singură entitate logică.

### 2. Interfata Utilizatorului (*User Interface*) `UI`
Am implementat o ierarhie polimorfică pentru elementele de interfață:
`UI` este **clasa de bază**, ce definește comportamentul si datele elementar al oricarui obiect grafic (desenare, vizibilitate).
* **`PopUp`**: Un container complex care utilizează `std::unique_ptr<UI>` pentru a stoca dinamic elemente precum etichete (`Label`), butoane sau celule de grid. 
Această clasa permite compunerea unor ferestre complexe (`Intro`, `Instr`) în mod modular.
<br><br>
* **`GridCell`**:  Este unitatea fundamentală a tablei de joc.
Fiecare celulă gestionează o singură literă și o **stare vizuală** (`CellState`).
În funcție de starea setată (`Correct`, `Present`, `Absent` sau `Default`), celula își actualizează automat culoarea de fundal și a conturului, oferind **feedback vizual instantaneu** jucătorului.
  <br><br>
* **`Label`**:  O componentă utilizată pentru afișarea textelor statice sau dinamice.
Spre deosebire de un simplu `sf::Text`, clasa `Label` încapsulează logica de aliniere (centrare automată în funcție de origine) și poate fi integrată ușor în containere complexe precum `PopUp`.
  <br><br>
* **`Button`**:  Elementul principal de interactivitate prin mouse.
Implementează detectarea coliziunii (`isMouseOver`) și gestionează un **callback** (folosind `std::function`).
Acest lucru permite butonului să execute orice fragment de cod atunci când este apăsat (de exemplu, închiderea unui PopUp sau pornirea jocului), fără ca butonul să cunoască detaliile logice ale restului programului.
  <br><br>
  * **`Key`**:  O specializare a butoanelor, adaptată special pentru tastatura virtuală.
Pe lângă logica de click, o instanță `Key` își poate schimba starea de culoare în mod persistent (dacă ai încercat o literă și e greșită, tasta rămâne gri pe tot parcursul jocului).
Aceasta servește ca punte între input-ul utilizatorului și logică, transmițând caracterele direct către Game Engine.

### 3. Gestionarea Cuvintelor (`WordManager`)
Cuvintele sunt încărcate dintr-un dicționar extern. Verificarea validității unui cuvânt introdus se face rapid pentru a asigura integritatea regulilor jocului.

---

---

##  Elementele Vizuale 
* **Tastatura Dinamică**: Tastele se colorează pe măsură ce joci, ajutându-te să elimini vizual literele care nu se află în cuvânt.
  <br><br>
* **Sistemul de Pop-Up-uri**: Instrucțiunile de joc sunt prezentate printr-un Pop-Up care conține un mini-grid demonstrativ, construit dinamic în `main`.
  <br><br>
* **Fonturi Personalizate**: Utilizarea `Diplomata-Regular` pentru titluri oferă o estetică distinctă și premium.

---
## Resurse
- [NYT WORDLE - Daily Word Game](https://www.nytimes.com/games/wordle/index.html)
- [SFML library](https://www.sfml-dev.org/tutorials/3.0/graphics/draw/)
- [GITHUB: shmookey - 3102 common 5-letter words](https://gist.github.com/shmookey/b28e342e1b1756c4700f42f17102c2ff)
---

## Cerințe indeplinite
- [ ] minim o funcție șablon și o clasă șablon (template)
- [ ] modificați o clasă existentă și transformați-o în clasă template
- [ ] adăugați (minim) un atribut de tip T sau care depinde de T
- [ ] adăugați (minim) o funcție membru care să depindă de T (sau de alt parametru template);
- [ ] adăugați (minim) o funcție normală/liberă template; poate să fie friend
- [ ] minim 2 design patterns



## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).
