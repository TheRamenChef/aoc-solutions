# [Day 9: Marble Mania](https://adventofcode.com/2018/day/9)

These solutions use two integer inputs, `players` and `maxMarble`, instead of the regular text input.

## Part 1

Let's first make a way of representing the game state. We'll have an integer array representing the numbers on the marbles in the circle, going clockwise, as well as an integer variable for the index of the current marble. Since we can't change the length of the array after we've created it, we'll initialize it to the number of marbles we have (the largest it could possibly be), track its current size, and just ignore any extra space. We'll also have an array of the elves' scores.

    int[] circle = new int[maxMarble + 1];
    int size = 1;
    int current = 0;
    int[] score = new int[players];

When a marble is divisible by 23, we remove a marble by decrementing the size and moving each marble that comes after it in the array one index back:

    score[i % players] += i;
    current -= 7;
    while (current < 0)
        current += size;
    score[i % players] += circle[current];
    size--;
    for (int j = current; j < size; j++)
        circle[j] = circle[j + 1];

Otherwise, we move all marbles with a higher index than the new marble one index forward and increment the size to make space for the new marble:

    current = (current + 2) % size;
    for (int j = size; j > current; j--)
        circle[j] = circle[j - 1];
    size++;
    circle[current] = i;

To make this a bit faster, we can use Java's `System.arraycopy` method to do this reindexing. It uses native code to copy entire sections of memory at once, and runs about twice as fast as a loop in pure Java code. Here's how it'll look all together:

    int[] circle = new int[maxMarble + 1];
    int size = 1;
    int current = 0;
    int[] score = new int[players];
    for (int i = 1; i <= maxMarble; i++) {
        if (i % 23 == 0) {
            score[i % players] += i;
            current -= 7;
            while (current < 0)
                current += size;
            score[i % players] += circle[current];
            size--;
            System.arraycopy(circle, current + 1, circle, current, size - current);
        } else {
            current = (current + 2) % size;
            System.arraycopy(circle, current, circle, current + 1, size - current);
            size++;
            circle[current] = i;
        }
    }
    int max = score[0];
    for (int i = 1; i < players; i++)
        if (max < score[i])
            max = score[i];
    return max;

## Part 2

Our solution for part 1 is too slow. It worked for the smaller numbers in part 1, but if we plug in a number 100 times larger, it will take 10000 times longer to compute. If you can let your computer think on it for several hours, it's doable, but we're looking for a solution that runs in under a minute.

A problem with our solution for part 1 is that whenever we add or remove a marble, the marbles ahead of it all need to be reindexed. If we insert a marble at index 50, and there are 100 marbles already in the circle, that's 50 marbles that need to be moved forward, and this needs to happen every time a marble is added or removed. `System.arraycopy` speeds this process up somewhat, but it still takes longer the more marbles need to be moved. Could there be some way around this problem?

Enter the `Marble` class. It has one field for its point value, and two other (mutable) fields for the marbles immediately adjacent to it (this is basically what the `LinkedList` class uses internally):

    public class Marble {
        public final int value;
        public Marble cw;
        public Marble ccw;

        public Marble(int value, Marble ccw, Marble cw) {
            this.value = value;
            this.ccw = ccw;
            this.cw = cw;
        }
    }

We can initialize our game like this:

    Marble current = new Marble(0, null, null);
    current.cw = current;
    current.ccw = ccw;

We can add a marble to the circle by modifying the `cw` and `ccw` fields of the marbles that should be adjacent to it to point to the new marble:

    toAdd.cw.ccw = toAdd;
    toAdd.ccw.cw = toAdd;

Likewise, we can remove a marble from the circle by modifying the `cw` and `ccw` fields of the adjacent marbles to "skip over" the marble to be removed:

    toRemove.cw.ccw = toRemove.ccw;
    toRemove.ccw.cw = toRemove.cw;

Most importantly, the `Marble` class doesn't need to track indexes: they don't matter anymore. This means that with the `Marble` class, we won't run into the reindexing problem!

There's one other thing, which is that we need to use longs to track scores, since they now overflow 32-bit integers. With this, we have our solution:

    Marble current = new Marble(0, null, null);
    current.cw = current;
    current.ccw = current;
    long[] score = new int[players];
    for (int i = 1; i <= maxMarble; i++) {
        if (i % 23 == 0) {
            score[i % players] += i;
            current = current.ccw.ccw.ccw.ccw.ccw.ccw.ccw; // 7 steps
            score[i % players] += current.value;
            current.cw.ccw = current.ccw;
            current.ccw.cw = current.cw;
            current = current.cw;
        } else {
            current = new Marble(i, current.cw, current.cw.cw);
            current.cw.ccw = current;
            current.ccw.cw = current;
        }
    }
    long max = score[0];
    for (int i = 1; i < players; i++)
        if (max < score[i])
            max = score[i];
    return max;
