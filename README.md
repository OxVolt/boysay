# 🧍‍♂️ boysay

**boysay** is a simple and dumb C program — an *alternative* to [cowsay](https://en.wikipedia.org/wiki/Cowsay),  
but instead of a cow, it’s *that one guy* from the meme saying:  
> "You like kissing boys, don't you?"

Because why would a cow say your message when this guy could do it instead?

---

## 💡 What it does

You type something, and the **boy** says it back to you — judgmentally.  
Example:

```bash
$ ./boysay "I love open-source software"
```

Output:
```
 ------------------------------
 I love open-source software
 ------------------------------
        \
         \
          🧍‍♂️ You like kissing boys, don't you?
```

(ASCII art not guaranteed to be pretty. Or consistent. Or even aligned.)

---

## ⚙️ Compilation

You only need a C compiler. On most systems:

```bash
gcc main.c -o boysay
```

That’s it. No dependencies, no nonsense.  
If you can compile “Hello, world!”, you can compile this.

---

## 🧠 Usage

Basic usage:

```bash
./boysay "your message here"
```

If you run it without arguments, **boysay** might just mock you for that too.

---

## 🧑‍💻 Why?

Because *why not*.  
Also, because **cowsay** had its time — now it’s **boysay’s** turn.

---

## 🚧 TODO

- Maybe add more meme characters  
- Maybe add colors  
- Probably not

---

## ❤️ Contribute

Feel free to open a pull request if you have a fun idea or want to improve the boy’s ASCII art.  
No overengineering allowed.

---

**boysay** — bringing cursed energy to your terminal since 2025.
