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

##  Evenimente și Interactivitate
Jocul este extrem de receptiv, ascultând activ evenimentele de sistem:

* **Input Mixt**: Utilizatorul poate juca folosind atât **tastatura fizică** (evenimente `TextEntered`), cât și **mouse-ul** prin tastatura virtuală.
* **Sistem de Callbacks**: Butoanele folosesc expresii **lambda** pentru a defini acțiuni (`setOnClick`), permițând o comunicare elegantă între obiecte fără a crea dependențe circulare.
* **Gestiunea Stărilor**: Tranzițiile între ecrane (Intro -> Instrucțiuni -> Joc) sunt gestionate prin flag-uri de vizibilitate în interiorul obiectelor de tip PopUp.

---

##  Gestionarea Erorilor (*Exception Handling*)
Siguranța execuției este garantată printr-o ierarhie proprie de excepții, derivată din `std::exception`:
* **`ResourceException`**: Aruncată în cazul în care activele (fonturi, imagini) lipsesc, prevenind un crash silențios.
<br><br>
* **`WordleException`**: O clasă de bază pentru erori specifice domeniului jocului, capturată în `main` pentru a oferi mesaje de diagnostic clare utilizatorului.
<br><br>
* **`DictionaryException`**: Excepție personalizată care semnalează probleme critice legate de fișierul de cuvinte al jocului. Aceasta este aruncată dacă fișierul `words.txt` lipsește din folderul de resurse sau dacă acesta este găsit, dar nu conține date valide. 

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
- [x] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [x] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie; cele 3 derivate moștenesc aceeași clasă de bază
  - [x] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
  - [x] apelarea constructorului din clasa de bază din constructori din derivate
  - [x] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [x] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [x] smart pointers (recomandat, opțional)
- [x] excepții
  - [x] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (exemplu de erori echivalente: citire fișiere cu diverse extensii)
  - [x] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
- [x] funcții și atribute `static`
- [x] STL
- [x] cât mai multe `const`
- [x] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
- [ ] minim 75-78% din codul propriu să fie C++
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie)
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`
- [ ] code review #2 2 proiecte



## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).
