# Συχνότητα

<details><summary>Details</summary>
<p>
	
Γράψτε ένα πρόγραμμα (**[hw2a.c](https://github.com/tasos-ana/C/blob/master/Basic/hw2a.c)**) που θα διαβάζει από το πληκτρολόγιο έναν ακέραιο αριθμό
N, στη συνέχεια θα διαβάζει N λέξεις, θα υπολογίζει την πιο συχνή λέξη ανάμεσα στις N και
θα την τυπώνει.

</p>
</details>


# Παλίνδρομο

<details><summary>Details</summary>
<p>
	
Παλίνδρομο είναι μια λέξη ή φράση που διαβάζεται το ίδιο από την αρχή και από το τέλος,
π.χ., “dad”, “anna”, “Never odd or even”, κλπ. Γράψτε ένα πρόγραμμα (**[hw2b.c](https://github.com/tasos-ana/C/blob/master/Basic/hw2b.c)**) που θα διαβά-
ζει έναν αριθμό N και N λέξεις από το πληκτρολόγιο και θα ελέγχει αν οι λέξεις αποτελούν
παλίνδρομο αγνοώντας τα κενά μεταξύ τους. Αν είναι παλίνδρομο, το πρόγραμμα θα τυπώνει:
“Input is a palindrome”, διαφορετικά θα τυπώνει “Input is not a palindrome”. Μπορείτε
να υποθέσετε ότι κάθε λέξη θα είναι το πολύ 20 χαρακτήρες και Ν μικρότερο ή ίσο με 30.

</p>
</details>


# Τρίγωνα

<details><summary>Details</summary>
<p>
	
Γράψτε ένα πρόγραμμα (**[hw2c.c](https://github.com/tasos-ana/C/blob/master/Basic/hw2c.c)**) που θα διαβάζει έναν αριθμό N από το πληκτρολόγιο, και
θα τυπώνει ένα τρίγωνο από ‘*’. Για παράδειγμα, η έξοδος για N = 4 θα είναι:

    *
    **
    * * *
    * * * *
    
</p>
</details>


# Τρίλιζα

<details><summary>Details</summary>
<p>
	
Γράψτε ένα πρόγραμμα (**[hw3a.c](https://github.com/tasos-ana/C/blob/master/Basic/hw3a.c)**) για να παίζουν τρίλιζα 2 χρήστες. Αρχικά το πρόγραμμα
θα τυπώνει μια άδεια τρίλιζα:

         |   |
      ---+---+---
         |	 |
      ---+---+---
         |	 |
και τον παίκτη X ή O που έχει σειρά, π.χ.:
   Player X:

Ο παίκτης δίνει έναν αριθμό από 1 έως 9 ανάλογα με το τετράγωνο που θέλει να παίξει. Αν
δώσει λάθος είσοδο, το πρόγραμμα πρέπει να ξαναζητά απο τον ίδιο παίκτη να παίξει. Στη
συνέχεια το πρόγραμμα θα τυπώνει τη νέα κατάσταση της τρίλιζας, και θα ζητά από τον
άλλο παίκτη να παίξει. Αν κάποιος παίκτης κερδίσει, το παιχνίδι θα πρέπει να τυπώνει Player
X won ή Player O won αντίστοιχα, ή Tie αν δεν κερδίζει κανείς στο τέλος.
	Παράδειγμα εκτέλεσης:
   
        |   |
     ---+---+---
        |   |
     ---+---+---
        |	|
      Player X: 1

      X |   |
     ---+---+---
        |   |
     ---+---+---
        |	|
      Player O: 2

      X | O |
     ---+---+---
        |   |
     ---+---+---
        |	|
      Player X: 4

      X | O |
     ---+---+---
      X |   |
     ---+---+---
        |	|
      Player O: 5

      X | O |
     ---+---+---
      X | O |
     ---+---+---
        |	|
      Player X: 7

      X | O |
     ---+---+---
      X | O |
     ---+---+---
      X |	|
      Player X won
      
</p>
</details>


# Sudoku

<details><summary>Details</summary>
<p>
	
Ένα sudoku 9 x 9 είναι ένας πίνακας με 9 γραμμές και 9 στήλες με τις εξής ιδιότητες:

- Κάθε γραμμή περιέχει όλους τους αριθμούς από 1 έως 9
- Κάθε στήλη περιέχει όλους τους αριθμούς από 1 έως 9
- Κάθε ένα από τα 9 υποτετράγωνα μεγέθους 3x3 που αποτελούν το sudoku περιέχει όλους
τους αριθμούς από 1 έως 9
Για παράδειγμα:

		5 3 4 6 7 8 9 1 2
		6 7 2 1 9 5 3 4 8
		1 9 8 3 4 2 5 6 7
		8 5 9 7 6 1 4 2 3
		4 2 6 8 5 3 7 9 1
		7 1 3 9 2 4 8 5 6
		9 6 1 5 3 7 2 8 4
		2 8 7 4 1 9 6 3 5
		3 4 5 2 8 6 1 7 9
		
Γράψτε ένα πρόγραμμα (**[hw3b.c](https://github.com/tasos-ana/C/blob/master/Basic/hw3b.c)**) που δέχεται 81 αριθμούς και ελέγχει αν αποτελούν sudoku.
Αν ναι, το πρόγραμμα πρέπει να τυπώνει OK. Αν όχι, το πρόγραμμα πρέπει να τυπώνει τις
γραμμές, στήλες και μπλοκ που έχουν λάθος. Για παράδειγμα:

Invalid rows: 1 2 8
Invalid columns: 1
Invalid blocks: 1 7

</p>
</details>

# Πύργος του Ανόι (Hanoi Tower)

<details><summary>Details</summary>
<p>

Γράψτε ένα πρόγραμμα (**[hw4.c](https://github.com/tasos-ana/C/blob/master/Basic/hw4.c)**) που λύνει το παιχνίδι των πύργων του Ανόι:
- Υπάρχουν 3 κατακόρυφοι άξονες
- Αρχικά στον πρώτο άξονα είναι περασμένοι Ν δίσκοι με διαφορετικές διαμέτρους έτσι
ώστε κάθε δίσκος έχει πάνω του μόνο δίσκους με μικρότερες διαμέτρους
- Στους άλλους 2 άξονες αρχικά δεν υπάρχουν δίσκοι
Το ζητούμενο είναι να μεταφερθούν όλοι οι δίσκοι στον τελευταίο άξονα σύμφωνα με τους
κανόνες:
- Σε κάθε κίνηση επιλέγουμε δυο άξονες
- Μπορεί να μετακινηθεί μόνο ο πάνω δίσκος του ενός άξονα στην κορυφή του άλλου άξονα
- Δεν μπορεί να τοποθετηθεί ένας μεγαλύτερος δίσκος πάνω σε ένα μικρότερο
Το πρόγραμμα θα πρέπει να παίρνει τον αριθμό Ν ως παράμετρο από τη γραμμή εντολών,
και να τυπώνει την αρχική κατάσταση του παιχνιδιού. Στη συνέχεια πρέπει να υπολογίζει και
να τυπώνει όλες τις κινήσεις του παιχνιδιού που χρειάζονται για να μετακινηθούν όλοι οι
δίσκοι από τον άξονα A στον άξονα C, και την κατάσταση μετά από κάθε κίνηση. Ο κάθε
άξονας θα αναπαρίσταται με ύψος N+1, η βάση με σύμβολα ’-’ και οι δίσκοι με αύξοντες
αριθμούς ανάλογα με την ακτίνα τους.
Για παράδειγμα, η εκτέλεση του προγράμματος:

	$ gcc100 hw4a.c -o hw4a
	$ ./hw4a 2
	
θα πρέπει να παράγει την έξοδο:
	
	A   B   C
	|   |   |
	1   |   |
	222 |   |
	-----------------

	A -> B
	A   B   C
	|   |   |
	|   |   |
	222 1   |
	-----------------
	A -> C
	A   B   C
	|   |   |
	|   |   |
	|   1   222
	-----------------
	B -> C
	A   B   C
	|   |   |
	|   |   1
	|   |   222
	-----------------
</p>
</details>

# Διαμόρφωση κειμένου

<details><summary>Details</summary>
<p>

Γράψτε ένα πρόγραμμα (**[hw5.c](https://github.com/tasos-ana/C/blob/master/Basic/hw5.c)**) που παίρνει 2 παραμέτρους από τη γραμμή εντολών:
1. το όνομα ενός αρχείου κειμένου, και
2. τον αριθμό Ν των χαρακτήρων ανά γραμμή.

Το πρόγραμμα πρέπει να ανοίγει το δεδομένο αρχείο, να διαβάζει το κείμενο, και να το τυπώ-
νει στην κύρια έξοδο στοιχισμένο σε γραμμές μήκους Ν. Η πρώτη λέξη κάθε γραμμής πρέπει
να αρχίζει στο πρώτο γράμμα της γραμμής, και η τελευταία λέξη να τελειώνει στο τελευταίο
γράμμα της γραμμής, δηλαδή να μην υπάρχουν κενά πριν την πρώτη ή μετά την τελευταία
λέξη κάθε γραμμής. Για κάθε λέξη της εισόδου, το πρόγραμμα πρέπει να υπολογίζει αν χωράει
η λέξη στην τρέχουσα γραμμή και αν ναι, να την προσθέτει. Αν η επόμενη λέξη δεν χωράει
στην τρέχουσα γραμμή πρέπει να μπεί σε νέα γραμμή. Τότε το πρόγραμμα πρέπει να “γεμίσει”
την προηγούμενη με όσα κενά χρειάζονται ανάμεσα στις λέξεις για να πάρει ακριβώς Ν χαρα-
κτήρες, και θα την τυπώνει στην κύρια έξοδο. Τα κενά κάθε γραμμής πρέπει να είναι όσο το
δυνατόν ισοκατανεμημένα μεταξύ των λέξεων. Κατ’ εξαίρεση, η τελευταία σειρά δεν πρέπει
να είναι στοιχισμένη.

**Παράδειγμα χρήσης** Έστω το αρχείο monologue.txt που περιέχει το κείμενο:

	As you know, I’m quite keen of comic books, especially the
	ones about superheroes. I find the whole mythology surrounding
	superheroes fascinating. Take my favorite superhero; Superman, not a
	great comic book, not particularly well-drawn, but the mythology;
	the mythology is not only great, it’s unique.
	Now, the staple of the superhero mythology is;
	there is the superhero and there is the alter ego.
	Batman is actually Bruce Wayne, Spiderman is actually Peter Parker;
	when that character wakes up in the morning, he’s Peter Parker.
	He has to put on a costume to become Spiderman and it is in that
	characteristic, Superman stands alone.
	
Τότε η εντολή ./hw5 monologue.txt 60 θα πρέπει να παράγει την έξοδο:

	As you know, I’m quite keen of comic books, especially the
	ones about superheroes. I find the whole mythology
	surrounding superheroes fascinating. Take my favorite
	superhero; Superman, not a great comic book, not
	particularly well-drawn, but the mythology; the mythology is
	not only great, it’s unique. Now, the staple of the
	superhero mythology is; there is the superhero and there is
	the alter ego. Batman is actually Bruce Wayne, Spiderman is
	actually Peter Parker; when that character wakes up in the
	morning, he’s Peter Parker. He has to put on a costume to
	become Spiderman and it is in that characteristic, Superman
	stands alone.
</p>
</details>
