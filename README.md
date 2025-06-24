# 🏏 Cricket “Masterpiece” Simulator

> *“Brace yourselves: the pinnacle of my questionable C++ prowess.”*  

Welcome to the most gloriously over-engineered (and under-tested) cricket simulator you’ll ever stumble upon. If you’ve ever wondered what happens when someone who barely survived C++17 and a SQLite tutorial attempts to “sim” a full match, congratulations—you’ve found it.

---

## 🤹‍♂️ What Even Is This?

- **A CLI experience** so retro you’ll feel like you’re debugging on a Pentium II.
- **Ball-by-ball “simulation”** that entirely relies on my shaky probability math (and me rolling random numbers until something “works”).
- **SQLite database** because I watched one tutorial and thought, “Yes, let’s persist these half-baked stats forever.”

---

## 🏗️ Project Structure (Brace for Chaos)

Cricket/
├── include/ # Headers I hope are named sensibly
├── src/ # Source files where I desperately try to keep things organized
├── sqlite3.c/.h # The entire SQLite engine because “why install dependencies?”
├── .gitignore # Ignoring trash files (mostly successful)
└── cricket.db # Your new best friend (or relentless storage of my bugs)


---

## 🔨 Building (If You Dare)

1. **Compile SQLite**  
   ```
   gcc -c sqlite3.c -o sqlite3.o
   ```
2. **Compile the “game”**
  ```
  g++ src/*.cpp sqlite3.o -Iinclude -std=c++17 -o cricket_game
  ```
3. **Run and Pray Buddy**
   ```
   ./cricket_game
   ```
Hint: If it crashes, you’re not alone. Feel free to add --debug flags—just kidding, there are none. So just pray !!

---

## 🚨 Known “Features” and “Bugs”

1. **Feature:** You get to see perfectly balanced match results.
2. **Bug:** Sometimes a player scores 237 runs in an over. Embrace it.
3. **Feature:** Persistent stats—until they mysteriously disappear.
4. **Bug:** Database locks if you stare at it too hard.

---

## 🧩 Contributing (For Masochists)
1. Fork this dumpster fire 🔥
2. Make your changes (preferably fixing my mistakes)
3. Submit a PR and watch me cry tears of joy (or despair)
4. Profit (maybe)

---

## 🚦 WORKING OF CODE
No idea buddy, I have not run a single unit test, run it, fix it, play with it because neither you nor I touches grass play in terminal and be happy and pray for me to upgrade it to 2D and further to 3D.

---

## 📜 License
**MIT –** because I’d rather you actually use someone else’s code to do this properly.

---
Enjoy the chaos, and may your debug sessions be merciful.

