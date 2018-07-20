# Password manager console

One day I woke up with a notification that someone tried to use my Gmail account, only to be thwarted by 2FA. Anyone's normal reaction to that would be to change one's password, right ? Well, not me. Having nothing better to do, I decided to implement my own keyring system, just to see if I could.

It's programmed in C, is pretty lightweight (compared to most of the stuff I did that you can find on my GitHub), but it's still a bit incomplete.

Passwords are saved in Documents\passwordmanager on Windows, and $HOME/.passwordmanager on Linux. No idea on MacOS. Can't test, won't test.

**HOW TO USE**

Just type "help" ! It's pretty easy, really. I mean, it's not a man page, but it should be enough for such a simplistic piece of software.

**TODO**

About the only thing missing from what I would consider a minimum viable product is the ability to delete a row from the "database" (which is just a glorfied .csv).

Encryption could also be a nice feature, although I trust (maybe erroneously) file permissions to handle access control to the passwords file for me. Makes sense on Linux, makes a lot less sense on Windows. Either way, it could be a neat addition, if I ever figure how to do that on my own (I purposely avoided using nonstandard libraries like SQLite for this program)