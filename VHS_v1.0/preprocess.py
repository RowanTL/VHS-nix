#!/usr/bin/env python
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('input_file_prefix')
parser.add_argument('output_file_prefix')
parser.add_argument('-d', '--downsampling', help='downsampling amount', type=int, default=1)
parser.add_argument('-r', '--radius', help='radius of neighborhood in downsampled shape(float)', type=float, default=0.)
parser.add_argument('-l', '--lower', help='lower threshold for shape', type=int, default=0)
parser.add_argument('-u', '--upper', help='upper threshold for shape', type=int, default=255)
parser.add_argument('-n', '--noise', help='Gaussian noise standard deviation', type=int, default=0)
args = parser.parse_args()

from Shape import *
import glob

slices = len(glob.glob(args.input_file_prefix+'*.png'))

s = Shape()
s.loadScalar(args.input_file_prefix, slices, args.lower, args.upper, args.downsampling, args.radius, args.noise)
s.displayStats()
s.save(args.output_file_prefix)
