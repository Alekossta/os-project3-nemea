# Εξηγησεις συγχρονισμου και critical sections

## Ουρα καταστηματος για καρεκλα
Εχω φτιαξει ενα shared memory segment που μεσα εχει το seats info με πληροφοριες για την κατασταση των τραπεζιων
και καρεκλων. Καθε φορα το ατομο που ειναι μπροστα στην ουρα και "ψαχνει" τραπεζι κανει lock με mutex το seats
info μερος του shared memory segment. Αφου βρει καρεκλα και αλλαξει τα δεδομενα το κανει unlock ετσι ωστε να μπορει
να ψαξει ο επομενος. 

## Ουρα για σερβιρισμα
Εδω το σερβιρισμα το εχω κανει να ειναι με semaphores. Απλα κανουν οι visitor wait στο semaphore και
μετα κανουν sleep για οσο ειναι η ωρα του σερβιρισματος (δεδομενα που εχει "ανεβασει" ο receptionist
στο shared memory). Αφου τελειωσει το σερβιρισμα κανω post για να σερβιριστει ο επομενος

# Βασικα Προγραμματα

## main
Εδω ειναι το "root".
1. Αρχικα κανω μια φορα clear το log αρχειο.
2. Μετα ακουω για signal Ctrl+C για να ελεγχει ο χρηστης ποτε θα τελειωσει η main.
3. Επειτα φτιαχνω ενα shared memory που αποτελειται απο ενα struct SharedMemorySegment με μεσα του αλλα structs
4. Επειτε φτιαχνω ενα mutex lock για το seats info του shared memory
5. Μετα αρχικοποιω το bar και seats info
6. Φτιαχνω το semaphore για το μπαρ σερβιρισματος
7. Φτιαχνω εναν receptionist
8. Φτιαχνω N visitors
9. Τρεχω την main μεχρι να πατησουμε Ctrl+C και να φυγουμε απο το "endless" loop
10. Περιμενω να τελειωσουν οι visitors και ο receptionists
11. Τυπωνω τα στατιστικα
12. Καθαριζω το mutex lock και το shared memory
13. Καθαριζω το ba (για σερβιρισμασ) semaphore

## monitor
Απλα κανει access το shared memory και τυπωνει τα bar και seats info.

## receptionist
Απλα κανει access το shared memory και "ανεβαζει" το max serving time στο shared memory

## visitor