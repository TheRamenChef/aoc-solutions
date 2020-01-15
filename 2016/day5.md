# [Day 5: How About a Nice Game of Chess?](https://adventofcode.com/2016/day/5)

## Part 1

MD5 is relatively hard to implement, which is why many programming languages have it as a builtin. Java has the `MessageDigest` class, which implements several different hash algorithms, including MD5. We do need to acquire the MD5 instance before we can use it, though:

    MessageDigest md5 = MessageDigest.getInstance("MD5");

Now we can continue. We'll also keep an array around for the hexadecimal characters:

    private static final char[] HEX_CHARS = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };

We can test the first four characters in the hash by testing the first two bytes equal to zero, and we can do the same with the fifth character using a bitwise AND.

    String password = "";
    for (int i = 0; ; i++) {
        byte[] hash = md5.digest((input + i).getBytes());
        md5.reset();
        if (hash[0] == 0 && hash[1] == 0 && (hash[2] & 0xf0) == 0) {
            password += HEX_CHARS[hash[2]];
            if (password.length() >= 8)
                return password;
        }
    }

## Part 2

Because "invalid positions" are equivalent to positions &ge;8, we can catch invalid positions by simply extending our zero-test a single bit. We'll also make a cinematic "decrypting" animation as suggested by the puzzle.

    System.out.println("DECRYPTING PASSWORD...");
    System.out.println("________");
    MessageDigest md5 = MessageDigest.getInstance("MD5");
    char[] password = {'_', '_', '_', '_', '_', '_', '_', '_'};
    int chars = 0;
    for (int i = 0; ; i++) {
        byte[] hash = md5.digest((input + i).getBytes());
        md5.reset();
        if (hash[0] == 0 && hash[1] == 0 && (hash[2] & 0xf8) == 0 && password[hash[2]] == '_') {
            password[hash[2]] = HEX_CHARS[hash[3] >>> 4];
            String psf = new String(password);
            System.out.println(psf);
            if (++chars == 8)
                return psf;
        }
    }
