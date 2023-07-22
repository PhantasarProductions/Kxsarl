"""   -- Start License block
***********************************************************
Kxsarl/qHead.py
This particular file has been released in the public domain
and is therefore free of any restriction. You are allowed
to credit me as the original author, but this is not
required.
This file was setup/modified in:
2023
If the law of your country does not support the concept
of a product being released in the public domain, while
the original author is still alive, or if his death was
not longer than 70 years ago, you can deem this file
"(c) Jeroen Broks - licensed under the CC0 License",
with basically comes down to the same lack of
restriction the public domain offers. (YAY!)
***********************************************************
Version 23.07.22
-- End License block   """

from glob import glob;

l = glob("Headers/*.hpp")
for h in l:
    print('#include "../'+h.replace('\\','/')+'"')
    
    
# Better to INGORE this script unless you are entitled to modify the repository 
# or working on a pull request. Since the generated file is part of the repository as well
# You might spook things up, y'know!