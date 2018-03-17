# Python Version: 3.x
# -*- coding: utf-8 -*-
from colorama import Fore, Back, Style
import json
import subprocess
import sys

def command_get(args):
    data = []
    for seed in range(1, args.n + 1):
        proc = subprocess.run([ './a.out', '-json', '-seed', str(seed) ], stdout=subprocess.PIPE)
        print(proc.stdout.decode(), file=sys.stderr)
        lines = proc.stdout.splitlines()
        data += [ json.loads(lines[-1].decode()) ]
    json.dump(data, sys.stdout)

def command_diff(args):
    with open(args.a) as fh:
        a = json.load(fh)
    with open(args.b) as fh:
        b = json.load(fh)
    assert len(a) == len(b)
    score_a = 0
    score_b = 0
    for i in range(len(a)):
        assert a[i]['seed'] == b[i]['seed']
        seed = a[i]['seed']
        coins = a[i]['coins']
        maxTime = a[i]['maxTime']
        noteTime = a[i]['noteTime']
        numMachines = a[i]['numMachines']
        val_a = a[i]['result']
        val_b = a[i]['result']
        score_a += val_a / max(1, val_a, val_b)
        score_b += val_b / max(1, val_a, val_b)
        if val_a > val_b:
            style_a = Fore.GREEN
            style_b = Fore.RED
        elif val_a < val_b:
            style_a = Fore.RED
            style_b = Fore.GREEN
        else:
            style_a = ''
            style_b = ''
        s = ''.join([
            str(seed).rjust(6),
            style_a, str(val_a).rjust(6), Style.RESET_ALL,
            style_b, str(val_b).rjust(6), Style.RESET_ALL,
            str(coins).rjust(6),
            str(maxTime).rjust(6),
            str(noteTime).rjust(6),
            str(numMachines).rjust(6),
        ])
        print(s)
    print(score_a, score_b)

def main():
    import argparse
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='command')
    parser_info = subparsers.add_parser('get')
    parser_info.add_argument('-n', type=int, default=300)
    parser_diff = subparsers.add_parser('diff')
    parser_diff.add_argument('a')
    parser_diff.add_argument('b')
    args = parser.parse_args()

    if args.command == 'get':
        command_get(args)
    elif args.command == 'diff':
        command_diff(args)
    else:
        parser.print_help()

if __name__ == '__main__':
    main()
