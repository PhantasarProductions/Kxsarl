If you intend to try to compile the game yourself, a few notes are in order.

# General notes

- No configuration was set into the Visual Studio project files for Debug builds. So trying to compile it in that mode will fail anyway
- Only x64 will do!

# Slyvina

Kxsarl uses the Slyvina framework. The JCR6, Units, Statistician and TQSL libraries or modules or whatever are required. Best way to go here is as follows
~~~powershell
mkdir "Slyvina"
mkdir "Slyvina/Libs"
mkdir "Slyvina/Apps"
cd "Slyvina/libs" 
git "clone https://github.com/Slyvina/TQSL"
git "clone https://github.com/Slyvina/Units"
git "clone https://github.com/Slyvina/JCR6"
git "clone https://github.com/Slyvina/Statistician"
cd "../Apps"
git "https://github.com/PhantasarProductions/Ksxarl"
~~~

Please note that the .lib files and the .dll files for SDL2 (required by TQSL) and zlib (required by JCR6) are not present in the repositories and should be added manually. The header files of both libraries should be in the respositories. I did not wanna break the file limits.

I hope this shone a light on things. 😉
