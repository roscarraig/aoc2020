#!/usr/bin/env python3

import sys


def seat_id(line):
    row = int(line[0:7].replace('F', '0').replace('B', '1'), 2)
    cross = int(line[7:10].replace('L', '0').replace('R', '1'), 2)
    return row * 8 + cross


def main():
    seats = set([seat_id(line) for line in open(sys.argv[1], 'r').readlines()])
    print(set(range(min(seats), max(seats))) - seats)


if __name__ == '__main__':
    main()
