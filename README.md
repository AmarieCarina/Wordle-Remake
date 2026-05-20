# Wordle Clone - SFML C++ (tema 3 POO)
<p align="center">
 <img width="400" height="450" alt="image" src="https://github.com/user-attachments/assets/520692a7-4cfa-424d-8e1a-f46fe04b9c88" />
</p>

O implementare modernă și interactivă a celebrului joc **Wordle**, dezvoltată în C++ folosind biblioteca grafică **SFML**. 
Proiectul pune accent pe o arhitectură curată și gestionarea eficientă a resurselor.

## Descrierea Jocului
Obiectivul este simplu: ghicește cuvântul secret de 5 litere în maximum **6 încercări**.
* După fiecare încercare, celulele își schimbă culoarea:
  *  **Verde**: Litera este corectă și se află în poziția corectă.
  *  **Galben**: Litera face parte din cuvânt, dar se află în altă poziție.
  *  **Gri**: Litera nu face parte din cuvânt.
* Jocul include o tastatură virtuală interactivă care se actualizează în timp real și ferestre de tip Pop-Up pentru instrucțiuni și starea jocului.
<p align="center">
 <img width="200" height="220" alt="image" src="https://github.com/user-attachments/assets/e1ac5a14-cb1e-4d83-bcb9-adc1733c097c" />
</p>

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
<p align = "center">
 <img width="300" height="330" alt="image" src="https://github.com/user-attachments/assets/bb441094-bdac-465e-9b8a-c8384d6a13e9" />
</p>



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

## Review personal
La început, proiectul **Wordle** avea o structură destul de rigidă, în care componentele erau **strâns cuplate** între ele. Tot codul era legat într-un singur bloc, ceea ce însemna că logica de funcționare a jocului și **interfața grafică** se intersectau în mod direct. De exemplu, pentru a gestiona click-urile mouse-ului, aveam un lanț lung de instrucțiuni condiționale scris direct în nucleul **motorului de joc**. Din această cauză, adăugarea unui simplu buton sau a unei ferestre noi devenea un proces riscant, deoarece modificările aduse fișierelor principale puteau genera oricând **erori de regresie** în zone care funcționau deja. Logica vizuală suferea de aceeași rigiditate, culorile celulelor și ale tastelor fiind definite prin valori fixe în interiorul claselor respective. Acest lucru făcea imposibilă implementarea dinamică a unor teme alternative, cum ar fi un **mod de noapte**, fără a rescrie codul de la zero. În plus, ferestrele suprapuse pentru introducere, instrucțiuni și final foloseau **cod duplicat**, deoarece nu aveau capacitatea de a reutiliza o structură comună de date. Tot la fel, sistemul inițial de indicii era îngreunat de variabile globale și de calcule secundare pentru un scor arbitrar, fapt care genera desincronizări ale **cursorului grafic** și suprascrierea literelor introduse de utilizator.

Pentru a corecta aceste probleme de arhitectură și a trece la un stil de programare curat și scalabil, am reconstruit proiectul utilizând **programarea generică**, mai exact șabloanele de tip **template**. Modificarea principală a constat în transformarea clasei pentru ferestrele de dialog într-o **clasă șablon**. Prin această abordare, componenta a devenit un **container generic** care își poate adapta conținutul în mod automat, la compilare și la runtime, în funcție de tipul specific de date pe care trebuie să îl afișeze, fie că este vorba de text sau de alte elemente grafice. Pentru a menține interfața sincronizată cu starea jocului, am implementat o funcție membru dedicată **actualizării datelor**, care reîmprospătează ecranul pe baza structurii de date primite, de exemplu, pentru a dezvălui cuvântul secret la final. Flexibilitatea șabloanelor a fost extinsă și în afara claselor prin crearea unei funcții libere template pentru **alinierea geometrică**. Această funcție abstractizează algoritmul de poziționare pe rânduri și coloane, permițând aranjarea uniformă a celulelor din rețea, dar și a butoanelor din meniuri, folosind un singur bloc de cod și eliminând **formulele matematice repetitive** pentru coordonate.

Pe lângă utilizarea șabloanelor, am integrat două șabloane de proiectare fundamentale, numite **design patterns**, cu scopul de a decupla complet modulele aplicației. Pentru gestionarea evenimentelor de tip input, am aplicat modelul **Observer**, definind o **interfață abstractă** pentru notificările de click. Motorul de joc moștenește această interfață și devine un **observator concret**, în timp ce butoanele grafice devin complet agnostice din punct de vedere logic. Ele nu cunosc detaliile din spatele acțiunilor, ci doar își notifică observatorii înregistrați în momentul în care sunt apăsate, lăsând motorul de joc să gestioneze fluxul aplicației. Pentru izolarea logicii vizuale, am apelat la modelul **Strategy**. Am extras algoritmii de selectare a culorilor din clasele celulelor și i-am încapsulat într-o structură separată. Astfel, celulele doar interoghează strategia activă pentru a-și determina culoarea în funcție de starea lor internă, permițând schimbarea completă a **temei grafice** la runtime fără a modifica codul de randare.

În final, sistemul de asistență a fost refactorizat prin transformarea componentei de indicii într-un **manager de stare** predictibil, axat strict pe monitorizarea utilizărilor prin intermediul unui **contor privat**. Logica actuală de funcționare analizează rândul activ, identifică prima celulă liberă și plasează acolo litera corectă din cuvântul soluție. Pentru a asigura regulile de gameplay și a proteja progresul, celula primește o stare unică de **blocare permanentă**, marcată vizual prin culoarea roz, care instruiește funcțiile de adăugare și de ștergere să sară automat peste acea poziție. Sistemul verifică integritatea rândului după fiecare indiciu, iar dacă nu mai există spații goale, trimite un semnal care activează instantaneu **ecranul de final**, prevenind blocarea tastaturii. Limita de utilizare este gestionată controlat, butonul grafic fiind dezactivat și colorat într-o nuanță inactivă imediat ce contorul intern atinge cele **cinci indicii maxime** permise, asigurând o experiență de joc robustă și corectă.

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
