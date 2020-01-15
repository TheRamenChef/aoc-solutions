# Advent of Code Solutions

In this repository, I will be posting my solutions to Advent of Code puzzles, as well as explaining those solutions. I will be starting with the toughest puzzles, because those have the most interesting solutions.

All solutions posted here will be in Java, though not all of my original solutions used this language. If a block doesn't include a method declaration, it can be assumed to be inside a `solve` method which takes in the puzzle input as a string and returns whatever type the solution uses.

    public static Whatever solve(String input) {
        // code goes here
    }

Some solutions may return `void` instead and use the console to show the solution. I will also be avoiding using some of the standard library methods in favor of implementing them myself, primarily so that I can explain how those classes work and why they're useful.

There is also a `PuzzleException` class for states that should be unreachable with real puzzle inputs.
