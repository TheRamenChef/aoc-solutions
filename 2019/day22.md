# [Day 22: Slam Shuffle](https://adventofcode.com/2019/day/22)

Just a quick note: these explanations use a positive modulo, equivalent to `Math.floorMod`. The `%` operator will return a negative value if the dividend is negative, whereas `Math.floorMod` will always return a positive value.

## Part 1

For part 1, the deck's (relatively) small size makes the method of physically simulating the shuffles with an array fast enough to be feasible. But it's even faster to just track the position of the `2019` card:

  - `deal into new stack` will move the card at position *x* to position 10006&ndash;*x*.
  - `cut N` will move the card at position *x* to position *x*&ndash;*N* modulo 10007.
  - `deal with increment N` will move the card at position *x* to position *Nx* modulo 10007.

Here's how it'll look:

    String[] steps = input.split("\n");
    int pos = 2019;
    for (int i = 0; i < steps.length; i++) {
        String step = steps[i];
        if (step.equals("deal into new stack"))
            pos = 10006 - pos;
        else if (step.startsWith("cut "))
            pos = Math.floorMod(pos - Integer.parseInt(step.substring(4)), 10007);
        else if (step.startsWith("deal with increment "))
            pos = pos * Integer.parseInt(step.substring(20)) % 10007;
        else
            throw new PuzzleException("invalid step");
    }
    return pos;

## Part 2

Buckle up.

We'll start by adding some constants for the number of cards we have and the number of times we're shuffling the deck.

    private static final long NUM_CARDS = 119315717514047L;
    private static final long NUM_SHUFFLES = 101741582076661L;

Since simulating the entire hundred-trillion-card deck is obviously infeasible, we'll need some way of simplifying the shuffles. Let's define a set ***S*** of shuffles representable as some *a* and *c*, both between 0 and `NUM_CARDS - 1` inclusive, where the new card at position *x* was originally at position *ax+c* modulo `NUM_CARDS`. Because of the way modulo works, `deal into new stack` is a member of ***S*** with both *a* and *c* equal to `NUM_CARDS - 1`. `cut N` is a clear member as well, with *a* being 1 and *c* being equal to *N*. `deal with increment N` is a bit harder.

If position 1 needs to receive a card, then there must be a certain number of steps, *k*, after which you arrive at the position after the original position. By repeatedly taking this number of steps and then modding the step count by the number of cards, you can reach the entire deck this way in order. This means that after the instruction, the new card at position *x* was originally at position *kx* modulo `NUM_CARDS`. In other words, the instruction is a member of ***S*** with *a* equal to *k* and *c* equal to zero. Let's add a method to get the *k* value for any *N*:

    public static long getK(int n) {
        for (int i = 0; i < n; i++) {
            long steps = NUM_CARDS * i + 1;
            if (steps % n == 0)
                return steps / n;
        }
        throw new PuzzleException("no k value");
    }

This will find the *k* value if it exists, which it will for any puzzle input.

Two consecutive shuffles from ***S*** is also a shuffle from ***S***. This means that, by concatenating them together, *any sequence of shuffle steps* can be represented as a single shuffle from ***S***. But before we can start concatenating shuffles, we'll need a way to find the product of two values modulo `NUM_CARDS` without overflowing our 64-bit integers in intermediate values. We can do this bit-by-bit, applying our modulus each step of the way to avoid an overflow:

    public static long multiplyWithModulo(long a, long b) {
        long res = 0L;
        for (long bit = Long.highestOneBit(a); bit > 0; bit >>>= 1) {
            res <<= 1;
            if ((a & bit) != 0L)
                res += b;
            res %= NUM_CARDS;
        }
        return res;
    }

Now we can implement a class for shuffles from ***S***.

    public class Shuffle {
        private final long a;
        private final long c;

        public Shuffle(long a, long c) {
            this.a = a;
            this.c = c;
        }

        public Shuffle concat(Shuffle then) {
            return new Shuffle(multiplyWithModulo(then.a, this.a), this.apply(then.c));
        }

        public long apply(long pos) {
            return (multiplyWithModulo(pos, a) + c) % NUM_CARDS;
        }
    }

Now, we can start our solver method by constructing a shuffle representing a single repetition of the shuffling method:

    String[] steps = input.split("\n");
    Shuffle shuffle = new Shuffle(1L, 0L); // identity shuffle
    for (int i = 0; i < steps.length; i++) {
        String step = steps[i];
        if (step.equals("deal into new stack"))
            shuffle = shuffle.concat(new Shuffle(NUM_CARDS - 1, NUM_CARDS - 1));
        else if (step.startsWith("cut "))
            shuffle = shuffle.concat(new Shuffle(1L, Math.floorMod(Long.parseLong(step.substring(4)), NUM_CARDS)));
        else if (step.startsWith("deal with increment "))
            shuffle = shuffle.concat(new Shuffle(getK(Integer.parseInt(step.substring(20))), 0L));
        else
            throw new PuzzleException("invalid step");
    }

Next, all of the individual shuffle iterations combine together to form a giant mega-shuffle from which we can get our answer:

    Shuffle outShuffle = new Shuffle(1L, 0L);
    for (long bit = 1L; bit <= NUM_SHUFFLES; bit <<= 1) {
        if ((bit & NUM_SHUFFLES) != 0L)
            outShuffle = outShuffle.concat(shuffle);
        shuffle = shuffle.concat(shuffle);
    }
    return outShuffle.apply(2020L);
