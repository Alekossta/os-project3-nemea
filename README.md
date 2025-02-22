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
1. Αρχικα κανω ενα rand με seed που εχει μεσα το id του process για να αποφυγω
ιδιες συμπεριφορες σε processes που εκτελουνται περιπου την ιδια στιγμη
2. Επειτα ανοιγω τα log file και shared memory
3. Μετα χρησιμοποιω την clock για να αποθηκευσω την ωρα που εφτασε ο visitor
4. Μετα κανω lock στο mutex ετσι ωστε να περιμενω στην ουρα και μολις ερθει η σειρα μου
να ψαξω για τραπεζι αφου ειμαι μπροστα 
5. Αφου βγω απο το loop σημαινει οτι βρηκα τραπεζι
6. Ετσι κανω ενα clock για να δω την ωρα αφου βρηκα τραπεζι
7. Κανω unlock απο το mutex για να ψαξει ο επομενος
8. επειτα ανοιγω το bar semaphore για να περιμενω στο μπαρ
9. αφου ερθει η σημερα με την βοηθεια καποιοων random συναρτησεων επιλεγω τι θα φαω 
και πιω και μετα κανω usleep στο range που εχει max την ωρα που "ανεβασε" ο receptionist στο shared memory
10. επειτα κανω post στο semaphore του bar και μετα παω και κανω usleep στο τραπεζι
με χρονικο διαστημα με range με max οσο πηραμε απο το command line.
11. Αφου τελειωσω αποδεσμευω την καρεκλα
12. Τελος καθαριζω share memory και semaphore

# Βοηθητικα Προγραμματα

## BarInfo
struct για να αποθηκευει τα δεδομενα του μπαρ

## Console Reader
helper για να διαβαζουμε απο την κονσολα

## Logger
helper για να κανουμε append σε αρχειο με timestamps και locks.

## ReceptionistInfo
struct για να αποθηκευει δεδομενα του receptionist

## SeatsInfo
struct για να αποηθκευει δεδομενα σχετικα με τα τραπεζια και τις καρεκλες

## SharedMemorySegment
struct για την αναπαρασταση του shared memory

## Util
utility functionss