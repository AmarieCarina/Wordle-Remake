# Wordle Clone - SFML C++ (tema 3 POO)

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

### 4. Mecanism de GamePlay (`Hint`)
Clasa responsabilă cu gestionarea sistemului de indicii (ajutoare) puse la dispoziția jucătorului în timpul gameplay-ului activ.
Monitorizează rândul curent pe care se joacă și completează automat prima căsuță goală cu litera corectă din cuvântul țintă (`targetWord`). Clasa încapsulează logica de blocare a celulelor folosite (starea `Hint`, colorată dedicat în roz), previne ștergerea accidentală a acestora și limitează abuzul prin oprirea funcționalității după maximum 5 utilizări per joc.


---
## Templates
* **functie libera/normala template** `alignElements<T>`
  * funcție utilitară care aliniază geometric o listă de elemente pe ecran
  * funcționează cu un vector de GridCell, un vector de Label sau orice alt element de UI derivat, eliminând complet duplicarea de cod.
* **modificare in clasa template** `PopUp<T>`
* **clasa template** 
  * `ElementHolder`
* **functie membru dependenta de T** 
  * `void updateData(<T>)`
* **data membra dependenta de T**
  * `data`
  
---
## Design Patterns
1. `OBSERVER`
   * implementata prin interfața abstractă `ButtonObserver` și clasa `Button`. 
   * clasa Button nu știe cine este motorul jocului (GE); ea deține doar o listă de pointeri/referințe către observatori (`addObserver`). 
   * in loc să folosim funcții callback clasice sau pointeri rigizi către funcții, butoanele din PopUp-uri (cum ar fi `playButton` sau `closeButton`) își notifică observatorii în momentul în care sunt apăsate, apelând metoda `onButtonClick("PLAY")` sau `onButtonClick("CLOSE")`. 
   * clasa GE moștenește `ButtonObserver` și reacționează centralizat la aceste mesaje.
<br><br>
2. `STRATEGY`
   * implementat prin interfața `ColorStrategy` și derivata sa, `ClassicWordleStrategy`  
   * algoritmul care decide ce culoare primește o celulă în funcție de starea ei (**Correct**, **Present**, **Absent**) este complet izolat de clasa `GridCell`. 

---
## Resurse
- [NYT WORDLE - Daily Word Game](https://www.nytimes.com/games/wordle/index.html)
- [SFML library](https://www.sfml-dev.org/tutorials/3.0/graphics/draw/)
- [GITHUB: shmookey - 3102 common 5-letter words](https://gist.github.com/shmookey/b28e342e1b1756c4700f42f17102c2ff)
- [DINOPIXEL: icon joc](https://dinopixel.com/wordle-logo-pixel-art-42618)
---

## Cerințe indeplinite
- [x] minim o funcție șablon și o clasă șablon (template)
- [x] modificați o clasă existentă și transformați-o în clasă template
- [x] adăugați (minim) un atribut de tip T sau care depinde de T
- [x] adăugați (minim) o funcție membru care să depindă de T
- [x] adăugați (minim) o funcție normală/liberă template
- [x] minim 2 design patterns

---

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).
