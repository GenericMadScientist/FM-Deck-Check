# FM Deck Checker

A small command-line program to check starter decks in Yu-Gi-Oh! Forbidden
Memories.

## Install

Download from the [Releases page](../../releases). If you're on something other
than Windows, either compile it yourself or ask me.

## Usage

This is a very simple command-line program. Open up cmd, cd to the directory the
program is in, and enter the known cards like so:

```
> deck-check,exe -c 100 -c 100 -c 8 -c 337
```

After a few moments the program will print out the number of seeds that match.
If all the matching seeds give the same deck, the list of seeds will be printed
along with the deck. Otherwise the program will suggest some cards that will
help in working out the deck.

## Contact

If you have any bug reports I would prefer they be reported on the GitHub page,
but this isn't required. My Discord is GenericMadScientist#5303, and you can
send me an e-mail at rayw1710@gmail.com. Discord will probably get you a quicker
response.