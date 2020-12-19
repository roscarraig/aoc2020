#!/usr/bin/env python3

import sys


def match(rules, queue, already):
    if rules[queue[0]]['type'] == 3:
        prefix = already + rules[queue[0]]['val']
        if len(queue) > 1:
            match(rules, queue[1:], prefix)
        else:
            print(prefix)
    else:
        for subrule in rules[queue[0]]['sub']:
            match(rules, subrule + queue[1:], already)


def main():
    rules = {}
    stage = 1
    fh = open(sys.argv[1])

    while stage == 1:
        line = fh.readline()
        if (len(line) == 1):
            stage = 2
        else:
            [num, rule] = line.strip().split(': ')
            rules[num] = {'type': 0, 'sub': list()}
            if rule[0] == "\"":
                rules[num]['type'] = 3
                rules[num]['val'] = rule[1]
            else:
                rules[num]['type'] = 1
                for subrule in rule.split(' | '):
                    rules[num]['sub'].append(subrule.split(' '))

    match(rules, ['0'], "")


if __name__ == '__main__':
    main()
