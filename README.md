[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


# Password manager console

One day I woke up with a notification that someone tried to use my Gmail account, only to be thwarted by 2FA. Anyone's normal reaction to that would be to change one's password, right ? Well, not me. Having nothing better to do, I decided to implement my own keyring system, just to see if I could.

It is written in C, is pretty lightweight (compared to most of the stuff I did that you can find on my GitHub), but it's still a bit incomplete.

Passwords are saved in Documents\passwordmanager on Windows, and $HOME/.passwordmanager on Linux. No idea on MacOS. Can't test, won't test.
**UPDATE 22/7/18:** MacOS version will compile but not work as intended due to clang. Since I do not actually have a Mac myself, I cannot debug it. Feel free to, if you have the time and can bother with clang.

**HOW TO USE**

Just type "help" ! It's pretty easy, really. I mean, it's not a man page, but it should be enough for such a simplistic piece of software.

**TODO**

Encryption could also be a nice feature, although I trust (maybe erroneously) file permissions to handle access control to the passwords file for me. Makes sense on Linux, makes a lot less sense on Windows. Either way, it could be a neat addition, if I ever figure how to do that on my own (I purposely avoided using nonstandard libraries like SQLite for this program).

# IMPORTANT NOTE

As an individual, I believe long but easy to remember passphrases are better than passwords generated by pretty much anything, regardless of length and complexity. See [here](https://www.explainxkcd.com/wiki/index.php/936:_Password_Strength) for a better explanation than I can come up with. I only made this software to test myself, and while it *may* be production-ready (even without encryption, if your GPOs are solid, and/or if you place blind trust in OS access control lists), it isn't meant to be used in a professional environment. Please keep that in mind.
