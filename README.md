# Dll-Streaming

# A simple encrypted Dll/File streaming.

## How does this work?

This program will "stream" your encrypted dll by calling InternetReadFile.
After that it´s being decrypted using a simple blowfish en/decryption algorithm.

## How to use it?

[+] Encryption: 

Change the en/decryption keys. 
https://prnt.sc/SDmYS0ekJ6Me

Drag and drop your dll into the exe, it will now split the file into 4 encrypted parts. Upload them to your web server. 
 - Done.

[+] Decryption:

Include Stream.h and Stream.cpp in your project.

Replace the example urls with your uploaded file urls.
https://prnt.sc/Twz3h8OA-d-T

To stream the file simply call GetFile(). (Like shown and explained in main.cpp)

https://prnt.sc/SFo-md-e4Miu

- Done.


## Credits:

Credits to GH for providing this example.

I decided to make this a litzle better a few years ago. Please keep in mind that this is far away of being anywhere secure. 