#!/usr/bin/env python3

# Proving version for day22 part 2

import sys


def part2(decks):
    seen = set()
    c = [len(decks[0]), len(decks[1])]

    while c[0] > 0 and c[1] > 0:
        fp = tuple(decks[0]) + tuple([0]) + tuple(decks[1])
        if(fp in seen):
            return 0
        seen.add(fp)
        top = [decks[0].pop(0), decks[1].pop(0)]

        if top[0] < c[0] and top[1] < c[1]:
            winner = part2([decks[0][:top[0]], decks[1][:top[1]]])
        elif top[0] > top[1]:
            winner = 0
        else:
            winner = 1
        decks[winner].append(top[winner])
        decks[winner].append(top[1 - winner])
        c[winner] += 1
        c[1 - winner] -= 1
    if c[0] > 0:
        return 0
    return 1


def main():
    player = 0
    count = 0
    decks = [list(), list()]
    for line in open(sys.argv[1], 'r').readlines():
        if line[:6] == 'Player':
            player = int(line[7]) - 1
        elif len(line) > 1:
            decks[player].append(int(line))
            count += 1
    part2(decks)
    total = 0
    for i in range(count):
        total += (count - i) * (decks[0] + decks[1])[i]
    winner = (len(decks[1]) > 0) + 1
    print("Winner is player", winner)
    print(total)


if __name__ == '__main__':
    main()
