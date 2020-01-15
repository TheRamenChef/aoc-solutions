# [Day 1](https://adventofcode.com/2015/day/1)

## Part 1

As is usual with first puzzles, this one is extremely simple. Simply iterate over the input, incrementing a counter each time you encounter a `(`, and decrementing it for each `)`. Then we return the counter.

    int floor = 0;
    for (int i = 0; i < input.length(); i++) {
        char c = input.charAt(i);
        if (c == '(')
            floor++;
        else if (c == ')')
            floor--;
        else
            throw new PuzzleException('invalid direction');
    }
    return floor;

## Part 2

Now we just need to use the previous code, but modify it a little.

    int floor = 0;
    for (int i = 0; i < input.length(); i++) {
        char c = input.charAt(i);
        if (c == '(')
            floor++;
        else if (c == ')')
            floor--;
        else
            throw new PuzzleException('invalid direction');
        if (floor < 0)
            return i + 1;
    }
    throw new PuzzleException('does not reach basement');
