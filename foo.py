# Python Version: 3.x
# -*- coding: utf-8 -*-
from colorama import Fore, Back, Style
import json
import pandas as pd
import subprocess

def command_info(args):
    columns = [ 'seed', 'coins', 'maxTime', 'noteTime', 'numMachines' ]
    print(','.join(columns))
    for seed in range(1, args.n + 1):
        proc = subprocess.run([ './a.out', '-json', '-seed', str(seed) ], stdout=subprocess.PIPE)
        lines = proc.stdout.splitlines()
        data = json.loads(lines[-1].decode())
        print(','.join([ str(data[column]) for column in columns ]))

def command_diff(args):
    df = pd.read_csv(args.info, index_col='seed')
    df = pd.merge(df, pd.read_csv(args.a, index_col='seed').rename(columns={ 'result': 'a' }), left_index=True, right_index=True)
    df = pd.merge(df, pd.read_csv(args.b, index_col='seed').rename(columns={ 'result': 'b' }), left_index=True, right_index=True)
    score_a = 0
    score_b = 0
    for seed in df.index:
        coins = df.loc[seed]['coins']
        maxTime = df.loc[seed]['maxTime']
        noteTime = df.loc[seed]['noteTime']
        numMachines = df.loc[seed]['numMachines']
        val_a = df.loc[seed]['a']
        val_b = df.loc[seed]['b']
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
    parser_info = subparsers.add_parser('info')
    parser_info.add_argument('n', type=int)
    parser_diff = subparsers.add_parser('diff')
    parser_diff.add_argument('a')
    parser_diff.add_argument('b')
    parser_diff.add_argument('--info', default='log/info.csv')
    args = parser.parse_args()

    if args.command == 'info':
        command_info(args)
    elif args.command == 'diff':
        command_diff(args)

if __name__ == '__main__':
    main()
