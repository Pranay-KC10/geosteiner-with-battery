#!/usr/bin/env python3
import argparse, re

def parse_fsts(lines):
    """Split an .fst file into blocks: cost line + following lines until next cost line."""
    blocks, current = [], []
    costline = re.compile(r'^\s*\d+\.\d+')  # line starting with a float
    for line in lines:
        if costline.match(line):
            if current:
                blocks.append(current)
            current = [line]
        else:
            if current:
                current.append(line)
    if current:
        blocks.append(current)
    return blocks

def main():
    ap = argparse.ArgumentParser(description="Prune GeoSteiner .fst files")
    ap.add_argument("infile")
    ap.add_argument("outfile")
    ap.add_argument("-k", type=int, help="Keep top K FSTs by cost")
    ap.add_argument("-p", type=float, help="Keep FSTs within P%% of minimum cost")
    args = ap.parse_args()
    
    with open(args.infile) as f:
        lines = f.readlines()
    
    blocks = parse_fsts(lines)
    if not blocks:
        raise RuntimeError(f"No FST blocks found in {args.infile}")
    
    # extract costs from first line of each block
    costs = []
    for b in blocks:
        try:
            c = float(b[0].split()[0])
        except Exception:
            c = float("inf")
        costs.append(c)
    
    idx = sorted(range(len(costs)), key=lambda i: costs[i])
    keep = set()
    
    if args.k:
        keep.update(idx[:args.k])
    
    if args.p:
        minc = costs[idx[0]]
        for i in idx:
            if costs[i] <= minc * (1 + args.p / 100.0):
                keep.add(i)
    
    if not args.k and not args.p:
        keep = set(idx)
    
    with open(args.outfile, "w") as out:
        for i in sorted(keep):
            for line in blocks[i]:
                if not line.endswith('\n'):
                    line += '\n'
                out.write(line)

if __name__ == "__main__":
    main()